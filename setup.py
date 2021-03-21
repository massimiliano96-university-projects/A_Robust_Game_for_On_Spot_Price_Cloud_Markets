import os
import csv

application = "saas_robust_game"

#path = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/"
path = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/"

os.system("cd instances; rm -rf *")

#time= [0.55, 0.557, 0.467, 0.446, 0.459, 0.462, 0.452, 0.422, 0.413, 0.480, 0.517, 0.523, 0.424, 0.570, 0.547, 0.580, 0.587, 0.635, 0.664, 0.568, 0.599, 0.643, 1, 0.876 ] #spiky trace
time =[0.635, 0.657, 0.516, 0.509, 0.466, 0.412, 0.379, 0.397, 0.401, 0.437, 0.505, 0.578, 0.61, 0.679, 0.65, 0.726, 0.816, 1, 0.87, 0.801, 0.884, 0.852, 0.903, 0.819] #relaxed trace

i_max= 23

for i in range(0, i_max):
    directory_name = application + str(i)
    command1 = "mkdir " +directory_name
    os.system( "cd instances; "+ command1)
    os.system("cd sequential_version; cp config_file.csv "+ path +directory_name)
    r = csv.reader(open(path+directory_name+'/config_file.csv')) # Here your csv file
    lines = list(r)
    lines[1][3] = time[i]
    writer = csv.writer(open(path+directory_name+'/config_file.csv', 'w'))
    writer.writerows(lines)
