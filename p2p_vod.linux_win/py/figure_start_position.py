__author__ = 'wbtang'

import random_seek_distribution
import log_base
import log_draw

def plot(ax,  x, y):
    ax.set_ylim(top = 1.1)
    ax.grid(True)
    ax.plot(x, y, marker='.')

cdf = random_seek_distribution.distribution.strip().split('\n')
figure_name = '%s/%s' % (log_base.folder_final, 'cdf_start_position.jpg')
title = 'The CDF of Start Position'
x_label = 'Position'
y_label = 'CDF'
x = [i for i in range(len(cdf))]
y = [(float(cdf[i]) * 0.5 + 0.5) for i in range(len(cdf))]

log_draw.draw(figure_name, title, x, y, x_label, y_label, plot_=plot)
