__author__ = 'wbtang'

import pickle
import os
import log_base
import log_draw

def get_avg(conf, peer_data):
    avg_latency = 0.
    avg_smoothness = 0.
    num_smoothness = 0
    for data in peer_data:
        avg_latency += data.latency
        if data.smoothness >= 0:
            num_smoothness += 1
            avg_smoothness += data.smoothness

    avg_latency /= len(peer_data)
    avg_smoothness /= num_smoothness * log_base.scale
    return avg_latency, avg_smoothness

def parse_folder(folder):
    latency = {}
    smoothness = {}

    for root, dirs, files in os.walk(folder):
        print(root, dirs, files)
        for file in files:
            log_name = '%s/%s' % (root, file)
            conf, peer_data = log_base.read(log_name)
            pr_selfish = float(conf['pr_selfish'])

            latency[pr_selfish], smoothness[pr_selfish] = get_avg(conf, peer_data)

            print('pr_selfish = %f, avg_latency = %f, avg_smoothness = %f'
                  % (pr_selfish, latency[pr_selfish], smoothness[pr_selfish]))

    x = [[] for i in range(2)]
    y = [[] for i in range(2)]
    x[0], y[0] = log_draw.get_xy_from_map(latency)
    x[1], y[1] = log_draw.get_xy_from_map(smoothness)
    return x, y

def draw(x, y):
    figures = log_draw.get_figure_path([
        '%s_ps_latency' % log_base.graph_type,
        '%s_ps_fluency' % log_base.graph_type
    ])
    titles = [
        '%s: average latency' % log_base.graph_type,
        '%s: average fluency' % log_base.graph_type
    ]
    x_labels = [
        'Percentage of selfish peers',
        'Percentage of selfish peers'
    ]
    y_labels = [
        '',
        ''
    ]

    log_draw.draw_all(figures, titles, x, y, x_labels, y_labels)

def parse_log():
    log_folder = '%s/%s' % (log_base.folder_final_ex3, log_base.graph_type)
    print('log folder: ' + log_folder)
    dump_file = '%s/%s' % (log_folder, log_base.file_ex3_dump)

    if not os.path.exists(dump_file):
        x, y = parse_folder(log_folder)
        try:
            with open(dump_file, 'wb') as file:
                pickle.dump([x, y], file)
        except IOError as err:
            print(str(err))
    else:
        try:
            with open(dump_file, 'rb') as file:
                x, y = pickle.load(file)
        except IOError as err:
            print(str(err))

    draw(x, y)

if __name__ == '__main__':
    log_base.graph_type = 'PA'

    parse_log()
