import time
import os
import csv
from paramiko import SSHClient
#from scp import SCPClient
import json
import glob
import pandas as pd

#path = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version"
#path_template = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/simulator/templates"
file_path_on_remote_machine = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations"
path = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version"
path_template="/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/simulator/templates"
path_simulation ="/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations"
path_run = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/"



# count of the number of WSs
def compute_template_dimension():
    r = csv.reader(open(path+'/config_file.csv'))
    lines = list(r)
    n_saas = lines[1][0]
    n_app = lines[1][1]
    n_ws = lines[1][2]
    return int(n_saas), int(n_app), int(n_ws)


def prepare_template():
    os.system("rm -r "+path_template+"/FogDaniloConfig.json")
    data = json.load(open(path_template+"/FogDaniloConfig_template.json"))
    n_saas, n_app, n_ws = compute_template_dimension()
    datum = data["workload"]["class0"]
    for i in range(1, n_ws):
        #datum = {f'class{i}':datum}
        datum = {"class"+str(i):datum}
        data["workload"].update(datum)
        with open(path_template+"/FogDaniloConfig.json", "w") as file:
            json.dump(data, file)
        datum = data["workload"]["class0"]


def complete_template( current_row, time ): #prepare the file .json for a single app, the current row input indicates at which row we have to start

    specific_path = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/"
    directory_name ="saas_robust_game"+str(time)

    n_saas, n_app, n_ws = compute_template_dimension()
    ws = csv.reader(open(specific_path+ directory_name +'/WSs_parameters.csv')) # Here your csv file
    lines_ws = list(ws)

    param = csv.reader(open(specific_path+ directory_name +'/global_parameters.csv'))
    lines_param = list(param)
    print(lines_param)

    rt = csv.reader(open(specific_path+ directory_name +'/response_time.csv'))
    lines_rt = list(rt)

    data = json.load(open(path_template+'/FogDaniloConfig.json'))

    gamma = int(lines_param[1][2])

    #data["name"] = f'Test{lines_ws[1][6]}_{lines_ws[1][7]}'
    data["name"] = "Test_"+str(lines_ws[current_row][6])+"_"+str(lines_ws[current_row][7])

    data["nservers"] = lines_rt[current_row][0]

    class_index = 0

    for i in range(current_row, current_row + n_ws):
        data["workload"]["class"+str(class_index)]["lambda"] = lines_ws[i][0]
    #if gamma > 0 :
        data["workload"]["class"+str(class_index)]["mu_min"] = float(lines_ws[i][3]) - float(lines_param[1][5]) * float(lines_ws[i][3])
        data["workload"]["class"+str(class_index)]["mu_max"] = float(lines_ws[i][3]) + float(lines_param[1][5]) * float(lines_ws[i][3])
        data["workload"]["class"+str(class_index)]["delay_min"] = float(lines_ws[i][2]) - float(lines_param[1][6]) * float(lines_ws[i][2])
        data["workload"]["class"+str(class_index)]["delay_max"] = float(lines_ws[i][2]) + float(lines_param[1][6]) * float(lines_ws[i][2])
        #print(float(lines_ws[i][2]))
        #gamma -= 1
    #else :
        #data["workload"]["class"+str(class_index)]["mu_min"] = float(lines_ws[i][3])
        #data["workload"]["class"+str(class_index)]["mu_max"] = float(lines_ws[i][3])
        #data["workload"]["class"+str(class_index)]["delay_min"] = float(lines_ws[i][2])
        #data["workload"]["class"+str(class_index)]["delay_max"] = float(lines_ws[i][2])
        class_index += 1
    with open(path_template+"/FogDaniloConfig.json", "w") as file:
        json.dump(data, file)

def single_simulation():
    os.system("pwd")
    os.system("cd "+path_simulation)
    os.system("pwd")
    os.system("cd "+path_simulation+ ";" + "touch -c configDanilo_template.json FogDanilo_template.ini")
    os.system("cd "+path_simulation+ ";" + "./update_template.py")
    os.system("cd "+path_simulation+ ";" + "./makeRunfile.sh FogDanilo.ini")
    os.system("cd "+path_simulation+ ";" + "make -j 18 -f Runfile")
    os.system("cd "+path_simulation+ ";" + "./parse_data4.py -r -j 18 -c configDanilo.json -d danilo.db results/*.sca")
    os.system("cd "+path_simulation+ ";" + "./analyze_data2.py -d danilo.db -c configDanilo.json")
    os.system("cd "+path_simulation+ "/results/" + ";" + "rm -rf *.sca")

def run_app(current_row, time):
    complete_template(current_row,time)
    os.system("cp "+path_template+"/FogDaniloConfig.json /home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/")
    single_simulation()
    os.system("cd "+path_simulation+ ";" + "cp *.data /home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/saas_robust_game"+str(time))
    os.system("cd "+path_simulation+ ";" + "rm -rf *.data")


