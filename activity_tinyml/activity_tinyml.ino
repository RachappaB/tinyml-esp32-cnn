#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model_data.h"
#include "scaler_params.h"

// ============================================================
// LED configuration
// ============================================================

const int LED_RED = 0;
const int LED_GREEN = 1;
// ============================================================
// MPU6050
// ============================================================

Adafruit_MPU6050 mpu;


// ============================================================
// Model
// ============================================================

const tflite::Model* model = nullptr;

tflite::MicroInterpreter* interpreter = nullptr;


// ============================================================
// Tensor arena
// ============================================================

// Start with 160 KB.
// If allocation fails, we will increase it.
constexpr int kTensorArenaSize = 160 * 1024;

uint8_t tensor_arena[kTensorArenaSize];


// ============================================================
// Input / output tensors
// ============================================================

TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;


// ============================================================
// Sensor configuration
// ============================================================

constexpr int SAMPLE_RATE = 50;
constexpr int WINDOW_SIZE = 100;
constexpr int NUM_FEATURES = 6;


// ============================================================
// Sensor buffer
// ============================================================

float sensor_buffer[WINDOW_SIZE][NUM_FEATURES];

void updateLEDs(int predicted) {

  switch (predicted) {

    case 0:
      // SETTING
      // Green only
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      break;


    case 1:
      // WALKING
      // Red + Green
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, HIGH);
      break;


    case 2:
      // RUNNING
      // Red only
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      break;
  }
}
// ============================================================
// Setup
// ============================================================

void setup() {

  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("================================");
  Serial.println(" ESP32-C3 TinyML Activity Model ");
  Serial.println("================================");

  // ----------------------------------------------------------
  // I2C
  // ----------------------------------------------------------

  Wire.begin(8, 9);

  // ----------------------------------------------------------
  // MPU6050
  // ----------------------------------------------------------

  if (!mpu.begin()) {

    Serial.println("ERROR: MPU6050 not found!");

    while (1) {
      delay(1000);
    }
  }

  Serial.println("MPU6050 connected.");
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Start with both LEDs OFF
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);


  // ----------------------------------------------------------
  // Load TensorFlow Lite model
  // ----------------------------------------------------------

  model = tflite::GetModel(activity_model);

  if (model->version() != TFLITE_SCHEMA_VERSION) {

    Serial.println("ERROR: Model schema mismatch!");

    while (1) {
      delay(1000);
    }
  }

  Serial.println("TFLite model loaded.");


  // ----------------------------------------------------------
  // Register only required operators
  // ----------------------------------------------------------

  static tflite::MicroMutableOpResolver<9> resolver;

  resolver.AddExpandDims();
  resolver.AddConv2D();
  resolver.AddMul();
  resolver.AddAdd();
  resolver.AddReshape();
  resolver.AddMaxPool2D();
  resolver.AddMean();
  resolver.AddFullyConnected();
  resolver.AddSoftmax();


  // ----------------------------------------------------------
  // Create interpreter
  // ----------------------------------------------------------

  static tflite::MicroInterpreter static_interpreter(
    model,
    resolver,
    tensor_arena,
    kTensorArenaSize
  );

  interpreter = &static_interpreter;


  // ----------------------------------------------------------
  // Allocate tensors
  // ----------------------------------------------------------

  TfLiteStatus status = interpreter->AllocateTensors();

  if (status != kTfLiteOk) {

    Serial.println("ERROR: AllocateTensors() failed!");

    while (1) {
      delay(1000);
    }
  }

  Serial.println("Tensor allocation successful.");


  // ----------------------------------------------------------
  // Get input/output
  // ----------------------------------------------------------

  input = interpreter->input(0);
  output = interpreter->output(0);


  Serial.println();
  Serial.println("Model information:");

  Serial.print("Input type: ");
  Serial.println(input->type);

  Serial.print("Input dimensions: ");

  for (int i = 0; i < input->dims->size; i++) {
    Serial.print(input->dims->data[i]);

    if (i < input->dims->size - 1) {
      Serial.print(" x ");
    }
  }

  Serial.println();


  Serial.print("Output type: ");
  Serial.println(output->type);

  Serial.println();
  Serial.println("TinyML initialization complete.");
}


