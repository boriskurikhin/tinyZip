#!/usr/bin/env Python3

from tree_node import Node
from tree_node import create_encodings

f = open('out.boris', 'rb')

# bytes_req = int.from_bytes(f.read(4), byteorder='little')
tree_bytes = int.from_bytes(f.read(4), byteorder='little')
enc_size = int.from_bytes(f.read(4), byteorder='little')

stack = []

i = 0
# read tree
while i < tree_bytes:
    node_type = int.from_bytes(f.read(4), byteorder='little')
    if node_type == 0:
        v = str(bytes(f.read(1)), 'utf-8')
        i += 1
        stack.append(Node(0, v, None, None, True))
    else:
        right = stack.pop()
        left = stack.pop()

        n = Node(0, '', left, right)
        stack.append(n)

    i += 1

root = stack.pop()
out_file = open('out.txt', 'w')

create_encodings(root, '')

i = 0

cur = root
while i <= enc_size:
    if cur.leaf:
        out_file.write(cur.value)
        cur = root
        continue
    x = bytes(f.read(1))
    if x == b'0': cur = cur.left
    elif x == b'1': cur = cur.right
    i += 1
