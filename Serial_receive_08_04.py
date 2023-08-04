import serial
import csv
from datetime import datetime, timedelta

ser = serial.Serial('/dev/ttyACM0', 115200)
now = datetime.now()
csv_filename = now.strftime("%Y%m%d_%H%M") + '.csv'
data_list = []
average_list = []

ignore_first_data = True
start_time = datetime.now()

while True:
	line = ser.readline().decode().strip()
	data = line.split('\t')
	now = datetime.now()
	data.insert(0, now.strftime("%H:%M"))
	
	if ignore_first_data:
		ignore_first_data = False
	else:
		data_list.append(data)
		print(data)
    
	if now - start_time >= timedelta(minutes = 5):
		if len(data_list) > 0:
			avg_data = []
			for i in range(1, len(data_list[0])):
				sum_value = [float(item[i]) for item in data_list]
				average_value = sum_value / len(data_list)
				avg_data.append(avg_value)
				print(f"Average for Column {i}: {avg_value}")
				
			avg_data.insert(0, now.strftime("%H:%M"))
			average_list.append(avg_data)
			
		start_time = now
		data_list.clear()

	with open(csv_filename, mode='w', newline='') as file:
		writer = csv.writer(file)
		writer.writerows(data_list)
