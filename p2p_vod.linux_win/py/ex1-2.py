__author__ = 'wbtang'

import log_base
import sys

def parse_cmd():
    exe = log_base.get_full_path(log_base.file_exe)
    sys_cmd = ''

    ps = False
    for i in range(len(sys.argv)):
        if ps == True:
            ps = False
            continue
        key = sys.argv[i]
        if key[0] == '-':
            ps = True
            value = sys.argv[i+1]
            if key[1:] == 'g':
                log_base.graph_type = value
            else:
                sys_cmd += ' %s %s' % (key, value)

    log_base.verify_graph_type()
    cmd = sys_cmd
    cmd += ' -FORCE_%s_GRAPH true' % (log_base.graph_type)
    log_name = '%s/ex1/%s_unselfish_%s.xls' % (log_base.folder_log, log_base.graph_type, log_base.get_time())
    cmd += ' -LOG_NAME %s' % (log_name)

    log_base.run_exe(exe, cmd, async=False)

    cmd = ' ex2.py %s' % sys_cmd
    cmd += ' -m %s' % (log_name)
    log_base.run_exe('python', cmd, async=False)

if __name__ == '__main__':
    log_base.graph_type = ''

    parse_cmd()
