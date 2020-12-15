from helpers import Node, create_encodings, write_tree
from helpers import create_encodings
from lz77 import lz77
from heapq import heappop, heappush
from collections import Counter

def huffman(inp: bytes, file) -> bytes:
    ''' declare all the important stuff '''
    size = len(inp)
    counts = Counter(inp)
    reference = {}
    heap = []

    # create priority queue based on the frequencies
    # of nodes.
    for byteCode in counts:
        node = Node(counts[byteCode], byteCode, None, None, True)
        reference[byteCode] = node
        heappush(heap, (counts[byteCode], node))

    while len(heap) >= 2:
        freq1, node1 = heappop(heap)
        freq2, node2 = heappop(heap)

        order = freq1 < freq2
        if freq1 == freq2:
            order = node1.value >= node2.value
        
        joinedFreq = freq1 + freq2
        joinedValue = node1.value + node2.value if order else node2.value + node1.value

        node = Node(joinedFreq, joinedValue, node1 if order else node2, node2 if order else node1)
        heappush(heap, (joinedFreq, node))

    totalBytes, root = heappop(heap)
    create_encodings(root, b'')

    treeBytes = write_tree(root)
    # 3 bytes per node, not very good.
    encoded = b''

    # begin writing to file
    file.write(len(treeBytes).to_bytes(4, 'little'))
    for byte in inp:
        encoded += reference[byte].code
    file.write(len(encoded).to_bytes(4, 'little'))
    # print(len(treeBytes), len(encoded))
    file.write(treeBytes)
    # print(treeBytes)
    file.write(encoded)


f = open('file.txt', 'rb')
s = lz77(bytes(f.read()))
print(s)
# print(str(s, 'utf-8'))
tree = huffman(s, open('out.boris', 'wb'))
# print(tree)