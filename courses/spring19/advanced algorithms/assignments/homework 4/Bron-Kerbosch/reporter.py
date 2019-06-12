# coding: utf-8

class Reporter(object):
    def __init__(self, name):
        self.name = name
        self.cnt = 0
        self.mis = []
 
    def inc_count(self):
        self.cnt += 1
 
    def record(self, mis):
        self.mis.append(mis)
 
    def print_report(self):
        print self.name
        print '%d recursive calls' % self.cnt
        for i, m in enumerate(self.mis):
            print '%d: %s' % (i, m)
        print
