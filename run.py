import os

application = "saas_robust_game"

path = "/home/massimiliano/github/A_Robust_Game_for_On_Spot_Price_Cloud_Markets/instances/"

i_max= 24

for i in range(0, i_max):
    directory_name = application + str(i)
    command = "cd sequential_version; ./exe g "+path+directory_name+"/config_file.csv"
    os.system(command)
    os.system("cd sequential_version; cp global_parameters.csv SaaSs_parameters.csv WSs_parameters.csv "+ path +directory_name)
    command1 =  "cd sequential_version; ./exe e "+path+directory_name+"/config_file.csv " +path+directory_name+"/global_parameters.csv " \
                +path+directory_name+"/SaaSs_parameters.csv "+path+directory_name+"/WSs_parameters.csv "
    os.system(command1)
    os.system("cd sequential_version; cp results.txt response_time.csv "+ path +directory_name)
