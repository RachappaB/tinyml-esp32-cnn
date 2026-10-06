# TinyML Activity Recognition with ESP32

A complete **TinyML / Edge AI activity-recognition project** that combines:

- Machine learning model development and training
- Sensor-data collection and preprocessing
- TensorFlow / Keras
- TensorFlow Lite and INT8 quantization
- ESP32 embedded inference
- Arduino deployment
- A Node.js / Express server for collecting and storing sensor data

The repository is organized into three main parts:

```text
tinyml-esp32-cnn/
├── tinyml/                 # Machine learning, datasets and trained models
├── activity_tinyml/        # ESP32 / Arduino deployment
├── my-express-server/      # Node.js / Express data collection server
└── Readme.md
```

## Project Architecture

```text
                  SENSOR DATA
                       │
                       ▼
              ┌─────────────────┐
              │   ESP32 + IMU   │
              └────────┬────────┘
                       │
             Data Collection
                       │
                       ▼
              ┌─────────────────┐
              │ Express Server  │
              │   Node.js       │
              └────────┬────────┘
                       │
                 Training Data
                       │
                       ▼
              ┌─────────────────┐
              │   Python /      │
              │ TensorFlow CNN  │
              └────────┬────────┘
                       │
                Model Training
                       │
                       ▼
              ┌─────────────────┐
              │ TensorFlow Lite │
              │    INT8 Model   │
              └────────┬────────┘
                       │
                Model Conversion
                       │
                       ▼
              ┌─────────────────┐
              │      ESP32      │
              │ TFLite Micro    │
              └────────┬────────┘
                       │
                       ▼
                Activity Result
```

---

# 📁 Repository Structure

## 1. `tinyml/` — Machine Learning

This directory contains the machine-learning workflow, datasets, trained models and preprocessing files.

```text
tinyml/
├── tinyml.ipynb
├── walking.jsonl
├── running.jsonl
├── setting.jsonl
│
└── data/
    ├── walking.csv
    ├── running.csv
    ├── setting.csv
    ├── y.csv
    │
    ├── scaler.pkl
    ├── scaler_params.h
    │
    ├── activity_model.keras
    ├── activity_model.tflite
    ├── activity_model_int8.tflite
    │
    ├── model_data.cc
    └── model_data.h
```

### Main notebook

```text
tinyml/tinyml.ipynb
```

The notebook is the main workspace for the machine-learning process.

### Activity datasets

The repository currently contains data for:

- Walking
- Running
- Setting

CSV datasets are available in:

```text
tinyml/data/
```

JSONL data files are available directly under:

```text
tinyml/
```

---

# 🧠 Machine Learning Pipeline

The general ML workflow is:

```text
Raw Sensor Data
      ↓
Data Preparation
      ↓
Preprocessing
      ↓
Feature Scaling
      ↓
Training Dataset
      ↓
CNN Model
      ↓
Model Training
      ↓
Model Evaluation
      ↓
TensorFlow Lite
      ↓
INT8 Quantization
      ↓
ESP32 Deployment
```

The project uses a CNN model for activity classification from sequential sensor data.

---

# 📦 Trained Models

The trained models are stored under:

```text
tinyml/data/
```

### Keras model

```text
activity_model.keras
```

The Keras model is the main TensorFlow/Keras model used during ML development.

### TensorFlow Lite model

```text
activity_model.tflite
```

This is the TensorFlow Lite version of the trained model.

### INT8 TensorFlow Lite model

```text
activity_model_int8.tflite
```

The INT8 model is intended for efficient embedded inference and TinyML deployment.

Quantization can reduce the computational and memory requirements of a neural network, making the model more suitable for microcontrollers.

---

# 📊 Data Scaling

The project contains:

```text
scaler.pkl
```

for the Python-side scaler.

The corresponding parameters are also exported as:

```text
scaler_params.h
```

The C/C++ header allows the ESP32 application to reproduce the preprocessing used during model training.

This is important because the model should receive data processed in the same way during training and inference.

---

# 2. `activity_tinyml/` — ESP32 / Arduino

This directory contains the embedded implementation.

```text
activity_tinyml/
├── activity_tinyml.ino
├── model_data.cpp
├── model_data.h
└── scaler_params.h
```

### Arduino sketch

```text
activity_tinyml.ino
```

This is the main Arduino/ESP32 application.

### Embedded model

```text
model_data.cpp
model_data.h
```

These files contain the converted model data required by the embedded application.

### Embedded preprocessing

```text
scaler_params.h
```

Contains the preprocessing/scaling parameters required by the ESP32 inference pipeline.

---

# 3. `my-express-server/` — Data Collection Server

The `my-express-server/` directory contains a Node.js / Express server used as part of the sensor-data workflow.

```text
my-express-server/
├── server.js
├── package.json
├── package-lock.json
│
├── mpu_data.csv
├── ruuning.jsonl
├── sitting.jsonl
└── walking.jsonl
```

The server-side directory contains collected sensor/activity data and the Node.js application.

> `node_modules/` is also present locally, but it should normally **not be committed to Git** because dependencies can be recreated using `npm install`.

---

# 🚀 Getting Started

## Prerequisites

### Machine Learning

Install:

