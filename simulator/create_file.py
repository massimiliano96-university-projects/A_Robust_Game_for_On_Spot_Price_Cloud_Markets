import os
import csv
from paramiko import SSHClient
from scp import SCPClient
import json

path = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/sequential_version"
path_template = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/simulator/templates"
file_path_on_remote_machine = "/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations"



# count of the number of WSs
def compute_template_dimension():
    os.system("cd "+path)
    r = csv.reader(open(path+'/config_file.csv'))
    lines = list(r)
    n_saas = lines[1][0]
    n_app = lines[1][1]
    n_ws = lines[1][2]
    return int(n_saas), int(n_app), int(n_ws)


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

def prepare_template():
    data = json.load(open(path_template+"/FogDaniloConfig_template.json"))
    n_saas, n_app, n_ws = compute_template_dimension()
    for i in range(0, n_ws):
        datum = data["workload"]["class0"]
        datum = {f"class{i}":datum}
        data["workload"].update(datum)
        with open(path_template+"/FogDaniloConfig.json", "w") as file:
            json.dump(data, file)

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

def complete_template( current_row ): #prepare the file .json for a single app, the current row input indicates at which row we have to start

    n_saas, n_app, n_ws = compute_template_dimension()
    ws = csv.reader(open(path+'/WSs_parameters.csv')) # Here your csv file
    lines_ws = list(ws)

    param = csv.reader(open(path+'/global_parameters.csv'))
    lines_param = list(param)

    rt = csv.reader(open(path+'/response_time.csv'))
    lines_rt = list(rt)

    data = json.load(open(path_template+"/FogDaniloConfig.json"))

    gamma = int(lines_param[1][2])

    data["name"] = f"Test{lines_ws[1][6]}_{lines_ws[1][7]}"

    data["nservers"] = lines_rt[current_row][0]

    for i in range(n_ws):
        data["workload"][f"class{i}"]["lambda"] = lines_ws[i+1][0]
        if gamma > 0 :
            data["workload"][f"class{i}"]["mu_min"] = float(lines_ws[i+1][3]) - float(lines_param[1][5]) * float(lines_ws[i+1][3])
            data["workload"][f"class{i}"]["mu_max"] = float(lines_ws[i+1][3]) + float(lines_param[1][5]) * float(lines_ws[i+1][3])
            data["workload"][f"class{i}"]["delay_min"] = float(lines_ws[i+1][2]) - float(lines_param[1][6]) * float(lines_ws[i+1][2])
            data["workload"][f"class{i}"]["delay_max"] = float(lines_ws[i+1][2]) + float(lines_param[1][6]) * float(lines_ws[i+1][2])
            gamma -= 1
        else :
            data["workload"][f"class{i}"]["mu_min"] = float(lines_ws[i+1][3])
            data["workload"][f"class{i}"]["mu_max"] = float(lines_ws[i+1][3])
            data["workload"][f"class{i}"]["delay_min"] = float(lines_ws[i+1][2])
            data["workload"][f"class{i}"]["delay_max"] = float(lines_ws[i+1][2])
    with open(path_template+"/FogDaniloConfig.json", "w") as file:
        json.dump(data, file)

def single_simulation():
        ssh = SSHClient()
        ssh.load_system_host_keys()
        ssh.connect(hostname='matemagician.deib.polimi.it',
                    username='simula',
                    password='ricibm76')
        command = "cd dief_2018_fog_simulator/omnetpp-5.4.1/;. setenv;cd samples/Fog/simulations/;touch configDanilo_template.json FogDanilo_template.ini;\
                    ./update_template.py;./makeRunfile.sh FogDanilo.ini;make -j 18 -f Runfile;./parse_data4.py -r -j 18 -c configDanilo.json -d danilo.db results/*.sca;\
                    ./analyze_data2.py -d danilo.db -c configDanilo.json"
        command1 = "cd dief_2018_fog_simulator/omnetpp-5.4.1/;. setenv"
        stdin_, stdout_, stderr_ = ssh.exec_command(command1)
        stdout_.channel.recv_exit_status()
        lines = stdout_.readlines()
        for line in lines:
            print(line)

        #ssh.exec_command("cd dief_2018_fog_simulator/omnetpp-5.4.1/")
        #ssh.exec_command(". setenv")
        #ssh.exec_command("cd samples/Fog/simulations/")
        #ssh.exec_command("touch configDanilo_template.json FogDanilo_template.ini")
        #ssh.exec_command("./update_template.py")
        #ssh.exec_command("./makeRunfile.sh FogDanilo.ini")
        #ssh.exec_command("make -j 18 -f Runfile")
        #ssh.exec_command("./parse_data4.py -r -j 18 -c configDanilo.json -d danilo.db results/*.sca")
        #ssh.exec_command("./analyze_data2.py -d danilo.db -c configDanilo.json")
        #ssh.close()

#download_template()
#prepare_template()
#upload_file()
#complete_template(1)
single_simulation()
