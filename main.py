#!/usr/bin/env Python3

'''
    1. get huffman encoding to work
'''

class Node:
    def __init__(self, prob, value, left = None, right = None):
        self.prob = prob
        self.value = value

        self.left = left
        self.right = right
    
    def __lt__(self, o):
        return self.prob < o.prob
    
    def __str__(self):
        return f'{self.value} ({self.prob})'

from heapq import heappop, heappush
from collections import Counter

f = open('file.txt', 'r')

contents = f.read()

nodes = Counter(contents)
total = len(contents)
heap = []

for node in nodes:
    p = nodes[node] / total
    heappush(heap, (p, Node(p, node)))

while len(heap) >= 2:
    p1, n1 = heappop(heap)
    p2, n2 = heappop(heap)

    # left vs right child
    order = n1.prob < n2.prob
    # lexicographic if same probability
    if abs(n1.prob - n2.prob) < 1e-20:
        order = n1.value >= n2.value
    
    p = p1 + p2
    v = n1.value + n2.value if order else n2.value + n1.value

    parent = Node(p, v, n1 if order else n2 , n2 if order else n1)
    heappush(heap, (p, parent))

_, root = heappop(heap)
assert _ == 1, 'Something broke'













