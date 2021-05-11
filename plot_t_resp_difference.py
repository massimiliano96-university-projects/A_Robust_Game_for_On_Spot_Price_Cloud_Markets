import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os
import csv

path = "/home/massimiliano/Scrivania/instances_tot_relaxed/lognormal/instances_1/"
#path = "/home/massimiliano/Scrivania/instances_relaxed/"

directory_hour = "saas_robust_game"

file_name1 = "Response_time_simulator_"
file_name2 = "response_time"

gamma = np.zeros((6))

for i in range(6):
    final_t_risp = 0.0
    for hour in range(24):
        #print(path+ directory_hour+str(hour)+"/"+file_name+str(i)+".csv")
        file1 = csv.reader(open(path+ directory_hour+str(hour)+"/"+file_name1+str(i)+".csv"))
        file2 = csv.reader(open(path+ directory_hour+str(hour)+"/"+file_name2+str(i)+".csv"))
        lines1 = list(file1)
        lines2 = list(file2)
        t_risp_sim = 0.0
        t_risp = 0.0
        for current_line in range(1,len(lines1)):
            t_risp_sim = t_risp_sim + float(lines1[current_line][0])
            t_risp =  t_risp + float(lines2[current_line][4])
        final_t_risp = final_t_risp + abs(t_risp_sim - t_risp)/len(lines1)
    gamma[i] = final_t_risp/24


'''
gamma0 = gamma[0]
gamma1 = gamma[1]
gamma2 = gamma[2]
gamma3 = gamma[3]
gamma4 = gamma[4]
gamma5 = gamma[5]
'''
#print(gamma0)

x = ['0', '1', '2', '3', '4', '5']

plt.ylabel('Average Response Time [s]')
plt.xlabel('Gamma')
plt.title('Response Time Analytical Model vs Simulator (lognormal)')
#plt.legend([gamma0, gamma1, gamma2, gamma3, gamma4, gamma5],['Gamma 0', 'Gamma 1', 'Gamma 2', 'Gamma 3', 'Gamma 4', 'Gamma 5'])

plt.plot(x,gamma, label="Average difference Response Time")
plt.legend(loc="best", prop={'size': 6})
plt.show()
