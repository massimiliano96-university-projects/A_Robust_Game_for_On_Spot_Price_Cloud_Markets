import os

os.system("pwd")

def single_simulation():
    os.system("touch configDanilo_template.json FogDanilo_template.ini")
    os.system("./update_template.py")
    os.system("./makeRunfile.sh FogDanilo.ini")
    os.system("make -j 18 -f Runfile")
    os.system("./parse_data4.py -r -j 18 -c configDanilo.json -d danilo.db results/*.sca")
    os.system("./analyze_data2.py -d danilo.db -c configDanilo.json")
    os.system("less Test_prova.data")

single_simulation()
