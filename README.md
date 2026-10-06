# 🧠 TinyML Activity Recognition with ESP32

A complete **TinyML / Edge AI activity-recognition project** using sensor data, a CNN model, TensorFlow Lite, ESP32, Arduino, and a Node.js/Express server.

The project covers the complete workflow:

```text
Sensor Data
    ↓
ESP32 / MPU6050
    ↓
Node.js / Express
    ↓
Dataset Creation
    ↓
Python + TensorFlow / Keras
    ↓
CNN Training
    ↓
TensorFlow Lite
    ↓
INT8 Quantization
    ↓
ESP32 + TensorFlow Lite Micro
    ↓
Activity Prediction
```

## 🎯 Activities

The current project contains data and demonstrations for:

- 🚶 Walking
- 🏃 Running
- 🪑 Sitting

---

# 📁 Repository Structure

```text
tinyml-esp32-cnn/
│
├── activity_tinyml/
│   ├── activity_tinyml.ino
│   ├── model_data.cpp
│   ├── model_data.h
│   └── scaler_params.h
│
├── tinyml/
│   ├── tinyml.ipynb
│   ├── running.jsonl
│   ├── setting.jsonl
│   ├── walking.jsonl
│   │
│   └── data/
│       ├── activity_model.keras
│       ├── activity_model.tflite
│       ├── activity_model_int8.tflite
│       ├── model_data.cc
│       ├── model_data.h
│       ├── running.csv
│       ├── setting.csv
│       ├── walking.csv
│       ├── scaler.pkl
│       ├── scaler_params.h
│       └── y.csv
│
├── my-express-server/
│   ├── server.js
│   ├── package.json
│   ├── package-lock.json
│   ├── mpu_data.csv
│   ├── ruuning.jsonl
│   ├── sitting.jsonl
│   └── walking.jsonl
│
├── vedios/
│   ├── running.mp4
│   ├── sitting.mp4
│   └── walking.mp4
│
└── README.md
```

---

# 🧩 Project Components

## 1. 🧠 Machine Learning — `tinyml/`

The `tinyml/` directory contains the machine-learning workflow.

Main notebook:

```text
tinyml/tinyml.ipynb
```

It is used for:

1. Loading sensor data
2. Preparing the dataset
3. Preprocessing
4. Feature scaling
5. Preparing training data
6. Building the CNN
7. Training the model
8. Evaluating the model
9. Converting the model to TensorFlow Lite
10. INT8 quantization
11. Preparing the model for ESP32 deployment

---

## 2. 🤖 ESP32 / Arduino — `activity_tinyml/`

The ESP32 deployment project is located in:

```text
activity_tinyml/
```

Main files:

```text
activity_tinyml.ino
model_data.cpp
model_data.h
scaler_params.h
```

The Arduino sketch runs the TinyML inference application on the ESP32.

---

## 3. 🌐 Node.js / Express Server — `my-express-server/`

The project includes a Node.js/Express server for sensor-data collection.

```text
my-express-server/
├── server.js
├── package.json
├── package-lock.json
├── mpu_data.csv
├── ruuning.jsonl
├── sitting.jsonl
└── walking.jsonl
```

Install dependencies:

```bash
cd my-express-server
npm install
```

Run the server:

```bash
node server.js
```

---

# 🔬 Machine Learning Pipeline

```text
             SENSOR DATA
                  │
                  ▼
          ┌───────────────┐
          │     ESP32     │
          │    MPU6050    │
          └───────┬───────┘
                  │
                  ▼
          ┌───────────────┐
          │ Node.js /     │
          │ Express       │
          └───────┬───────┘
                  │
                  ▼
             DATASET
                  │
                  ▼
          ┌───────────────┐
          │ TensorFlow /  │
          │ Keras         │
          └───────┬───────┘
                  │
                  ▼
                CNN
                  │
                  ▼
        TensorFlow Lite
                  │
                  ▼
          INT8 Quantization
                  │
                  ▼
          C/C++ Model Data
                  │
                  ▼
          ┌───────────────┐
          │     ESP32     │
          │ TFLite Micro  │
          └───────┬───────┘
                  │
                  ▼
          ACTIVITY RESULT
```

---

# 📊 Dataset

The machine-learning data contains activity classes including:

| Activity | Dataset |
|---|---|
| 🚶 Walking | `walking.csv` |
| 🏃 Running | `running.csv` |
| 🪑 Sitting | `setting.csv` |

The CSV datasets are located inside:

```text
tinyml/data/
```

JSONL activity data is available under:

```text
tinyml/
```

and additional collected data is available under:

```text
my-express-server/
```

---

# 🧠 CNN Model