- Python 3
- Jupyter Notebook
- TensorFlow
- NumPy
- Pandas
- Scikit-learn
- Joblib
- Matplotlib

Example:

```bash
python3 -m venv venv
source venv/bin/activate

pip install tensorflow numpy pandas scikit-learn joblib matplotlib jupyter
```

Start Jupyter:

```bash
jupyter notebook
```

Then open:

```text
tinyml/tinyml.ipynb
```

---

# 🖥️ Run the Express Server

Go to the server directory:

```bash
cd my-express-server
```

Install dependencies:

```bash
npm install
```

Start the server using the command defined by the project configuration, or run the server entry point directly if appropriate:

```bash
node server.js
```

The server can be used as part of the sensor-data collection workflow.

---

# 🔧 ESP32 Setup

Open:

```text
activity_tinyml/activity_tinyml.ino
```

Use the Arduino IDE with ESP32 board support installed.

Depending on the implementation, install/configure the required:

- ESP32 board package
- TensorFlow Lite Micro libraries
- Sensor libraries

Select the correct ESP32 board and serial port, compile the project, and upload it to the device.

---

# 🤖 TinyML Inference

The embedded inference process follows this general flow:

```text
Sensor
  ↓
ESP32
  ↓
Collect Sensor Samples
  ↓
Preprocess / Scale
  ↓
Prepare Tensor
  ↓
TensorFlow Lite Micro
  ↓
CNN Inference
  ↓
Activity Classification
```

The purpose of TinyML is to move inference from a larger computer or cloud server directly onto a resource-constrained embedded device.

---

# 🌐 Server-Based Data Collection vs Edge Inference

This repository demonstrates both sides of an ML system.

### Data collection

```text
ESP32
  ↓
Node.js / Express
  ↓
Collected Dataset
  ↓
Training
```

### Edge inference

```text
Sensor
  ↓
ESP32
  ↓
TFLite Micro
  ↓
CNN
  ↓
Prediction
```

This separation makes it possible to use a server/computer during development while deploying the final trained model directly to the ESP32.

---

# 🧪 Activity Classes

The current repository contains activity data for:

| Activity | Data |
|---|---|
| Walking | `walking.csv`, `walking.jsonl` |
| Running | `running.csv`, `ruuning.jsonl` |
| Setting | `setting.csv`, `setting.jsonl` |

> The filename `ruuning.jsonl` is kept as it currently exists in the repository.

---

# 🛠️ Technologies

| Technology | Purpose |
|---|---|
| Python | ML development |
| TensorFlow | Deep learning |
| Keras | CNN model development |
| TensorFlow Lite | Model conversion |
| TensorFlow Lite Micro | Embedded inference |
| NumPy | Numerical processing |
| Pandas | Dataset processing |
| Scikit-learn | Data preprocessing |
| Joblib | Saving/loading scaler |
| Jupyter Notebook | ML experimentation |
| ESP32 | Edge AI hardware |
| Arduino | Embedded development |
| Node.js | Server runtime |
| Express.js | Data collection server |

---

# 🎯 Project Objectives

This project is intended to demonstrate the complete TinyML development lifecycle:

1. Collect sensor data
2. Store and organize the data
3. Prepare the dataset
4. Preprocess sensor measurements
5. Train a CNN
6. Evaluate the trained model
7. Convert the model to TensorFlow Lite
8. Quantize the model
9. Convert the model into embedded C/C++ data
10. Deploy the model to ESP32
11. Perform inference on the edge

---

# 📚 Learning Topics

This project can be used to learn:

- Machine Learning
- Deep Learning
- Convolutional Neural Networks
- Time-series sensor data
- Data preprocessing
- Feature scaling
- Model training
- Model conversion
- INT8 quantization
- TensorFlow Lite
- TensorFlow Lite Micro
- TinyML
- Edge AI
- ESP32
- Arduino
- Node.js
- Express.js
- IoT data collection

---

# 🔮 Future Improvements

Possible improvements include:

- [ ] Add more activity classes
- [ ] Increase the size of the dataset
- [ ] Improve model accuracy
- [ ] Add training/validation graphs
- [ ] Add confusion matrix
- [ ] Benchmark ESP32 inference latency
- [ ] Measure RAM usage
- [ ] Measure Flash usage
- [ ] Optimize the INT8 model
- [ ] Add OLED display
- [ ] Add real-time prediction output
- [ ] Add MQTT support
- [ ] Add Wi-Fi data synchronization
- [ ] Compare CNN with LSTM
- [ ] Compare CNN with traditional machine-learning algorithms
- [ ] Optimize power consumption

---

# ⚠️ Git Recommendation

Do **not** normally commit the following generated/local directories:

```text
node_modules/
.ipynb_checkpoints/
```

A suitable `.gitignore` can include:

```gitignore
node_modules/
__pycache__/
.ipynb_checkpoints/
*.pyc
.env
venv/
```

---

# 👨‍💻 Author

**Rachappa Biradar**

Computer Science Student

Interests:

- TinyML
- Machine Learning
- Edge AI
- Embedded Systems
- IoT
- Cybersecurity
- Research

---

# ⭐ Project

If you find this project useful for learning TinyML, ESP32, or Edge AI, consider giving the repository a star.

---

## 📜 License

Add the project's preferred open-source license before publishing if you intend others to reuse the code.
