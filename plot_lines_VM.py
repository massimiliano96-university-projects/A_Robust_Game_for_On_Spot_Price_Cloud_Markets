import matplotlib
import matplotlib.pyplot as plt
import numpy as np


x = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23']
gamma0 = [2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 3, 3, 4, 3, 3, 3, 3, 3, 3]
gamma1 = [4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 6, 5, 5, 5, 5, 5, 5]
gamma2 = [5, 5, 4, 4, 4, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, 6, 6, 7, 7, 6, 7, 6, 7, 7]
gamma3 = [6, 6, 5, 5, 4, 4, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 9, 8, 7, 8, 8, 8, 8]
gamma4 = [7, 7, 5, 5, 5, 4, 4, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 10, 9, 8, 9, 9, 9, 9]
gamma5 = [7, 7, 6, 6, 5, 5, 5, 5, 5, 5, 6, 7, 7, 8, 7, 8, 9, 11, 10, 9, 10, 9, 10, 10]

plt.ylabel('Number of VMs')
plt.xlabel('Hours of the day')
plt.title('Daily number of VMs')
#plt.legend(['Gamma 0', 'Gamma 1', 'Gamma 2', 'Gamma 3', 'Gamma 4', 'Gamma 5'])

plt.plot(x,gamma0, label="Gamma 0")
plt.plot(x,gamma1, label="Gamma 1")
plt.plot(x,gamma2, label="Gamma 2")
plt.plot(x,gamma3, label="Gamma 3")
plt.plot(x,gamma4, label="Gamma 4")
plt.plot(x,gamma5, label="Gamma 5")
plt.legend(loc="best", prop={'size': 6})
plt.show()
