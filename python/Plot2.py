#!/usr/bin/python

import matplotlib.pyplot as plt
import math


# https://matplotlib.org/users/pyplot_tutorial.html
# https://matplotlib.org/api/_as_gen/matplotlib.pyplot.plot.html

if __name__ == '__main__':
    xAxis = []
    y1Axis = []
    y2Axis = []
    y3Axis = []
    for i in range(-100000, 100000):
        x = i * 0.0001
        y1 = math.sin(x)
        y2 = 0.1 * math.sin(x * 50)
        y3 = y1 + y2
        xAxis.append(x)
        y1Axis.append(y1)
        y2Axis.append(y2)
        y3Axis.append(y3)

    plt.plot(xAxis, y1Axis, label='sin1')
    plt.plot(xAxis, y2Axis, label='sin2')
    plt.plot(xAxis, y3Axis, label='sin3')
    plt.legend(loc='upper left')
    plt.show()
