import serial.tools.list_ports
import serial
from flask import Flask, jsonify
from flask_cors import CORS
import threading
import time

app = Flask(__name__)

# Enable CORS for all routes
CORS(app)

# Define serial port and baud rate
serial_port = 'COM5'  # Adjust the port as needed
baud_rate = 9600

# Global variable to store moisture value
moisture_level = 0

def open_serial_port():
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=10)
        if not ser.isOpen():
            ser.open()
        return ser
    except SerialException as e:
        print(f"Error opening serial port '{serial_port}': {e}")
        return None

ser = open_serial_port()

if ser:
    print(f"Serial port '{serial_port}' opened successfully.")
else:
    print(f"Failed to open serial port '{serial_port}'.")
    exit(1)

@app.route('/pump/on', methods=['POST'])
def pump_on():
    ser.write(b'1')  # Send command to Arduino to turn the pump on
    return 'Pump turned on'

@app.route('/pump/off', methods=['POST'])
def pump_off():
    ser.write(b'0')  # Send command to Arduino to turn the pump off
    return 'Pump turned off'

# Function to update moisture value from serial
def update_moisture_value():
    global moisture_level
    while True:
        moisture_data = ser.readline().decode('utf-8').strip()
        print("Updated Moisture Value --> ", moisture_data)
        try:
            moisture_level = int(moisture_data)
        except ValueError:
            print("Error: Invalid moisture data received from serial.")
        time.sleep(0.5)  # Wait for 1 second before reading again

# Start a thread to continuously update moisture value
update_thread = threading.Thread(target=update_moisture_value)
update_thread.daemon = True
update_thread.start()

# Route to fetch moisture value
@app.route('/moisture', methods=['POST'])
def get_moisture():
    print("Moisture Level : ", moisture_level)
    return jsonify({'moisture_level': moisture_level})  


# @app.route('/moisture', methods=['POST'])
# def get_moisture():
#     # Read moisture data from Arduino
#     moisture_data = ser.readline().decode().strip()
    
#     # Convert string to integer
#     # moisture_level = int(moisture_data)
#     moisture_level = moisture_data

#     # Process moisture data as needed
#     # For example, you can return it as JSON
#     print("Moisture Level : ",moisture_level)
#     return jsonify({'moisture_level': moisture_level})  



if __name__ == '__main__':
    app.run()