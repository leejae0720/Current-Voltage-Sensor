import serial
import csv
import os
from datetime import datetime, timedelta

ser = serial.Serial('/dev/ttyACM0', 115200)
now = datetime.now()
csv_directory = "/home/irms/web_app/Power_data/"
csv_filename = "Mirae Power Data.csv"
csv_path = os.path.join(csv_directory, csv_filename)

data_list = []
average_list = []

column_name = ["Date", "Time", "Irms0", "Vrms0", "PF0", "P0", "Irms1", "Vrms1", "PF1", "P1", "Irms2", "Vrms2", "PF2", "P2"]  

ignore_first_data = True
start_time = datetime.now()
			
print('\t'.join(column_name))

while True:
	line = ser.readline().decode().strip()
	data = line.split('\t')
	now = datetime.now()
	data.insert(0, now.strftime("%Y-%m-%d"))
	data.insert(1, now.strftime("%H:%M"))

	if ignore_first_data:
		ignore_first_data = False
	else:
		data_list.append(data)
		print(data)
		
	if now - start_time >= timedelta(minutes=5):
		if len(data_list) > 0:
			num_columns = len(data_list[0])
			avg_data = [start_time.strftime("%Y-%m-%d"), now.strftime("%H:%M")]

			for i in range(2, num_columns):
				sum_value = sum([float(item[i]) for item in data_list])
				average_value = round(sum_value / len(data_list), 2)
				avg_data.append(average_value)

			average_list.append(avg_data)
			print(average_list)
			
			if os.path.exists(csv_path):
				with open(csv_path, mode='a', newline='') as file:
					writer = csv.writer(file)
					writer.writerow(average_list[-1])
			else:
				with open(csv_path, mode='a', newline='') as file:
					writer = csv.writer(file)
					writer.writerow(column_name)
					writer.writerow(average_list[-1])

		start_time = now
		data_list.clear()
		average_list.clear()
	
	if now.day != start_time.day:
		start_time = now
		csv_filename = "Mirae Power Data.csv"
		csv_path = os.path.join(csv_directory, csv_filename)
