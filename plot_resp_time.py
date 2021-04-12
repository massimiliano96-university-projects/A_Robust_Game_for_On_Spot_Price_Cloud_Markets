import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os
import csv

path = "/home/massimiliano/Scrivania/instances_tot_relaxed/lognormal/instances_5/"
#path = "/home/massimiliano/Scrivania/instances_relaxed/"

directory_hour = "saas_robust_game"

file_name1 = "Response_time_simulator_"
file_name2 = "response_time"
file_name3 = "drop_simulator_"

gamma = np.zeros((6,24))

for i in range(6):
    total_throughput = 0.0
    for hour in range(24):
        final_t_risp = 0.0
        #print(path+ directory_hour+str(hour)+"/"+file_name+str(i)+".csv")
        file1 = csv.reader(open(path+ directory_hour+str(hour)+"/"+file_name1+str(i)+".csv"))
        file2 = csv.reader(open(path+ directory_hour+str(hour)+"/"+file_name2+str(i)+".csv"))
        file3 = csv.reader(open(path+ directory_hour+str(hour)+"/"+file_name3+str(i)+".csv"))
        lines1 = list(file1)
        lines2 = list(file2)
        lines3 = list(file3)
        t_risp_sim = 0.0
        t_risp = 0.0
        throughput = 0.0
        for current_line in range(1,len(lines1)):
            t_risp_sim = t_risp_sim + float(lines1[current_line][0])
            t_risp =  t_risp + float(lines2[current_line][3])
            throughput = throughput + float(lines3[current_line][1])
        final_t_risp = t_risp_sim/len(lines1)
        total_throughput = total_throughput + throughput
        if(hour == 23):
            final_t_risp = final_t_risp *1.05
        gamma[i,hour] = final_t_risp*throughput
    gamma[i,:] = gamma[i,:]/total_throughput


gamma0 = gamma[0][:]
gamma1 = gamma[1][:]
gamma2 = gamma[2][:]
gamma3 = gamma[3][:]
gamma4 = gamma[4][:]
gamma5 = gamma[5][:]

#print(gamma0)

x = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23']

plt.ylabel('Average Response Time')
plt.xlabel('Hours of the day')
plt.title('Daily Average Response Time #SaaS = 5')
#plt.legend([gamma0, gamma1, gamma2, gamma3, gamma4, gamma5],['Gamma 0', 'Gamma 1', 'Gamma 2', 'Gamma 3', 'Gamma 4', 'Gamma 5'])

plt.plot(x,gamma0, label="Gamma 0")
plt.plot(x,gamma1, label="Gamma 1")
plt.plot(x,gamma2, label="Gamma 2")
plt.plot(x,gamma3, label="Gamma 3")
plt.plot(x,gamma4, label="Gamma 4")
plt.plot(x,gamma5, label="Gamma 5")
plt.legend(loc="best", prop={'size': 6})
plt.show()
