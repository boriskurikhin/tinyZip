#!/usr/bin/env Python3

from helpers import Node
from helpers import create_encodings
from lz77 import lz77rev

f = open('out.boris', 'rb')

# bytes_req = int.from_bytes(f.read(4), byteorder='little')
treeBytes = int.from_bytes(f.read(4), byteorder='little')
encodedSize = int.from_bytes(f.read(4), byteorder='little')

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
out_file = open('out.txt', 'wb')
create_encodings(root, b'')

output = b''

i = 0
cur = root
while i <= encodedSize:
    if cur.leaf:
        # print(cur.value)
        output += cur.value.to_bytes(3, 'big')
        cur = root
        continue
    x = bytes(f.read(1))
    if x == b'0': cur = cur.left
    elif x == b'1': cur = cur.right
    i += 1

# print(output)
out_file.write(lz77rev(output))

