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
    r = csv.reader(open(path+'/WSs_parameters.csv'))
    count = sum(1 for line in r)
    return count


#download of the file
#os.system("scp simula@matemagician.deib.polimi.it:/home/simula/dief_2018_fog_simulator/omnetpp-5.4.1/samples/Fog/simulations/FogDaniloConfig.json " + path_template)
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
    data = json.load(open(path_template+"/FogDaniloConfig.json"))
    for i in range(1, compute_template_dimension() - 1 ):
        datum = data["workload"]["class1"]
        datum = {f"class{i+1}":datum}
        #print(datum)
        data["workload"].update(datum)
        #print(data["workload"])
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

download_template()
prepare_template()
upload_file()
