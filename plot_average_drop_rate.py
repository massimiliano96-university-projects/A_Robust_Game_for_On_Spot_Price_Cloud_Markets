import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os
import csv

path = "/home/massimiliano/Scrivania/instances_tot_relaxed/exponential/instances_30/"
#path = "/home/massimiliano/Scrivania/single_simulation/"

directory_hour = "saas_robust_game"

file_name = "response_time"
file_name2 = "WSs_parameters.csv"

gamma = np.zeros((6,24))



for i in range(6):
    for hour in range(24):
        mean_drop_rate = 0.0;
        num = 0
        den = 0
        #print(path+ directory_hour+str(hour)+"/"+file_name+str(i)+".csv")
        file = csv.reader(open(path+ directory_hour+str(hour)+"/"+file_name+str(i)+".csv"))
        lines = list(file)
        file2 = csv.reader(open(path + directory_hour+str(hour)+"/"+ file_name2))
        lines2 = list(file2)
        for current_line in range(1,len(lines)):
            #print(float(lines[current_line][0]))
            #drop_rate = (float(lines2[current_line][0]) - float(lines[current_line][3]))/float(lines2[current_line][0])
            num = num + float(lines[current_line][2])
            den = den + float(lines2[current_line][0])
            #print(drop_rate)
            #mean_drop_rate = mean_drop_rate + drop_rate
        #mean_drop_rate = float(mean_drop_rate)/float((len(lines)-1))
        mean_drop_rate = num / den
        gamma[i,hour] = mean_drop_rate

gamma0 = gamma[0][:]
gamma1 = gamma[1][:]
gamma2 = gamma[2][:]
gamma3 = gamma[3][:]
gamma4 = gamma[4][:]
gamma5 = gamma[5][:]

x = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23']

plt.ylabel('Average Drop Rate')
plt.xlabel('Time of the day [h]')
plt.title('Daily Average Drop Rate |SaaSs| = 30')
#plt.legend([gamma0, gamma1, gamma2, gamma3, gamma4, gamma5],['Gamma 0', 'Gamma 1', 'Gamma 2', 'Gamma 3', 'Gamma 4', 'Gamma 5'])

plt.plot(x,gamma0, label="Gamma 0")
plt.plot(x,gamma1, label="Gamma 1")
plt.plot(x,gamma2, label="Gamma 2")
plt.plot(x,gamma3, label="Gamma 3")
plt.plot(x,gamma4, label="Gamma 4")
plt.plot(x,gamma5, label="Gamma 5")
plt.legend(loc="best", prop={'size': 6})
plt.show()
