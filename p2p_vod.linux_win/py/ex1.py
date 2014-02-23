__author__ = 'wbtang'

import log_base
import sys

# -g 'graph type'

def parse_cmd():
    exe = '%s/%s ' % (log_base.folder_release, log_base.file_exe)
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
            if key[1:] == 'g':
                log_base.graph_type = value
            else:
                cmd += ' %s %s' % (key, value)

    log_base.verify_graph_type()

    cmd += ' -WORK_DIRECTORY %s/ex1' % (log_base.folder_log)
    cmd += ' -FORCE_%s_GRAPH true' % (log_base.graph_type)
    log_base.run_exe(exe, cmd, async=True)

if __name__ == '__main__':
    log_base.graph_type = ''

    parse_cmd()
