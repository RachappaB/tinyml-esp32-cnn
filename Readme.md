# TinyML Activity Recognition with ESP32

A **TinyML activity recognition project** that uses motion sensor data, a **1D Convolutional Neural Network (CNN)**, and an **ESP32** to perform activity classification directly on an embedded device.

The project is divided into two main parts:

- **Machine Learning (`tinyml/`)** — data collection, preprocessing, model training, evaluation, and TensorFlow Lite conversion.
- **ESP32 / Arduino (`activity_tinyml/`)** — deployment of the trained model on an ESP32 using TensorFlow Lite Micro.

---

## 📁 Project Structure

```text
tinyml-esp32-cnn/
│
├── tinyml/
│   │
│   ├── tinyml.ipynb
│   │
│   ├── walking.jsonl
│   ├── running.jsonl
│   │
│   ├── setting.jsonl
│   ├── y.csv
│   │
│   └── data/
│       ├── walking.csv
│       ├── running.csv
│       ├── setting.csv
│       │
│       ├── scaler.pkl
│       ├── scaler_params.h
│       │
│       ├── activity_model.keras
│       ├── activity_model.tflite
│       ├── activity_model_int8.tflite
│       │
│       ├── model_data.cc
│       └── model_data.h
│
├── activity_tinyml/
│   ├── activity_tinyml.ino
│   ├── model_data.cpp
│   ├── model_data.h
│   └── scaler_params.h
│
└── README.md
```

---

# 🧠 Machine Learning

The `tinyml/` directory contains everything required for the machine-learning pipeline.

The workflow is:

```text
Sensor Data
     ↓
Data Collection
     ↓
Data Cleaning
     ↓
Preprocessing
     ↓
Feature Scaling
     ↓
CNN Training
     ↓
Model Evaluation
     ↓
TensorFlow Lite
     ↓
INT8 Quantization
     ↓
ESP32 Deployment
```

## Dataset

The project currently contains activity data such as:

- Walking
- Running
- Setting

The raw/processed data is stored in:

```text
tinyml/data/
```

Example:

```text
walking.csv
running.csv
setting.csv
```

The JSONL files contain collected activity samples:

```text
walking.jsonl
running.jsonl
setting.jsonl
```

---

# 🔬 CNN Model

The activity recognition model uses a **1D Convolutional Neural Network (CNN)**.

A 1D CNN is suitable for sequential sensor data because the input consists of measurements changing over time.

Typical sensor data can be represented as:

```text
Time
 ↓

X-axis ────────┐
Y-axis ────────┼──→ CNN ──→ Activity
Z-axis ────────┘
```

The CNN learns patterns in the sensor signal that correspond to different physical activities.

---

# ⚙️ Model Files

The trained models are stored inside:

```text
tinyml/data/
```

### Keras model

```text
activity_model.keras
```

Used for:

- Training
- Evaluation
- Further experimentation

### TensorFlow Lite model

```text
activity_model.tflite
```

Used for TensorFlow Lite deployment.

### INT8 TensorFlow Lite model

```text
activity_model_int8.tflite
```

The INT8 model is optimized for TinyML deployment and can significantly reduce:

- Flash usage
- RAM usage
- Computational requirements

This makes it more suitable for microcontrollers such as ESP32.

---

# 📊 Data Preprocessing

The project uses a scaler to normalize the sensor data before feeding it into the CNN.

The Python scaler is stored as:

```text
scaler.pkl
```

The parameters required by the ESP32 are exported to:

```text
scaler_params.h
```

This allows the same preprocessing used during training to be reproduced on the ESP32.

```text
Training:

Raw Sensor Data
       ↓
     Scaler
       ↓
Normalized Data
       ↓
      CNN


ESP32:

Raw Sensor Data
       ↓
scaler_params.h
       ↓
Normalized Data
       ↓
TFLite Micro Model
```

Using the same preprocessing parameters is important because the model expects input data in the same numerical distribution used during training.

---

# 📓 Jupyter Notebook

The main machine-learning development notebook is:

```text
tinyml/tinyml.ipynb
```

It contains the ML workflow for:

1. Loading the dataset
2. Inspecting sensor data
3. Preprocessing
4. Scaling
5. Preparing training data
6. Creating the CNN
7. Training the model
8. Evaluating the model
9. Converting the model to TensorFlow Lite
10. Quantizing the model
11. Preparing the model for ESP32 deployment

