#!/usr/bin/env Python3

'''
    1. get huffman encoding to work
'''

class Node:
    def __init__(self, prob, value, left = None, right = None, leaf = False):
        self.prob = prob
        self.value = value

        self.left = left
        self.right = right

        self.leaf = leaf
    
    def __lt__(self, o):
        return self.prob < o.prob
    
    def __str__(self):
        return f'{self.value} ({self.prob})'

def create_encodings(root, running):
    if root.leaf:
        root.code = running
        return
    create_encodings(root.left, f'{running}0')
    create_encodings(root.right, f'{running}1')

from heapq import heappop, heappush
from collections import Counter

f = open('file.txt', 'r')

contents = f.read()

nodes = Counter(contents)
total = len(contents)
heap = []
reference = {}

for node in nodes:
    p = nodes[node] / total
    n = Node(p, node, None, None, True)
    heappush(heap, (p, n))
    reference[node] = n

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

# at this stage we should create the encodings for each leaf
create_encodings(root, '')

#attempt to encode string
out = ''
for c in contents:
    out += reference[c].code
print(out)

