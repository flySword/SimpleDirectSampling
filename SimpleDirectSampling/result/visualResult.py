import numpy as np
import matplotlib.pyplot as plt
import sys



if len(sys.argv) > 1:
    filePath = sys.argv[1]+"/"
else:
    filePath = ""

sgFilename = filePath + "sg.txt"
hdFilename = filePath + "hd.txt"
tiFilename = filePath + "ti.txt"


# plot the sg
plt.subplot(1,2,1)
data = np.loadtxt(sgFilename)
data[data<-9999] = 0 # unsimulated point need to be converted!!
plt.imshow(data ,cmap = plt.cm.gray)

# plot the hd
hdData = np.loadtxt(hdFilename)
plt.scatter(hdData[:, 0], hdData[:, 1], c='r', marker='.')


# plot the ti
plt.subplot(1,2,2)
data = np.loadtxt(tiFilename)
plt.imshow(data ,cmap = plt.cm.gray)



plt.show()