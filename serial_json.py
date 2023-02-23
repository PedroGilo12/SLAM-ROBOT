import numpy as np
import serial
import json
import time
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

ser = serial.Serial('/dev/ttyACM0')  # open serial port

fig, ax1 = plt.subplots()

buffer_ax = []
buffer_ay = []
buffer_az = []

list_x = []
list_y = []

velocidade_y = 0

maxt = 1000

def media_m(buffer, var):
    if len(buffer) >= 100:
        buffer.remove(buffer[0])
    
    buffer.append(var)
    media = sum(buffer)/len(buffer)

    return media


while True:

    for i in range(maxt):
        line = ser.readline()
        line = line.decode()
        if line != 0:
            parameters = line

        try:
            p = json.loads(parameters)
            i = i + 1
            flag = 1
        except:
            flag = 0
            pass

        if flag == 1:
            ax = int(p["x"])
            ay = int(p["y"])
            az = int(p["z"])

            axm = media_m(buffer_ax, ax)
            aym = media_m(buffer_ay, ay)
            azm = media_m(buffer_az, az)

            ax_real = round((ax - axm)*0.00977, 3)
            ay_real = round((ay - aym)*0.00977, 3)
            az_real = round((az - azm)*0.00977, 3)

            velocidade_y = velocidade_y + (ay_real*0.1)

            print(f"acelerações: ay = {ay_real} m/s²; vy = {velocidade_y} m/s")

            list_x.append(velocidade_y)
            list_y.append(i)
            time.sleep(0.1)

    ax1.plot(list_y, list_x)
    plt.show()
    quit()