def run_saas(current_row,time):
    n_saas, n_app, n_ws = compute_template_dimension()
    for i in range(n_app):
        run_app(current_row,time)
        current_row += n_ws

def single_run(time):
    prepare_template()
    current_row = 1
    n_saas, n_app, n_ws = compute_template_dimension()
    for i in range(n_saas):
        run_saas(current_row,time)
        current_row += n_app*n_ws

def run(gamma):
    i_max = 23
    for i in range(i_max):
        os.system("cd /home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/saas_robust_game"+str(i)+ ";" + " rm -rf *.data")
        single_run(i)
        parse_data(i, gamma)
        parse_data_drop(i, gamma)

def parse_data(index, gamma):
    path_1 = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/saas_robust_game"+str(index)
    n_saas, n_app, n_ws = compute_template_dimension()
    try:
        f = open(path_1 + "/Response_time_simulator_"+str(gamma)+".csv")
        os.system("cd "+path_1+"; rm /Response_time_simulator_"+str(gamma)+".csv")
    except IOError:
        print("File not accessible")
    file_write_name = "/Response_time_simulator_"+str(gamma)+".csv"
    writer = csv.writer(open(path_1+"/"+file_write_name, 'w'))
    writer.writerow(["response_time", "SaaS_index", "app_index", "gamma"])

    for k in range(n_saas):
        for j in range(n_app):
            file_name = "Test_"+str(k)+"_"+str(j)+".data"
            file = open(path_1+"/"+file_name, "r")
            lines = file.readlines()
            line = lines[1]
            count = 0
            count_s = 0
            flag = True
            datum = ""
            for i in range(len(line)):
                if line[i].isspace() :
                    count_s += 1
                if count_s > 5*n_ws :
                    if line[i].isspace():
                        count += 1
                    if count == 4 and flag :
                        datum = datum + line[i]
                        if line[i+1].isspace() :
                            flag = False
                            writer.writerow([datum, k, j, gamma])
                            datum = ""
                            count = 0
                    if count == 14 and not flag :
                            flag = True
                            count = 0

            file.close()
    del writer


def parse_data_drop(index, gamma):
    path_1 = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/saas_robust_game"+str(index)
    n_saas, n_app, n_ws = compute_template_dimension()
    try:
        f = open(path_1 + "/drop_simulator_"+str(gamma)+".csv")
        os.system("cd "+path_1+"; rm /drop_simulator_"+str(gamma)+".csv")
    except IOError:
        print("File not accessible")
    file_write_name = "/drop_simulator_"+str(gamma)+".csv"
    writer = csv.writer(open(path_1+"/"+file_write_name, 'w'))
    writer.writerow(["Arrived", "Sent_avg", "SaaS_index", "app_index", "gamma"])

    for k in range(n_saas):
        for j in range(n_app):
            file_name = "Test_"+str(k)+"_"+str(j)+"_onlydrop.data"
            file = open(path_1+"/"+file_name, "r")
            lines = file.readlines()
            line = lines[1]
            count = 0
            count_s = 0
            flag = True
            datum1 = ""
            datum2 = ""
            for i in range(len(line)):
                if line[i].isspace() :
                    count_s += 1
                if count_s >= 5*n_ws :
                    if line[i].isspace():
                        count += 1
                    if count == 1:
                        datum1 = datum1 + line[i]
                    if count == 3:
                        datum2 = datum2 + line[i]
                    if i + 1 < len(line):
                        if line[i+1].isspace() and count == 3 :
                            flag = False
                            writer.writerow([datum1, datum2, k, j, gamma])
                            datum1 = ""
                            datum2 = ""
                    if count == 4:
                        count = 0
                        flag = True


            file.close()
    del writer

def run_all():
    i_max=23
    n_saas, n_app, n_ws = compute_template_dimension()
    os.system("cd "+path_run+"; python3 setup.py")
    for gamma in range(n_ws + 1):
        os.system("cd "+path_run+"; python3 run.py "+ str(gamma))
        run(gamma)
        for index in range(i_max):
            path_1 = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/saas_robust_game"+str(index)
            os.system("cd "+path_1+"; mv response_time.csv response_time"+str(gamma)+".csv")
            os.system("cd "+path_1+"; mv game_result.csv game_result"+str(gamma)+".csv")

        """
        dfs = []
        dfs1 = []
        for file in glob.glob("Response_time_simulator_"+str(gamma)+".csv"):
            dfs.append(pd.read_csv(file))
            pd.concat(dfs)
        for file in glob.glob("drop_simulator_"+str(gamma)+".csv"):
            dfs1.append(pd.read_csv(file))
            pd.concat(dfs1)
        """

tic = time.perf_counter()

run_all()

toc = time.perf_counter()

print("Run performed in "+ str(toc - tic)+" seconds")
