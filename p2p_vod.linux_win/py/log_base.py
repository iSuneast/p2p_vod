__author__ = 'wbtang'

import datetime
import os

folder_src = '../src'
folder_log = '../log'
folder_release = '../release'

folder_final = '../ex_final'

folder_final_ex1 = '%s/%s' % (folder_final, 'ex1')
folder_final_ex2 = '%s/%s' % (folder_final, 'ex2')
folder_final_ex3 = '%s/%s' % (folder_final, 'ex3')

__name_exe = 'p2p_vod.exe'
__name_parser_ini = 'cmd.ini'

file_exe = '%s/%s' % (folder_release, __name_exe)
file_parser_ini = '%s/%s' % (folder_src, __name_parser_ini)

file_ex3_dump = 'ex3.dump'
file_ex2_log = 'monster_two_world.xls'

file_figure_extend = '.jpg'

graphs = {'BTC': True, 'PA': True, 'TWITTER': True}
graph_type = 'I DO NOT KNOW'

def folder_graph():
    return '%s/%s' % (folder_final, graph_type)

def get_time():
    return datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S')

def run_exe(exe, opt, async):
    if async == True:
        if is_windows():
            cmd = 'start %s %s' % (exe, opt)
        else:
            cmd = 'exec %s %s >> trace_screen &' % (exe, opt)
    else:
        if is_windows():
            cmd = '%s %s' % (exe, opt)
        else:
            cmd = 'exec %s %s' % (exe, opt)
    print('cmd: %s' % cmd)
    os.system(cmd)

def run_py_script(script, async):
    script = get_full_path(script)
    run_exe('python', script, async)

def get_full_path(file):
    file = os.path.abspath(file)
    for i in range(len(file)):
        if file[i] == '\\':
            file = file[:i] + '/' + file[(i+1):]
    return file

def is_windows():
    return os.name == 'nt'

def default_processor():
    if is_windows():
        return 3
    else:
        return 15

def make_script(scripts, jobs):
    for i in range(len(scripts)):
        file = open(scripts[i], "w")
        file.write('import os\n')

        for j in range(i, len(jobs), len(scripts)):
            file.write('os.system(\'%s %s\')\n' % (get_full_path(file_exe), jobs[j]))
        file.close()
        run_py_script(scripts[i], async=True)

scale = 1.

class PeerData:
    peer_id = -1

    enter_time = -1
    finish_time = -1
    exit_time = -1

    start_position = -1
    latency = -1
    interrupts = -1
    smoothness = -1

    def __init__(self, data):
        self.peer_id = int(data[0])
        self.enter_time = int(data[1])
        self.finish_time = int(data[2])
        self.start_position = int(data[3])
        self.latency = int(data[4])
        self.interrupts = int(data[5])
        self.smoothness = float(data[6]) * scale
        self.exit_time = int(data[7])

def __parse(file):
    conf = {}
    peer_data = []
    for line in file:
        if not line[:1].isdigit():
            tokens = line.strip().split(' ')
            tokens = list(filter(None, tokens))
            if len(tokens) == 4 and tokens[2] == '=':
                conf[tokens[1]] = tokens[3]
        else:
            peer_data.append(PeerData(line.strip().split('\t')))
    return conf, peer_data

def read(log_name):
    print('Parse Log: %s' % log_name)
    file = open(log_name)
    return __parse(file)

def verify_graph_type():
    if graph_type not in graphs:
        print('unknown graph: %s' % graph_type)
        print('valid graph: %s' % str(graphs))
        exit()
    print('verify graph: (OK) %s' % graph_type)
