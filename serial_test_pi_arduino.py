import serial

ser = serial.Serial('/dev/ttyACM0', 115200)

try:
    while True:
     
        data = ser.readline().decode('utf-8').strip()
        print(data)

except KeyboardInterrupt:
    
    ser.close()
    print("end")
