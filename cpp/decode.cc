#include <iostream>
#include <stdio.h>
#include <map>
#include <queue>
#include <bitset>
#include <vector>
#include <stack>

class Node {
    public:
        unsigned int count;
        bool isLeaf;
        Node * left;
        Node * right;
        char value;
        unsigned int code;
        
        Node(int c, char v) {
            count = c;
            value = v;
            left = NULL;
            right = NULL;
            isLeaf = false;
        }
};

struct Compare {
    bool operator()(Node * a, Node * b) {
        return a->count > b->count;
    }
};

std::map<unsigned int, Node*> ref;

void buildEncodings(Node * root, unsigned int encoding) {
    if (root->isLeaf) {
        // std::cout << (char) root->value << " " << std::bitset<32>(encoding) << std::endl;
        ref[encoding] = root;
        root->code = encoding;
        return;
    }
    if (root->left != NULL) buildEncodings(root->left, encoding << 1);
    if (root->right != NULL) buildEncodings(root->right, (encoding << 1) | 1);
}

int getTreeSize (Node * root) {
    if (root->isLeaf)
        return 4 + 1; // 4 bytes per node + 1 byte for indicator
    return 1 + getTreeSize(root->left) + getTreeSize(root->right);
}

std::stack<Node*> stack;


int main()
{

    FILE *inputFile = fopen("output.boris", "rb");
    int fileSize;

    fseek(inputFile, 0L, SEEK_END);
    fileSize = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);


    /* read the encoded file */
    char buffer[fileSize];
    fread(buffer, 1, fileSize, inputFile);

    // first 4 bytes represent the tree size
    int treeSize = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    treeSize += 4;
    int i = 4;

    /* Start reading in the tree */
    /* And re-building it */

    while (i < treeSize) {
        std::cout << i << std::endl;
        if (buffer[i] == '0') {
            // 2 bytes
            Node * leaf = new Node(0, buffer[i + 1]);
            leaf->isLeaf = true;
            
            stack.push(leaf);
            i += 2;
        } else if (buffer[i] == '1') {
            //1 byte
            Node * right = stack.top();
            stack.pop();
            Node * left = stack.top();
            stack.pop();

            Node * parent = new Node(0, -1);
            parent->left = left;
            parent->right = right;
            
            stack.push(parent);
            i += 1;
        }
    }

    Node * root = stack.top();
    stack.pop();

    buildEncodings(root, INT_MAX & ~1);

    while (i < fileSize) {
        int code = buffer[i + 0] | (buffer[i + 1] << 8) | (buffer[i + 2] << 16) | (buffer[i + 3] << 24);
        std::cout << ref[code]->value;
        i += 4;
    }

    std::cout << std::endl;

   

    return 0;
}