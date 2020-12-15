#!/usr/bin/env Python3

from helpers import Node
from helpers import create_encodings

f = open('out.boris', 'rb')

# bytes_req = int.from_bytes(f.read(4), byteorder='little')
treeBytes = int.from_bytes(f.read(4), byteorder='little')
encodedSize = int.from_bytes(f.read(4), byteorder='little')

stack = []
i = 0
# read tree
while i < treeBytes:
    buffer = f.read(3)
    check = buffer[0] & 1
    if check == 0:
        print('here 1')
        i += 1
        print(bin(int.from_bytes(buffer[1:], 'little')))
        stack.append(Node(0, int.from_bytes(buffer, 'little'), None, None, True))
    else:
        print('here 2')
        right = stack.pop()
        left = stack.pop()
        n = Node(0, '', left, right)
        stack.append(n)
    i += 1
root = stack.pop()
# out_file = open('out.txt', 'w')

# create_encodings(root, 0)

# i = 0

# cur = root
# print(root.left.value)
# while i <= encodedSize:
#     if cur.leaf:
#         out_file.write(chr(cur.value))
#         cur = root
#         continue
#     buffer = f.read(3)
#     node = int.from_bytes(buffer, 'big') & (1 << 23)
#     if not node: cur = cur.left
#     else: cur = cur.right
#     i += 1