The project uses a **Convolutional Neural Network (CNN)** for activity classification from sequential sensor data.

General flow:

```text
Sensor Data
     ↓
Input Window
     ↓
1D Convolution
     ↓
Feature Extraction
     ↓
Pooling
     ↓
Classification
     ↓
Activity
```

The CNN learns patterns in the motion/sensor signals associated with the different activity classes.

---

# 📦 Trained Models

The trained models are located in:

```text
tinyml/data/
```

### Keras

```text
activity_model.keras
```

The TensorFlow/Keras model used during ML development.

### TensorFlow Lite

```text
activity_model.tflite
```

The TensorFlow Lite version of the trained model.

### INT8 TensorFlow Lite

```text
activity_model_int8.tflite
```

The quantized model intended for efficient TinyML deployment.

---

# ⚙️ Scaling and Preprocessing

Python scaler:

```text
scaler.pkl
```

ESP32 scaler parameters:

```text
scaler_params.h
```

The same preprocessing parameters should be used during training and inference.

```text
Raw Sensor Data
       ↓
     Scaler
       ↓
Normalized Data
       ↓
      CNN
```

---

# 🤖 ESP32 Inference

The ESP32 inference process is:

```text
       MPU6050
          │
          ▼
        ESP32
          │
          ▼
   Sensor Samples
          │
          ▼
    Preprocessing
          │
          ▼
     Normalization
          │
          ▼
 TensorFlow Lite Micro
          │
          ▼
          CNN
          │
          ▼
 Activity Prediction
```

The model is intended to run locally on the ESP32.

---

# 🎥 Activity Demonstration Videos

The repository includes demonstration videos for the three activity classes.

All videos are stored in:

```text
vedios/
```

## 🏃 Running

<div align="center">

### Running Activity

<video src="vedios/running.mp4" controls width="700"></video>

**[▶️ Open Running Video](vedios/running.mp4)**

</div>

---

## 🪑 Sitting

<div align="center">

### Sitting Activity

<video src="vedios/sitting.mp4" controls width="700"></video>

**[▶️ Open Sitting Video](vedios/sitting.mp4)**

</div>

---

## 🚶 Walking

<div align="center">

### Walking Activity

<video src="vedios/walking.mp4" controls width="700"></video>

**[▶️ Open Walking Video](vedios/walking.mp4)**

</div>

> **Note:** GitHub README rendering does not consistently support HTML `<video>` playback for repository-relative MP4 files. The **Open Video** links above provide a reliable way to access the videos from the repository.

---

# 🎬 Activity Demonstration

The videos correspond to the activity classes used in the project:

```text
                  ACTIVITY
                     │
        ┌────────────┼────────────┐
        │            │            │
        ▼            ▼            ▼
     WALKING       RUNNING      SITTING
        │            │            │
        ▼            ▼            ▼
 walking.csv    running.csv    setting.csv
        │            │            │
        ▼            ▼            ▼
 walking.mp4    running.mp4    sitting.mp4
```

This gives a visual reference for the physical activities represented in the sensor dataset.

---

# 🚀 Complete Workflow

## Step 1 — Perform an Activity

Perform one of the supported activities:

```text
Walking
Running
Sitting
```

## Step 2 — Collect Sensor Data

The ESP32 collects motion data.

```text
Activity
   ↓
Motion
   ↓
MPU6050
   ↓
ESP32
```

## Step 3 — Send Data to Server

During data collection:

```text
ESP32
  ↓
Node.js / Express
  ↓
CSV / JSONL
```

## Step 4 — Prepare Dataset

The data is organized into activity classes.

## Step 5 — Train the CNN

```text
Dataset
   ↓
Preprocessing
   ↓
Scaling
   ↓
CNN
   ↓
Training
   ↓
Evaluation
```

## Step 6 — Convert the Model

```text
Keras Model
     ↓
TensorFlow Lite
     ↓
INT8 TensorFlow Lite
```

## Step 7 — Prepare ESP32 Model

```text
activity_model_int8.tflite
             ↓
      model_data.cpp
      model_data.h
```

## Step 8 — Deploy to ESP32

```text
ESP32
 ├── Arduino Sketch
 ├── CNN Model
 └── Scaler Parameters
```

## Step 9 — Run Local Inference

```text
Sensor
  ↓
ESP32
  ↓
Preprocessing
  ↓
CNN
  ↓
Prediction
```

---

# ⚡ TinyML / Edge AI

Traditional cloud-based ML:

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
ESP32
   ↓
TinyML Model
   ↓
