# visualization of the result
from mpl_toolkits.mplot3d import Axes3D # used for 3d plot
import matplotlib.pyplot as plt
import numpy as np
import sys


if len(sys.argv) > 1:
	filePath = sys.argv[1]+"/"
# sgX = sys.argv[2]
# sgY = sys.argv[3]
else:
	filePath = ""

sgFilename = filePath + "sg.txt"
tiFilename = filePath + "ti.txt"
hdFilename = filePath + "hd.txt"
orientHdFilename = filePath + "orientHd.txt"
orientTiFilename = filePath + "orientTI.txt"

# plot the sg

fig = plt.figure()
ax = fig.add_subplot(121, projection='3d')

#plt.subplot(1, 2, 1, projection = '3d')
data = np.loadtxt(sgFilename)
sizeY = data.shape[0]
sizeX = data.shape[1]
x = np.linspace(0, sizeX - 1, sizeX)
y = np.linspace(0, sizeY - 1, sizeY)
[X, Y] = np.meshgrid(x, y)

ax.scatter(X[:], Y[:], data[:])
# plot the hd
hdData = np.loadtxt(hdFilename)
ax.scatter(hdData[:, 0], hdData[:, 1], hdData[:, 2], c='r', marker='*')


# plot the TI
ax = fig.add_subplot(122, projection='3d')
ti = np.loadtxt(tiFilename)
tiSizeY = data.shape[0]
tiSizeX = data.shape[1]
tix = np.linspace(0, tiSizeX - 1, tiSizeX)
tiy = np.linspace(0, tiSizeY - 1, tiSizeY)
[tiX, tiY] = np.meshgrid(tix, tiy)
ax.scatter(tiX, tiY, ti)

new = np.concatenate((tiX.reshape(-1,1), tiY.reshape(-1,1), ti.reshape(-1,1)))
ax.scatter(hdData[:, 0], hdData[:, 1], hdData[:, 2], c='r', marker='*')
np.savetxt('result.txt',new.reshape(3,-1).T,fmt='%d')



plt.show()
