# -*- coding:utf-8 -*-

import matplotlib.pyplot as plt
from pylab import *
# x = linspace(0, 5, 10)
# y = x ** 2

fig = plt.figure()
axes = fig.add_axes([0, 0, 1, 1]) # left, bottom, width, height (range 0 to 1)

x=[]
y=[]
with open("../line.origin.db") as file:
    for line in file:
        newline = line.split(' ')

        x = newline[:-1:2]
        y = newline[1::2]
        axes.scatter(x,y)
        # axes.plot(x, y, 'r')
        axes.set_xlabel('x')
        axes.set_ylabel('y')
        axes.set_title('TTTTTTitle')

        plt.show()
        break
