import serial
import pickle

# Load the model
with open("crop_model.pkl", "rb") as f:
    model, label_encoder = pickle.load(f)

# Open serial port (replace 'COM3' with your port, or '/dev/ttyUSB0' on Linux)
ser = serial.Serial('COM10', 9600, timeout=2)

while True:
    line = ser.readline().decode(errors='ignore').strip()
    if "Moisture:" in line and "Temperature:" in line:
        try:
            parts = line.split(',')
            moisture = float(parts[0].split(':')[1])
            temp = float(parts[1].split(':')[1])
            
            input_data = [[moisture, temp]]
            prediction = model.predict(input_data)[0]
            crop = label_encoder.inverse_transform([prediction])[0]
            print(f"Predicted Crop: {crop}")
        except:
            print("Parsing error")