Prediction
```

The main idea is to bring machine-learning inference closer to the sensor and embedded device.

---

# 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| Python | ML development |
| TensorFlow | Deep learning |
| Keras | CNN development |
| TensorFlow Lite | Model conversion |
| TensorFlow Lite Micro | Embedded inference |
| NumPy | Numerical processing |
| Pandas | Dataset processing |
| Scikit-learn | Preprocessing |
| Joblib | Scaler storage |
| Jupyter Notebook | ML experimentation |
| ESP32 | Embedded AI hardware |
| Arduino | ESP32 development |
| MPU6050 | Motion sensing |
| Node.js | Server runtime |
| Express.js | Data collection |
| CSV | Dataset storage |
| JSONL | Sensor/activity data |
| Git/GitHub | Version control |

---

# 🚀 Installation

## Python Environment

Create a virtual environment:

```bash
python3 -m venv venv
```

Activate it:

```bash
source venv/bin/activate
```

Install dependencies:

```bash
pip install tensorflow
pip install numpy
pip install pandas
pip install scikit-learn
pip install joblib
pip install matplotlib
pip install jupyter
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

Open:

```text
activity_tinyml/activity_tinyml.ino
```

Configure Arduino IDE with:

- ESP32 board support
- Required sensor libraries
- TensorFlow Lite Micro / compatible library

Select the correct ESP32 board and serial port.

Then:

```text
Verify → Upload
```

---

# 📡 Node.js Server Setup

```bash
cd my-express-server
npm install
node server.js
```

---

# 📚 Learning Objectives

This project provides practical experience with:

### Machine Learning

- Dataset creation
- Data preprocessing
- Feature scaling
- CNN architecture
- Model training
- Model evaluation

### TinyML

- TensorFlow Lite
- INT8 quantization
- TensorFlow Lite Micro
- Model conversion
- Embedded inference

### Embedded Systems

- ESP32
- Arduino
- MPU6050
- Sensor data acquisition
- Real-time inference

### Backend

- Node.js
- Express.js
- Data collection
- CSV
- JSONL

---

# 🔮 Future Improvements

- [ ] Add more activity classes
- [ ] Increase dataset size
- [ ] Improve classification accuracy
- [ ] Add confusion matrix
- [ ] Add accuracy/loss graphs
- [ ] Add validation/test visualization
- [ ] Benchmark ESP32 inference latency
- [ ] Measure RAM usage
- [ ] Measure Flash usage
- [ ] Optimize INT8 model
- [ ] Add OLED display
- [ ] Add real-time prediction display
- [ ] Add MQTT support
- [ ] Add Wi-Fi synchronization
- [ ] Compare CNN with LSTM
- [ ] Compare CNN with traditional ML
- [ ] Optimize power consumption

---

# ⚠️ Recommended `.gitignore`

Do not normally commit generated dependencies or temporary files.

```gitignore
node_modules/
venv/
__pycache__/
*.pyc
.ipynb_checkpoints/
.env
```

---

# 👨‍💻 Author

## Rachappa Biradar

Computer Science Student

### Interests

- Machine Learning
- TinyML
- Edge AI
- Embedded Systems
- IoT
- Cybersecurity
- Research

---

# ⭐ Support

If you find this project useful for learning **TinyML, CNN, ESP32, TensorFlow Lite, or Edge AI**, consider giving the repository a ⭐ on GitHub.

---

# 📜 License

Add an appropriate open-source license before distributing the project if you want others to reuse and modify the code.

---

# 📌 Project Summary

```text
┌─────────────────────────────────────────────┐
│       TINYML ACTIVITY RECOGNITION           │
├─────────────────────────────────────────────┤
│                                             │
│  MPU6050                                    │
│      │                                      │
│      ▼                                      │
│  ESP32                                       │
│      │                                      │
│      ▼                                      │
│  Data Collection                            │
│      │                                      │
│      ▼                                      │
│  Node.js / Express                          │
│      │                                      │
│      ▼                                      │
│  Dataset                                    │
│      │                                      │
│      ▼                                      │
│  TensorFlow / Keras                         │
│      │                                      │
│      ▼                                      │
│  CNN Model                                  │
│      │                                      │
│      ▼                                      │
│  TensorFlow Lite                            │
│      │                                      │
│      ▼                                      │
│  INT8 Quantization                          │
│      │                                      │
│      ▼                                      │
│  TensorFlow Lite Micro                      │
│      │                                      │
│      ▼                                      │
│  ESP32                                      │
│      │                                      │
│      ▼                                      │
│  ┌─────────┬─────────┬─────────┐            │
│  │ Walking │ Running │ Sitting │            │
│  └─────────┴─────────┴─────────┘            │
│                                             │
└─────────────────────────────────────────────┘
```

**TinyML brings machine learning from larger computers and cloud systems to resource-constrained edge devices.**
