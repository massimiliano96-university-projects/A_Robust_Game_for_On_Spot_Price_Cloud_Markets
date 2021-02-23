import os
import csv
from paramiko import SSHClient
#from scp import SCPClient
import json

#path = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version"
#path_template = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/simulator/templates"
file_path_on_remote_machine = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations"
path = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version"
path_template="/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/simulator/templates"
path_simulation ="/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations"



# count of the number of WSs
def compute_template_dimension():
    r = csv.reader(open(path+'/config_file.csv'))
    lines = list(r)
    n_saas = lines[1][0]
    n_app = lines[1][1]
    n_ws = lines[1][2]
    return int(n_saas), int(n_app), int(n_ws)

"""
#download of the file
def download_template():
    ssh = SSHClient()
    ssh.load_system_host_keys()
    ssh.connect(hostname='matemagician.deib.polimi.it',
                username='simula',
                password='ricibm76')
    # SCPCLient takes a paramiko transport as its only argument
    scp = SCPClient(ssh.get_transport())

    #scp.put('file_path_on_local_machine', 'file_path_on_remote_machine')
    scp.get(file_path_on_remote_machine+"/FogDaniloConfig.json", path_template)
    scp.close()
"""

def prepare_template():
    data = json.load(open(path_template+"/FogDaniloConfig_template.json"))
    n_saas, n_app, n_ws = compute_template_dimension()
    for i in range(0, n_ws):
        datum = data["workload"]["class0"]
        #datum = {f'class{i}':datum}
        datum= { "class"+str(i):datum }
        data["workload"].update(datum)
        with open(path_template+"/FogDaniloConfig.json", "w") as file:
            json.dump(data, file)
"""
def upload_file():
    ssh = SSHClient()
    ssh.load_system_host_keys()
    ssh.connect(hostname='matemagician.deib.polimi.it',
                username='simula',
                password='ricibm76')
    # SCPCLient takes a paramiko transport as its only argument
    scp = SCPClient(ssh.get_transport())
    scp.put(path_template+"/FogDaniloConfig.json", file_path_on_remote_machine)
    #scp.get(file_path_on_remote_machine, path_template)
    scp.close()
    ssh.close()
"""


def complete_template( current_row, time ): #prepare the file .json for a single app, the current row input indicates at which row we have to start

    specific_path = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/"
    directory_name ="saas_robust_game"+str(time)

    n_saas, n_app, n_ws = compute_template_dimension()
    ws = csv.reader(open(specific_path+ directory_name +'/WSs_parameters.csv')) # Here your csv file
    lines_ws = list(ws)

    param = csv.reader(open(specific_path+ directory_name +'/global_parameters.csv'))
    lines_param = list(param)

    rt = csv.reader(open(specific_path+ directory_name +'/response_time.csv'))
    lines_rt = list(rt)

    data = json.load(open(path_template+'/FogDaniloConfig.json'))

    gamma = int(lines_param[1][2])

    #data["name"] = f'Test{lines_ws[1][6]}_{lines_ws[1][7]}'
    data["name"] = "Test"+str(lines_ws[1][6])+"_"+str(lines_ws[1][7])

    data["nservers"] = lines_rt[current_row][0]

    class_index = 0

    for i in range(current_row, current_row + n_ws-1):
        data["workload"]["class"+str(class_index)]["lambda"] = lines_ws[i][0]
        class_index += 1
        if gamma > 0 :
            data["workload"]["class"+str(i)]["mu_min"] = float(lines_ws[i][3]) - float(lines_param[1][5]) * float(lines_ws[i][3])
            data["workload"]["class"+str(i)]["mu_max"] = float(lines_ws[i][3]) + float(lines_param[1][5]) * float(lines_ws[i][3])
            data["workload"]["class"+str(i)]["delay_min"] = float(lines_ws[i][2]) - float(lines_param[1][6]) * float(lines_ws[i][2])
            data["workload"]["class"+str(i)]["delay_max"] = float(lines_ws[i][2]) + float(lines_param[1][6]) * float(lines_ws[i][2])
            gamma -= 1
        else :
            data["workload"]["class"+str(i)]["mu_min"] = float(lines_ws[i][3])
            data["workload"]["class"+str(i)]["mu_max"] = float(lines_ws[i][3])
            data["workload"]["class"+str(i)]["delay_min"] = float(lines_ws[i][2])
            data["workload"]["class"+str(i)]["delay_max"] = float(lines_ws[i][2])
    with open(path_template+"/FogDaniloConfig.json", "w") as file:
        json.dump(data, file)

def single_simulation():
    os.system("pwd")
    os.system("cd "+path_simulation)
    os.system("pwd")
    os.system("cd "+path_simulation+ ";" + "touch configDanilo_template.json FogDanilo_template.ini")
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
    os.system("cp *.data /home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/saas_robust_game"+str(time))
    os.system("rm -rf *.data")


def run_saas(current_row,time):
    n_saas, n_app, n_ws = compute_template_dimension()
    for i in range(n_app):
        run_app(current_row,time)
        current_row += n_ws

def single_run(time):
    current_row = 1
    n_saas, n_app, n_ws = compute_template_dimension()
    for i in range(n_saas):
        run_saas(current_row,time)
        current_row += n_app*n_ws

def run():
    i_max = 2
    for i in range(i_max):
        single_run(i)










run()

#download_template()
#prepare_template()
#upload_file()
#complete_template(1)
#single_simulation()
