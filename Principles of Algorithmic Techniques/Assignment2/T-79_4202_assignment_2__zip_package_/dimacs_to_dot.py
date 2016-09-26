#! /usr/bin/env python
# -*- coding: iso-8859-1 -*-

# A script for visualizing a directed graph given in DIMACS format.
# Usage: python dimacs_to_dot.py <my.graph | dotty -

header = raw_input().split()
n_vertices = int(header[2])
n_edges = int(header[3])
print 'digraph mygraph {'
for i in range(n_vertices):
    print '  n%d [label="%d"];' % (i+1, i+1)
for i in range(n_edges):
    (e, u, v) = raw_input().split()
    print '  n%s -> n%s;' % (u, v)
print '}'
