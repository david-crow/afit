# coding: utf-8

import time

from bron_kerbosch1 import bron_kerbosch1
from bron_kerbosch2 import bron_kerbosch2
from bron_kerbosch3 import bron_kerbosch3
from data import *
from reporter import Reporter

def complement():
    inverse = NEIGHBORS[:]

    for i in range(1, len(inverse)):
        for j in NODES:
            if j in inverse[i]:
                inverse[i].remove(j)
            else:
                inverse[i].append(j)
            if i == j:
                inverse[i].remove(j)

if __name__ == '__main__':
    funcs = [bron_kerbosch1,
             bron_kerbosch2,
             bron_kerbosch3]
    
    complement()
    start = time.time()

    for func in funcs:
        start = time.time()
        report = Reporter('## %s' % func.func_doc)
        func([], set(NODES), set(), report)
        end = time.time()
        print
        report.print_report()
        print("Execution time:", end - start)
        print