---

# 🤖 ESP32 Deployment

The `activity_tinyml/` directory contains the Arduino/ESP32 implementation.

```text
activity_tinyml/
│
├── activity_tinyml.ino
├── model_data.cpp
├── model_data.h
└── scaler_params.h
```

## Arduino Sketch

The main ESP32 program is:

```text
activity_tinyml.ino
```

It loads the TinyML model and performs inference on the ESP32.

---

# 📦 Model Conversion for ESP32

TensorFlow Lite models cannot simply be loaded directly like a normal file on many embedded environments.

The trained model is converted into C/C++ data.

```text
activity_model_int8.tflite
            ↓
       Model Conversion
            ↓
       model_data.h
       model_data.cpp
            ↓
          ESP32
```

The generated model data is included in the Arduino project.

---

# 🧩 TensorFlow Lite Micro

The ESP32 deployment uses **TensorFlow Lite Micro**.

The inference pipeline is approximately:

```text
Sensor
  ↓
ESP32
  ↓
Collect Sensor Window
  ↓
Preprocessing
  ↓
Scaling
  ↓
TensorFlow Lite Micro
  ↓
CNN Inference
  ↓
Activity Prediction
```

The model runs locally on the ESP32 rather than sending sensor data to a server.

This is an example of **Edge AI / TinyML**.

---

# 🚀 Getting Started

## 1. Clone the Repository

```bash
git clone https://github.com/<your-username>/tinyml-esp32-cnn.git
cd tinyml-esp32-cnn
```

---

# 🐍 Machine Learning Setup

Create a Python virtual environment:

```bash
python3 -m venv venv
```

Activate it:

```bash
source venv/bin/activate
```

Install the required packages:

```bash
pip install tensorflow numpy pandas scikit-learn matplotlib jupyter joblib
```

Start Jupyter:

```bash
jupyter notebook
```

Open:

```text
tinyml/tinyml.ipynb
```

---

# 🔧 ESP32 Setup

Open the Arduino project:

```text
activity_tinyml/activity_tinyml.ino
```

Install/configure:

- Arduino IDE
- ESP32 board support
- TensorFlow Lite Micro / compatible TFLite Micro library
- Required sensor libraries

Select the appropriate ESP32 board and serial port.

Then compile and upload the sketch.

---

# 📡 Hardware

The project is designed for an ESP32-based TinyML setup.

Possible hardware configuration:

```text
       MPU6050
       ┌───────┐
       │ Accel │
       │ Gyro  │
       └───┬───┘
           │
           │ I²C
           │
       ┌───▼────┐
       │  ESP32 │
       │        │
       │ CNN    │
       │ Model  │
       └───┬────┘
           │
           ▼
     Activity Result
```

---

# 🎯 Project Goal

The main goal of this project is to demonstrate how a machine-learning model can be trained on a computer and then deployed to a resource-constrained microcontroller.

Instead of:

```text
Sensor → Internet → Server → ML Model → Result
```

this project aims for:

```text
Sensor → ESP32 → ML Model → Result
```

This reduces dependency on:

- Internet connectivity
- Cloud servers
- External APIs
- High-performance computers

---

# 🌐 TinyML Concept

TinyML brings machine learning capabilities to low-power embedded devices.

Traditional ML:

```text
Sensor
  ↓
Internet
  ↓
Cloud Server
  ↓
ML Model
  ↓
Prediction
```

TinyML:

```text
Sensor
  ↓
Microcontroller
  ↓
ML Model
  ↓
Prediction
```

This enables applications such as:

- Activity recognition
- Gesture recognition
- Anomaly detection
- Predictive maintenance
- Keyword spotting
- Sensor classification
- Wearable devices
- Smart IoT devices

---

# 📈 Future Improvements

Planned improvements include:

- [ ] Add more activities
- [ ] Improve dataset size
- [ ] Improve classification accuracy
- [ ] Add confusion matrix
- [ ] Add model accuracy/loss graphs
- [ ] Optimize INT8 model
- [ ] Measure ESP32 inference latency
- [ ] Measure RAM usage
- [ ] Measure Flash usage
- [ ] Add real-time activity display
- [ ] Add OLED display
- [ ]
