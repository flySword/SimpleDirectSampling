import numpy as np

sg = np.loadtxt('sg.txt')

size = 50

x = np.linspace(0, size - 1, size)
y = np.linspace(0, size - 1, size)
[X, Y] = np.meshgrid(x, y)

new = np.concatenate((X.reshape(-1,1), Y.reshape(-1,1), sg.reshape(-1,1)))
new = new.reshape(3,-1)
np.savetxt('aa.txt', new.T, fmt='%d')