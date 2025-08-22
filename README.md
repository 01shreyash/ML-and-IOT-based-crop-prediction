# IoT Crop Predictor 🌱

An IoT + ML project that reads soil moisture & temperature from Arduino and predicts a crop in real time using a Python model.

## 🚀 Features
- Arduino collects soil moisture & temperature via sensors.
- Python (`pred.py`) reads serial data and predicts crop using ML model.
- Model stored in `models/` folder.

## 📂 Structure
- `iot_el.ino` → Arduino sketch
- `pred.py` → Python script for predictions
- `models/` → Stores ML models (`.pkl`)
- `data/` → Optional dataset folder
- `.vscode/` → Debug & IntelliSense configs
- `requirements.txt` → Dependencies

## 🛠️ Setup
1. Clone the repo:
   ```bash
   git clone https://github.com/01shreyash/IoT-Crop-Predictor.git
   cd IoT-Crop-Predictor
   ```
2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Upload `iot_el.ino` to Arduino.
4. Run prediction script:
   ```bash
   python pred.py
   ```
