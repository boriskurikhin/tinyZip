#!/usr/bin/env Python3

from helpers import Node
from helpers import create_encodings

f = open('out.boris', 'rb')

# bytes_req = int.from_bytes(f.read(4), byteorder='little')
treeBytes = int.from_bytes(f.read(4), byteorder='little')
encodedSize = int.from_bytes(f.read(4), byteorder='little')

ref = {}
stack = []
i = 0
# read tree
while i < treeBytes:
    buffer = f.read(3)
    check = int.from_bytes(buffer, 'little')
    if check != 1:
        stack.append(Node(0, check >> 1, None, None, True))
    else:
        right = stack.pop()
        left = stack.pop()
        n = Node(0, None, left, right)
        stack.append(n)
    i += 3

root = stack.pop()
out_file = open('out.txt', 'w')
create_encodings(root, 0, ref)
i = 0
cur = root
while i < encodedSize:
    buffer = int.from_bytes(f.read(3), 'little')
    out_file.write(chr(ref[buffer]))
    i += 3
