__author__ = 'wbtang'

import log_base
import log_draw

labels = [
    'selfish',
    'unselfish'
]

def plot(ax, x, y):
    ax.grid(True)
    ax.scatter(x, y, marker='o')

    # line
    bound = max(max(x), max(y)) * 1.05
    ax.plot([0, bound], [0, bound], linestyle='--', color='r')

    ax.set_xlim(left=0, right=bound)
    ax.set_ylim(bottom=0, top=bound)

def draw(x, y):
    figures = log_draw.get_figure_path([
        '%s_os_latency' % (log_base.graph_type) ,
        '%s_os_fluency' % (log_base.graph_type)
    ])
    titles = [
        '%s: latency' % (log_base.graph_type) ,
        '%s: fluency' % (log_base.graph_type)
    ]
    x_labels = [
        'unselfish',
        'unselfish'
    ]
    y_labels = [
        'selfish',
        'selfish'
    ]

    log_draw.draw_all(figures, titles, x, y, x_labels, y_labels, plot_=plot)

def parse_two_world(file):
    selfish_latency = []
    selfish_smoothness = []
    social_latency = []
    social_smoothness = []

    for line in file:
        tokens = line.strip().split('\t')
        data = log_base.PeerData(tokens[1:])

        if tokens[0] == labels[0]:
            selfish_latency.append(data.latency)
            selfish_smoothness.append(data.smoothness/log_base.scale)
        else:
            social_latency.append(data.latency)
            social_smoothness.append(data.smoothness/log_base.scale)

    draw(
        [social_latency, social_smoothness],
        [selfish_latency, selfish_smoothness]
    )

def parse_log():
    log_file = '%s/%s/%s' % (log_base.folder_final_ex2, log_base.graph_type, log_base.file_ex2_log)
    print('log file: ' + log_file)

    try:
        with open(log_file, 'r') as file:
            parse_two_world(file)
    except IOError as err:
        print(err)

if __name__ == '__main__':
    log_base.graph_type = ''

    parse_log()
