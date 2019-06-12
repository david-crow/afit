# coding: utf-8
 
from data import NEIGHBORS
MIN_SIZE = 2

def bron_kerbosch1(mis, candidates, excluded, reporter):
    '''Naive Bron–Kerbosch algorithm'''
    print("Naive:    ", mis)
    reporter.inc_count()
    if not candidates and not excluded:
        if len(mis) >= MIN_SIZE:
            reporter.record(mis)
            print("Naive MIS:", mis)
        return
 
    for v in list(candidates):
        new_candidates = candidates.intersection(NEIGHBORS[v])
        new_excluded = excluded.intersection(NEIGHBORS[v])
        bron_kerbosch1(mis + [v], new_candidates, new_excluded, reporter)
        candidates.remove(v)
        excluded.add(v)
