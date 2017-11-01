#!/usr/bin/python

import matplotlib.pyplot as plt
import math


if __name__ == '__main__':
    xAxis = []
    yAxis = []
    for i in range(-100000, 100000):
        x = i * 0.0001
        y = math.sin(x)
        xAxis.append(x)
        yAxis.append(y)

    plt.plot(xAxis, yAxis, label='sin')
    plt.legend(loc='upper left')
    plt.show()
