import itertools
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import serial
import json

ser = serial.Serial('/dev/ttyUSB1', 57600)  # open serial port


def data_gen():
    t = 0

    while True:
        line = ser.readline()
        line = line.decode()
        if line != 0:
            parameters = line
            print(line)

        try:
            p = json.loads(parameters)
            flag = 1
        except:
            flag = 0
            pass

        if flag == 1:
            a = p["a"]
            r = p["d"]

            t = int(r)*np.cos(np.radians(int(a)))
            j = int(r)*np.sin(np.radians(int(a)))
            
            yield t, j

def init():
    ax.set_ylim(-600, 600)
    ax.set_xlim(-600, 600)
    del xdata[:]
    del ydata[:]
    line.set_data(xdata, ydata)
    return line,

fig, ax = plt.subplots()
line, = ax.plot([], [], lw=0, marker = "o")
ax.grid()

ax.plot(0, 0, marker = "s", color = "red")
xdata, ydata = [], []


def run(data):
    # update the data
    t, y = data
    print(data)
    xdata.append(t)
    ydata.append(y)
    xmin, xmax = ax.get_xlim()

    if t >= xmax:
        ax.set_xlim(xmin, 2*xmax)
        ax.figure.canvas.draw()
    line.set_data(xdata, ydata)

    return line,

# Only save last 100 frames, but run forever
ani = animation.FuncAnimation(fig, run, data_gen, interval=100, init_func=init,
                              save_count=100)
plt.show()
