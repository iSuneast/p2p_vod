__author__ = 'wbtang'

import queue
import log_draw
import log_base

twitter_ori = '../comm.txt'
twitter_out = '../twitter_graph.txt'

edges = {}

def clean(nodes, times):
    print('num of nodes: %d' % len(nodes))
    for i in range(times):
        nodes_to_del = []
        for node in nodes:
            if len(edges[node]) <= 2:
                nodes_to_del.append(node)
        print('round %d. num of nodes to del: %d' % (i+1, len(nodes_to_del)))
        if len(nodes_to_del) == 0:
            break
        for u in nodes_to_del:
            edge_u = [v for v in edges[u]]
            for v in edge_u:
                edges[v].remove(u)
            nodes.remove(u)
        print('num of nodes: %d' % len(nodes))

    return nodes

def find_connection(source):
    print('find_connection')
    nodes = set()
    q = queue.Queue()
    q.put(source)
    nodes.add(source)
    while not q.empty():
        u = q.get()
        for v in edges[u]:
            if v not in nodes:
                nodes.add(v)
                q.put(v)

    return clean(nodes, 15)

def verify_degree(nodes):
    degree = {}
    for u in nodes:
        edge_u = edges[u]
        num_edge = len(edge_u)
        if num_edge not in degree:
            degree[num_edge] = 1
        else:
            degree[num_edge] += 1

    print(degree)
    x = sorted([i for i in degree])
    y = [int(degree[i]) for i in x]

    log_draw.draw('%s/twitter_graph.jpg' % log_base.folder_final, 'twitter_graph',
                  x, y,
                  'degree ', 'num of nodees', plot_=log_draw.plot_ex)
    long = 100
    log_draw.draw('%s/twitter_graph_long.jpg' % log_base.folder_final, 'twitter_graph (degree > %d)' % long,
                  x[long:], y[long:],
                  'degree ', 'num of nodees', plot_=log_draw.plot_ex)

def claw_graph():
    all_nodes = [v for v in edges]
    nodes = find_connection(all_nodes[0])
    all_nodes = [v for v in nodes]
    nodes = find_connection(all_nodes[0])

    print('connected nodes: ', len(nodes))
    valid_edges = 0
    with open(twitter_out, 'w') as file:
        n = len(nodes)
        file.write('%d\n' % n)
        for u in nodes:
            for v in edges[u]:
                if u < v:
                    file.write('%d %d\n' % (u, v))
                    valid_edges += 1
    print('num of nodes %d' % n)
    print('num of edges %d' % valid_edges)
    print('done')

    verify_degree(nodes)

fa = {}

def find_fa(u):
    if fa[u] == -1:
        return u
    else:
        fa[u] = find_fa(fa[u])
        return fa[u]

def union(u, v):
    ancestor_u = find_fa(u)
    ancestor_v = find_fa(v)
    if ancestor_u != ancestor_v:
        fa[ancestor_u] = ancestor_v

def verify_connection():
    for i in edges:
        fa[i] = -1
    for u in edges:
        for v in edges[u]:
            union(u, v)
    cc = {find_fa(i) for i in edges}
    print('cc = ', len(cc))

def add_edge(u, v):
    if u == v:
        return
    if u not in edges:
        edges[u] = set()
    if v not in edges:
        edges[v] = set()
    edges[u].add(v)
    edges[v].add(u)

with open(twitter_ori) as file:
    for line in file:
        if not line[0].isdigit():
            continue
        tokens = line.strip().split('\t')
        u = int(tokens[0])
        v = int(tokens[1])
        add_edge(u, v)

claw_graph()

