import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os
import csv

path = "/home/massimiliano/Scrivania/results/The_power_of_Gamma/"
#path = "/home/massimiliano/Scrivania/single_simulation/"

directory_hour = "Gamma"

file_name = "response_time.csv"
file_name_2 = "game_result.csv"

gamma = np.zeros((4,6))


for i in range(6):
    reserved = 0;
    demand = 0;
    spot = 0;
    rejected = 0;
    file = csv.reader(open(path+ directory_hour+str(i)+"/"+file_name))
    file2 = csv.reader(open(path+ directory_hour+str(i)+"/"+file_name_2))
    lines = list(file)
    lines2 = list(file2)
    spot = int(lines[1][0])
    reserved = 5
    demand = int(lines[1][1]) - spot - reserved
    rejected = float(lines2[1][2])
    gamma[0,i] = reserved
    gamma[1,i] = demand
    gamma[2,i] = spot
    gamma[3,i] = rejected


reserved = gamma[0][:]
demand = gamma[1][:]
spot = gamma[2][:]
rejected = gamma[3][:]

#x = ['0', '1', '2', '3', '4', '5']
x =np.arange(6)
width = 0.3

plt.ylabel('Number of Virtual Machines')
plt.xlabel('Gamma')
plt.title('The effect of Gamma')
#plt.legend([gamma0, gamma1, gamma2, gamma3, gamma4, gamma5],['Gamma 0', 'Gamma 1', 'Gamma 2', 'Gamma 3', 'Gamma 4', 'Gamma 5'])

plt.bar(x,reserved,width ,label="Reserved VMs")
plt.bar(x+width,demand,width ,label="On demand VMs")
plt.bar(x-width,spot,width ,label="On Spot VMs")
#plt.plot(x,gamma3, label="Gamma 3")

plt.legend(loc="best", prop={'size': 6})
plt.show()

plt.plot(x,rejected, label = "rejected requests")
plt.ylabel('Dropped Requests [req/s]')
plt.xlabel('Gamma')
plt.title('The effect of Gamma')
plt.legend(loc="best", prop={'size': 6})
plt.show()
