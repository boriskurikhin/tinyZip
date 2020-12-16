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
    int i = 4;

    /* Start reading in the tree */
    /* And re-building it */

    while (i - 4 < treeSize) {
        if (buffer[i] == '0') {
            // 2 bytes
            std::cout << "creating child " << buffer[i + 1] << std::endl;
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

    std::cout << "Here" << std::endl;

    Node * root = stack.top();
    stack.pop();

    buildEncodings(root, INT_MAX & ~1);

    while (i < fileSize) {
        unsigned int code;
        fseek(inputFile, 0L + i, SEEK_SET);
        fread(&code, sizeof(unsigned int), 1, inputFile);
        std::cout << ref[code]->value;
        i += 4;
    }

    std::cout << std::endl;

   

    return 0;
}