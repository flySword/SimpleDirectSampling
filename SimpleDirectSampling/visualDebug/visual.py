# to plot the intermediate result file for debug

import numpy as np
import matplotlib.pyplot as plt
import math
# for rectangle draw
import matplotlib.patches as patches
import sys

if len(sys.argv) > 1:
    filePath = sys.argv[1]+"/"
else:
    filePath = ""


sgFilename = filePath + "sg.txt"

#  a txt file containing a line, with "x y attr" of current point
sgPntFilename = filePath + "sgPnt.txt"

tiPatternFilename = filePath + "pattern.txt"

# WARN！ the first line is the coordinate of TI: "x y attr"
# the other lines are relative coordinate: "deltaY deltaX attr"  !!!
# x y is not corresponded
tiFilename = filePath + "ti.txt"

orientHdFilename = filePath + "orientHd.txt"
#orientTiFilename = filePath + "orientTI.txt"
orientPaternFilename = filePath + "orientPatern.txt"


plt.subplot(1,2,1)


# plot the txt matrix
data = np.loadtxt(sgFilename)
sizeY = data.shape[0]
sizeX = data.shape[1]
x = np.linspace(0, sizeX-1, sizeX)
y = np.linspace(0, sizeY-1, sizeY)
[X, Y] = np.meshgrid(x, y)
#plt.scatter(X, Y, abs(data))
plt.contour(X, Y, data)

# plot current sg point
sgPntData = np.loadtxt(sgPntFilename)
sgX = sgPntData[0];
sgY = sgPntData[1];
sgRadius = sgPntData[2];
plt.scatter(sgX,sgY,c='r')


# plot the rect
rectMinX = sgX - sgRadius
rectMinY = sgY - sgRadius
rectMaxX = sgX + sgRadius
rectMaxY = sgY + sgRadius
if sgX - sgRadius < 0:
	rectMinX = 0
if sgY - sgRadius < 0:
	rectMinY = 0
if sgX + sgRadius >= sizeX:
	rectMaxX = sizeX-1
if sgY + sgRadius >= sizeY:
	rectMaxY = sizeY-1

rect = patches.Rectangle((rectMinX,rectMinY), (rectMaxX-rectMinX),
                         (rectMaxY-rectMinY),fill=None,ls = 'dashed', edgecolor = 'k')
plt.gca().add_patch(rect)
	

# plot the TI
plt.subplot(1,2,2)
ti = np.loadtxt(tiFilename)
tiSizeY = data.shape[0]
tiSizeX = data.shape[1]
tix = np.linspace(0, tiSizeX-1, tiSizeX)
tiy = np.linspace(0, tiSizeY-1, tiSizeY)
[tiX, tiY] = np.meshgrid(tix, tiy)
plt.contour(tiX,tiY,ti)
#plt.scatter(tiX, tiY, 80/abs(ti))


# plot the TI pattern
ptnData = np.loadtxt(tiPatternFilename) # ptnData[0] is the current ti point
currentTiX = ptnData[0][0]
currentTiY = ptnData[0][1]
plt.scatter(currentTiX, currentTiY,c='r')
ptnTi = np.ones((np.shape(ptnData)[0]-1,np.shape(ptnData)[1]))
ptnTi[:,0] = ptnData[1:,1] + ptnData[0][0]
ptnTi[:,1] = ptnData[1:,0] + ptnData[0][1]
ptnTi[:,2] = ptnData[1:,2]
plt.scatter(ptnTi[:,0],ptnTi[:,1], c='y')



plt.show()
