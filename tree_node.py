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