import numpy as np
import matplotlib.pyplot as plt
NX = 1000; NY = 1000

Z1 = []
Z2 = []
tmp = []

f = open("init.txt")
for i in range(NX):
    tmp = f.readline().split()
    tmp = map(float, tmp)
    Z1.append(tmp)
f.close()
f = open("result.txt")
for i in range(NX):
    tmp = f.readline().split()
    tmp = map(float, tmp)
    Z2.append(tmp)
f.close()

xlist = np.linspace(0, 1, NX);
ylist = np.linspace(0, 1, NY);
X, Y = np.meshgrid(xlist, ylist);

fg1 = plt.contourf(X, Y, Z1)
plt.show()

