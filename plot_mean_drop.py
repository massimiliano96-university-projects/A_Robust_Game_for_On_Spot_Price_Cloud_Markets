import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os
import csv

path = "/home/massimiliano/Scrivania/instances_tot_relaxed/exponential/instances_30/"
#path = "/home/massimiliano/Scrivania/single_simulation/"

directory_hour = "saas_robust_game"

file_name = "response_time"

gamma = np.zeros((6,24))


for i in range(6):
    mean_VM = 0.0;
    for hour in range(24):
        #print(path+ directory_hour+str(hour)+"/"+file_name+str(i)+".csv")
        file = csv.reader(open(path+ directory_hour+str(hour)+"/"+file_name+str(i)+".csv"))
        lines = list(file)
        for current_line in range(1,len(lines)):
            #print(float(lines[current_line][0]))
            vm = float(lines[current_line][2])
            mean_VM = mean_VM + vm
        mean_VM = float(mean_VM)/float((len(lines)-1))
        gamma[i,hour] = mean_VM

gamma0 = gamma[0][:]
gamma1 = gamma[1][:]
gamma2 = gamma[2][:]
gamma3 = gamma[3][:]
gamma4 = gamma[4][:]
gamma5 = gamma[5][:]

x = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23']

plt.ylabel('Average Drop [job/s]')
plt.xlabel('Time of the day [h]')
plt.title('Daily Average Drop |SaaSs| = 30')
#plt.legend([gamma0, gamma1, gamma2, gamma3, gamma4, gamma5],['Gamma 0', 'Gamma 1', 'Gamma 2', 'Gamma 3', 'Gamma 4', 'Gamma 5'])

plt.plot(x,gamma0, label="Gamma 0")
plt.plot(x,gamma1, label="Gamma 1")
plt.plot(x,gamma2, label="Gamma 2")
plt.plot(x,gamma3, label="Gamma 3")
plt.plot(x,gamma4, label="Gamma 4")
plt.plot(x,gamma5, label="Gamma 5")
plt.legend(loc="best", prop={'size': 6})
plt.show()
