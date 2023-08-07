import serial
import csv
from datetime import datetime, timedelta

ser = serial.Serial('/dev/ttyACM0', 115200)
now = datetime.now()
csv_filename = now.strftime("%Y%m%d_%H%M") + '.csv'
data_list = []
average_list = []

column_name = ["Time", "Irms0", "Vrms0", "PF0", "P0", "Irms1", "Vrms1", "PF1", "P1", "Irms2", "Vrms2", "PF2", "P2"]  

ignore_first_data = True
start_time = datetime.now()
			
print('\t'.join(column_name))

with open(csv_filename, mode='a', newline='') as file:
	writer = csv.writer(file)
	writer.writerow(column_name)

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
		
		#with open(csv_filename, mode='a', newline='') as file:
		#	writer = csv.writer(file)
		#	writer.writerows(data_list)

	if now - start_time >= timedelta(minutes=1):
		if len(data_list) > 0:
			num_columns = len(data_list[0])
			avg_data = ['Average']

			for i in range(1, num_columns):
				sum_value = sum([float(item[i]) for item in data_list])
				average_value = sum_value / len(data_list)
				avg_data.append(average_value)

			average_list.append(avg_data)
			print(average_list);
			
			with open(csv_filename, mode='a', newline='') as file:
				writer = csv.writer(file)
				writer.writerow(average_list)
				
			avg_line = '\t'.join([f"{average_value}" if i > 0 else '' for i, average_value in enumerate(avg_data)])
			print(avg_line)

			print('\t'.join(map(str, avg_data)))

		start_time = now
		data_list.clear()
