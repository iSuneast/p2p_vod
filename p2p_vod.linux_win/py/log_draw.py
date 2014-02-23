__author__ = 'wbtang'

import os
import log_base

from matplotlib.figure import Figure
from matplotlib.axes import Axes
from matplotlib.backends.backend_agg import FigureCanvasAgg

def __show_image(image_file):
    abspath = log_base.get_full_path(image_file)
    cmd = '"%s"' % abspath
    print('show image: %s' % cmd)
    os.system(cmd)

def get_xy_from_map(map):
    x = [key for key in map.keys()]
    x.sort()
    y = [map[key] for key in x]
    return x, y

def get_figure_path(figures):
    return [ ('%s/%s%s' % (log_base.folder_graph(), figures[i], log_base.file_figure_extend))
             for i in range(len(figures)) ]

def get_prefix_sum(ls):
    s = 0.
    ret = []
    for i in range(len(ls)):
        s += ls[i]
        ret.append(s)
    return ret

def plot(ax, x, y):
    ax.grid(True)
    ax.plot(x, y, marker='o')

def plot_ex(ax, x, y):
    plot(ax, x, y)

    scale = 0.05
    ex_x = (max(x)-min(x)) * scale
    ax.set_xlim(left=min(x)-ex_x, right=max(x)+ex_x)
    ex_y = (max(y)-min(y)) * scale
    ax.set_ylim(bottom=min(y)-ex_y, top=max(y)+ex_y)

def draw(figure_name, title, x, y, x_label, y_label, plot_=plot):
    fig = Figure(figsize=[5, 4])
    fig.suptitle(title)

    ax = Axes(fig, [0.15, 0.12, 0.8, 0.8])
    plot_(ax, x, y)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.legend()
    fig.add_axes(ax)

    canvas = FigureCanvasAgg(fig)
    canvas.print_figure(figure_name)
    __show_image(figure_name)

def draw_all(figures, titles, x, y, x_labels, y_labels, plot_=plot):
    for gid in range(len(x)):
        draw(figures[gid],
             titles[gid],
             x[gid], y[gid],
             x_labels[gid], y_labels[gid],
             plot_)
