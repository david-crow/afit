# coding: utf-8
 
from data import NEIGHBORS
MIN_SIZE = 2

def bron_kerbosch2(mis, candidates, excluded, reporter):
    '''Bron–Kerbosch algorithm with pivot'''
    print("Pivoting:    ", mis)
    reporter.inc_count()
    if not candidates and not excluded:
        if len(mis) >= MIN_SIZE:
            reporter.record(mis)
            print("Pivoting MIS:", mis)
        return
 
    pivot = pick_random(candidates) or pick_random(excluded)
    for v in list(candidates.difference(NEIGHBORS[pivot])):
        new_candidates = candidates.intersection(NEIGHBORS[v])
        new_excluded = excluded.intersection(NEIGHBORS[v])
        bron_kerbosch2(mis + [v], new_candidates, new_excluded, reporter)
        candidates.remove(v)
        excluded.add(v)

def pick_random(s):
    if s:
        elem = s.pop()
        s.add(elem)
        return elem
