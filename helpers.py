class Node:
    def __init__(self, count, value, left = None, right = None, leaf = False):
        self.count = count
        self.value = value

        self.left = left
        self.right = right

        self.leaf = leaf
    
    def __lt__(self, o):
        return self.count < o.count

# creates the huffman encodings for all leaves of the tree
def create_encodings(root: Node, running: bytes):
    if root.leaf:
        root.code = running
    else:
        # this is kind of bad...1 byte for every bit ...
        create_encodings(root.left, running + b'0')
        create_encodings(root.right, running + b'1')

# post-order traversal
def write_tree(root: Node):
    if root.leaf:  return (root.value << 1).to_bytes(3, 'little')
    else: return write_tree(root.left) + write_tree(root.right) + (1).to_bytes(3, 'little') # 3 bytes for now