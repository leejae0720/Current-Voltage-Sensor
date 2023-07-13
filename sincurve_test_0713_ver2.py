import serial
import time
import datetime
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

ser = serial.Serial('/dev/ttyUSB0', 115200)

directory = '/home/irms/'

data1 = []
data2 = []
data3 = []
data4 = []

while True:
    line = ser.readline().decode().strip()

    data = line.split('\t')
    if len(data) < 4:
        continue

    data1.append(float(data[0]))
    data2.append(float(data[1]))
    data3.append(float(data[2]))
    data4.append(float(data[3]))

    print(line)

    if len(data1) >= 10 and len(data2) >= 10 and len(data3) >= 10 and len(data4) >= 10:
        current_time = datetime.datetime.now()

        file_name = current_time.strftime("%Y%m%d%H%M%S") + ".txt"

        with open(directory + file_name, 'w') as file:
            for item1, item2, item3, item4 in zip(data1, data2, data3, data4):
                file.write("{}\t{}\n".format(item1, item2, item3, item4))

        x = np.arange(len(data1))

        def sin_func(x, a, b, c, d):
            return a * np.sin(b * x + c) + d

        popt1, pcov1 = curve_fit(sin_func, x, data1)
        popt2, pcov2 = curve_fit(sin_func, x, data2)
        popt3, pcov3 = curve_fit(sin_func, x, data3)
        popt4, pcov4 = curve_fit(sin_func, x, data4)

        multiplied_data_1 = np.multiply(data1, data2)
        multiplied_data_2 = np.multiply(data3, data4)

        #plt.plot(x, data1, 'b-', label='Data 1')
        plt.plot(x, sin_func(x, *popt1), 'r', label='Current_Channel_1')

        #plt.plot(x, data2, 'g-', label='Data 2')
        plt.plot(x, sin_func(x, *popt2), 'm', label='Voltage_channel_1')

        #plt.plot(x, data1, 'b-', label='Data 1')
        plt.plot(x, sin_func(x, *popt3), 'r', label='Current_Channel_2')

        #plt.plot(x, data2, 'g-', label='Data 2')
        plt.plot(x, sin_func(x, *popt4), 'm', label='Voltage_channel_2')
        
        plt.plot(x, multiplied_data_1, 'c-', label='Power_channel_1')
        plt.plot(x, multiplied_data_2, 'c-', label='Power_channel_2')

        plt.legend()
        plt.xlabel('Time')
        plt.ylabel('Data')
        plt.title('Current * Voltage = Power')
        plt.show()

        break
