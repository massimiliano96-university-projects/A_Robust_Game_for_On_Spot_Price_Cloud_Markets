import os
import csv
import sys

application = "saas_robust_game"

path = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/"

gamma = sys.argv[1]

i_max= 23

for i in range(0, i_max):
    directory_name = application + str(i)
    command = "cd sequential_version; ./exe g "+path+directory_name+"/config_file.csv"
    os.system(command)
    r = csv.reader(open('/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version/global_parameters.csv')) # Here your csv file
    lines = list(r)
    lines[1][2] = gamma
    print(lines)
    writer = csv.writer(open('/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version/global_parameters.csv', 'w'))
    writer.writerows(lines)
    del r
    del writer
    os.system("cd /home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version/; cp global_parameters.csv SaaSs_parameters.csv WSs_parameters.csv "+ path +directory_name)
    command1 =  "cd /home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version/; ./exe e "+path+directory_name+"/config_file.csv " +path+directory_name+"/global_parameters.csv " \
                +path+directory_name+"/SaaSs_parameters.csv "+path+directory_name+"/WSs_parameters.csv "
    os.system(command1)
    os.system("cd /home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version/; cp results.txt response_time.csv game_result.csv "+ path +directory_name)
