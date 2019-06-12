# coding: utf-8

from data import NEIGHBORS
from bron_kerbosch2 import bron_kerbosch2
MIN_SIZE = 2

def bron_kerbosch3(mis, candidates, excluded, reporter):
    '''Bron–Kerbosch algorithm with pivot and degeneracy ordering'''
    print("Pivoting, ordering:    ", mis)
    reporter.inc_count()
    if not candidates and not excluded:
        if len(mis) >= MIN_SIZE:
            reporter.record(mis)
            print("Pivoting, ordering MIS:", mis)
        return
 
    for v in list(degeneracy_order(candidates)):
        new_candidates = candidates.intersection(NEIGHBORS[v])
        new_excluded = excluded.intersection(NEIGHBORS[v])
        bron_kerbosch2(mis + [v], new_candidates, new_excluded, reporter)
        candidates.remove(v)
        excluded.add(v)

def degeneracy_order(nodes):
    deg = {}
    for node in nodes:
        deg[node] = len(NEIGHBORS[node])
 
    while deg:
        i, v = min(deg.iteritems(), key=lambda (i, v): v)
        yield i
        del deg[i]
        for v in NEIGHBORS[i]:
            if v in deg:
                deg[v] -= 1
