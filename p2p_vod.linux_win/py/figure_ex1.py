__author__ = 'wbtang'

import log_base
import log_draw

large_latency = 10
bad_smoothness = 0.970

def draw(x, y):
    figures = log_draw.get_figure_path([
        '%s_un_latency_full' % log_base.graph_type,
        '%s_un_latency_large' % log_base.graph_type,
        '%s_un_fluency_full' % log_base.graph_type,
        '%s_un_fluency_bad' % log_base.graph_type
    ])
    titles = [
        '%s: latency' % log_base.graph_type,
        '%s: latency ( > %d)' % (log_base.graph_type, large_latency),
        '%s: fluency' % log_base.graph_type,
        '%s: %.2f < fluency < 1' % (log_base.graph_type ,bad_smoothness)
    ]
    x_labels = [
        'latency',
        'latency',
        'fluency',
        'fluency'
    ]
    y_labels = [
        '',
        '',
        '',
        ''
    ]
    log_draw.draw_all(figures, titles, x, y, x_labels, y_labels, plot_=log_draw.plot_ex)

def merge(v, map):
    if v in map:
        map[v] += 1
    else:
        map[v] = 1

def parse_log():
    log_file = '%s/%s_unselfish.xls' % (log_base.folder_final_ex1, log_base.graph_type)
    print('log file: ' + log_file)

    conf, peer_data = log_base.read(log_file)

    latency = {}
    smoothness = {}
    num_valid_peers = float(len(peer_data))
    for data in peer_data:
        merge(data.latency, latency)
        merge(max(0, data.smoothness), smoothness)

    x = [[] for i in range(4)]
    y = [[] for i in range(4)]
    # latency
    x[0], tmp = log_draw.get_xy_from_map(latency)
    y[0] = log_draw.get_prefix_sum([(val/num_valid_peers) for val in tmp])
    x[1], y[1] = x[0][large_latency:], y[0][large_latency:]
    # smoothness
    x[2], tmp = log_draw.get_xy_from_map(smoothness)
    x[2] = [(float(val)/log_base.scale) for val in x[2]]
    y[2] = log_draw.get_prefix_sum([(val/num_valid_peers) for val in tmp])
    for i in range(len(x[2])):
        if x[2][i] > bad_smoothness and x[2][i] < 1.0:
            x[3].append(x[2][i])
            y[3].append(y[2][i])

    draw(x, y)

if __name__ == '__main__':
    log_base.graph_type = 'TWITTER'

    parse_log()
