import serial
import datetime

now = datetime.datetime.now()
timestamp = now.strftime("%Y%m%d%H%M%S")


file_name = f"{timestamp}.txt"
file = open(file_name, "w")

ser = serial.Serial('/dev/ttyUSB0',115200)

while True:
	data = ser.readline().decode().strip()
	
	file.write(data + "\n")
	file.flush()
	
	print(data)
	
	if data == 'q':
		break
		
file.close()
