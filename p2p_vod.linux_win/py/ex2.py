__author__ = 'wbtang'

import sys
import log_base
import random

# -p 'number of process'
# -m 'MONSTER NOTE'
# -n 'max num of id'
# -g 'graph type'

def select_peer(peers):
    p = int(random.random() * len(peers))
    return peers[p]

def get_monster_id(monster_note, max_num_id):
    print('max_num_id = %s' % max_num_id)
    print('monster_note = %s' % monster_note)

    conf, peer_data = log_base.read(monster_note)

    smoothness_slot = 0
    latency_slot = 0
    for data in peer_data:
        if data.finish_time != -1:
            smoothness_slot_p =  int(data.smoothness * 100.)
            smoothness_slot = max(smoothness_slot, smoothness_slot_p)
            latency_slot = max(latency_slot, data.latency)

    print('smoothness_slot = %d' % smoothness_slot)
    print('latency_slot = %d' % latency_slot)

    smoothness = [[] for i in range(smoothness_slot + 1)]
    latency = [[] for i in range(latency_slot + 1)]
    for data in peer_data:
        if data.finish_time != -1:
            smoothness_slot_p = int(data.smoothness * 100.)
            smoothness[smoothness_slot_p].append(data.peer_id)
            latency[data.latency].append(data.peer_id)

    candidate_id_set = {}
    for peers in smoothness:
        if len(peers) != 0:
            candidate_id_set[ select_peer(peers) ] = True
            candidate_id_set[ select_peer(peers) ] = True
            candidate_id_set[ select_peer(peers) ] = True
    for peers in latency:
        if len(peers) != 0:
            candidate_id_set[ select_peer(peers) ] = True
            candidate_id_set[ select_peer(peers) ] = True
            candidate_id_set[ select_peer(peers) ] = True

    monster_id = [val for val in candidate_id_set]
    if len(monster_id) > max_num_id and False: # disabled
        for i in range(len(monster_id)):
            j = int(random.random() * (len(monster_id) - i)) + i
            t = monster_id[j]
            monster_id[j] = monster_id[i]
            monster_id[i] = t
        monster_id = monster_id[:max_num_id]

    monster_id.sort()
    return monster_id

def make_script_with(monster_id, cmd, num_processor):
    print('cmd = %s' % cmd)
    print('num_process = %d' % num_processor)
    print('monster_id (%d)' % len(monster_id))
    print(monster_id)

    scripts = [('%s/run_ex2_%02d.py' % (log_base.folder_release, i))
               for i in range(num_processor)]
    jobs = [('%s -LOG_NAME_EX id_%06d -MONSTER_SELFISH_ID %d' % (cmd, id, id))
            for id in monster_id]
    log_base.make_script(scripts, jobs)

def parse_cmd():
    num_processor = log_base.default_processor()
    max_num_id = 300
    monster_note = ''
    cmd = ''

    ps = False
    for i in range(len(sys.argv)):
        if ps == True:
            ps = False
            continue
        key = sys.argv[i]
        if key[0] == '-':
            ps = True
            value = sys.argv[i+1]
            if key[1:] == 'p':
                num_processor = int(value)
            elif key[1:] == 'm':
                monster_note = value
            elif key[1:] == 'n':
                max_num_id = int(value)
            elif key[1:] == 'g':
                log_base.graph_type = value
            else:
                cmd += ' %s %s' % (key, value)

    log_base.verify_graph_type()

    if monster_note == '':
        monster_note = '../log/ex2/%s/%s_unselfish.xls'\
                       % (log_base.graph_type, log_base.graph_type)
    cmd += ' -WORK_DIRECTORY %s/ex2/%s' % (log_base.folder_log, log_base.graph_type)
    cmd += ' -FORCE_%s_GRAPH true' % (log_base.graph_type)
    cmd += ' -MONSTER_NOTE %s' % (monster_note)
    cmd += ' -ENABLE_MONSTER true'
    cmd += ' -MONSTER_TWO_WORLD %s/ex2/%s/%s_%s_%s' % (log_base.folder_log,
                                                       log_base.graph_type,
                                                       log_base.graph_type,
                                                       log_base.get_time(),
                                                       log_base.file_ex2_log)

    monster_id = get_monster_id(monster_note, max_num_id)
    make_script_with(monster_id, cmd, num_processor)

if __name__ == '__main__':
    log_base.graph_type = ''

    parse_cmd()
