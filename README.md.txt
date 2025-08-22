# IoT Crop Predictor 

An IoT + ML project that reads soil moisture & temperature from Arduino and predicts a crop in real time using a Python model.

## Run
1) `pip install -r requirements.txt`
2) Upload `iot_el.ino` to your Arduino.
3) Set your serial port in `pred.py` (e.g., `COM10` on Windows).
4) `python pred.py`

## Files
- `iot_el.ino` – Arduino sketch (sensor read + serial print)
- `pred.py` – Reads serial and predicts crop
- `.vscode/` – C/C++ debug & IntelliSense configs