// ============================================================
// Loop
// ============================================================

void loop() {

  Serial.println();
  Serial.println("Collecting 100 samples...");

  const unsigned long sample_interval = 1000 / SAMPLE_RATE;

  unsigned long start_time = millis();


  // ----------------------------------------------------------
  // Collect 100 samples
  // ----------------------------------------------------------

  for (int i = 0; i < WINDOW_SIZE; i++) {

    unsigned long sample_start = millis();

    sensors_event_t a;
    sensors_event_t g;
    sensors_event_t temp;

    mpu.getEvent(&a, &g, &temp);


    sensor_buffer[i][0] = a.acceleration.x;
    sensor_buffer[i][1] = a.acceleration.y;
    sensor_buffer[i][2] = a.acceleration.z;

    sensor_buffer[i][3] = g.gyro.x;
    sensor_buffer[i][4] = g.gyro.y;
    sensor_buffer[i][5] = g.gyro.z;


    // Maintain approximately 50 Hz
    while (millis() - sample_start < sample_interval) {
      delay(1);
    }
  }


  Serial.print("Window collected in ");
  Serial.print(millis() - start_time);
  Serial.println(" ms");


  // ----------------------------------------------------------
  // Normalize + quantize
  // ----------------------------------------------------------

  float input_scale = input->params.scale;
  int input_zero_point = input->params.zero_point;


  for (int i = 0; i < WINDOW_SIZE; i++) {

    for (int j = 0; j < NUM_FEATURES; j++) {

      float normalized =
        (sensor_buffer[i][j] - scaler_mean[j])
        / scaler_scale[j];


      int32_t quantized =
        round(normalized / input_scale)
        + input_zero_point;


      if (quantized > 127) {
        quantized = 127;
      }

      if (quantized < -128) {
        quantized = -128;
      }


      int index =
        i * NUM_FEATURES + j;


      input->data.int8[index] =
        (int8_t)quantized;
    }
  }


  // ----------------------------------------------------------
  // Run inference
  // ----------------------------------------------------------

  Serial.println("Running inference...");

  TfLiteStatus status = interpreter->Invoke();

  if (status != kTfLiteOk) {

    Serial.println("ERROR: Inference failed!");

    delay(1000);

    return;
  }


  // ----------------------------------------------------------
  // Read output
  // ----------------------------------------------------------

  float output_scale = output->params.scale;
  int output_zero_point = output->params.zero_point;


  float probabilities[3];


  for (int i = 0; i < 3; i++) {

    probabilities[i] =
      (output->data.int8[i] - output_zero_point)
      * output_scale;
  }


  // ----------------------------------------------------------
  // Find highest probability
  // ----------------------------------------------------------

  int predicted = 0;

  if (probabilities[1] > probabilities[predicted]) {
    predicted = 1;
  }

  if (probabilities[2] > probabilities[predicted]) {
    predicted = 2;
  }


  const char* labels[] = {
    "SETTING",
    "WALKING",
    "RUNNING"
  };


  // ----------------------------------------------------------
  // Print result
  // ----------------------------------------------------------

  Serial.println();
  Serial.println("========== RESULT ==========");

  Serial.print("Setting : ");
  Serial.println(probabilities[0], 4);

  Serial.print("Walking : ");
  Serial.println(probabilities[1], 4);

  Serial.print("Running : ");
  Serial.println(probabilities[2], 4);

  // Serial.print("Prediction: ");
  // Serial.println(labels[predicted]);

  // Serial.println("============================");

  // delay(500);

  Serial.print("Prediction: ");
  Serial.println(labels[predicted]);

  // Update LEDs according to activity
  updateLEDs(predicted);

  Serial.println("============================");

  delay(500);
}