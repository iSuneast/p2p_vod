__author__ = 'wbtang'

import sys
import log_base

# -p 'number of process'
# -g 'graph type'

def make_script_with(cmd, num_processor):
    print('cmd = %s' % cmd)
    print('num_process = %d' % num_processor)
    pr_selfish = [i for i in range(0, 101, 2)]
    print('selfish probability: ', pr_selfish)

    scripts = [('%s/run_ex3_%02d.py' % (log_base.folder_release, i))
               for i in range(num_processor)]
    jobs = [('%s -PR_SELFISH %.5f -LOG_NAME_EX ps_%03d'% (cmd, pr/100., pr))
            for pr in pr_selfish]

    log_base.make_script(scripts, jobs)


def parse_cmd():
    num_processor = log_base.default_processor()
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
            if key[1:] == 'g':
                log_base.graph_type = value
            else:
                cmd += ' %s %s' % (key, value)

    cmd += ' -WORK_DIRECTORY ../log/ex3/%s' % (log_base.graph_type)
    log_base.verify_graph_type()

    make_script_with(cmd, num_processor)

if __name__ == '__main__':
    log_base.graph_type = ''

    parse_cmd()
