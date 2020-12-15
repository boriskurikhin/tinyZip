#!/usr/bin/env Python3

'''
    1. get huffman encoding to work
'''
from helpers import Node, create_encodings, write_tree
from helpers import create_encodings
# bytes_required = 1
import math

from heapq import heappop, heappush
from collections import Counter

f = open('test.mp4', 'r')

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
assert _ == 1, 'Probabilities did not add up to 1'

# at this stage we should create the encodings for each leaf
create_encodings(root, '')
# print(bytes_required)

#attempt to encode string
f = open('out.boris', 'wb')
# total number of characters in uncompressed file
# f.write(total.to_bytes(4, 'little'))

''' header '''
# first 4 bytes will be how many bytes are required per encoding
# f.write(bytes_required.to_bytes(4, 'little'))
# write the huffman-encoded string
tree_bytes = write_tree(root, f)
encoded = b''
for c in contents:
    encoded += bytes(reference[c].code, 'utf-8')
f.write(len(tree_bytes).to_bytes(4, 'little'))
f.write(len(encoded).to_bytes(4, 'little'))

# write pre-order tree
for b in tree_bytes: f.write(b)
f.write(encoded)
