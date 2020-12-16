#include <iostream>
#include <stdio.h>
#include <map>
#include <queue>
#include <bitset>
#include <vector>

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


void buildEncodings(Node * root, unsigned int encoding) {
    if (root->isLeaf) {
        // std::cout << (char) root->value << " " << std::bitset<32>(encoding) << std::endl;
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

/* Read input file */
void * readInput(FILE *inputFile) {
    unsigned long inputFileSize = 0;
    fseek(inputFile, 0, SEEK_END);
    inputFileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
    void *buffer = malloc(inputFileSize);
    fread(buffer, 1, inputFileSize, inputFile);
    return buffer;
}

std::map<char, unsigned int> freqency;
std::map<char, Node *> referece;
std::priority_queue<Node*, std::vector<Node*>, Compare> queue;

int main()
{

    FILE *inputFile = fopen("input.txt", "rb");
    FILE *outputFile = fopen("output.txt", "wb");

    char *buffer = (char *)readInput(inputFile);
    int inputSize = strlen(buffer);


    for (int i = 0; i < inputSize; i++)
        freqency[buffer[i]] += 1;

    // create references and add to the queue
    for (const auto &[k, v] : freqency) {
        // add to reference map
        // std::cout << "adding" << " " << k << " with " << v << " value" << std::endl;
        Node * node = new Node(v, k);
        node->isLeaf = true;
        queue.emplace(node);
        referece[k] = node;
    }

    while (queue.size()) {
        Node * first = queue.top();
        queue.pop();
        Node * second = queue.top();
        queue.pop();

        std::cout << first->count << " " << second->count << std::endl;

        Node * n = new Node(first->count + second->count, 0xFF & (first->value + second->value));
        n->isLeaf = false;
        n->left = first;
        n->right = second;

        queue.emplace(n);
    }

    std::cout << "Here" << std::endl;
    Node * root = queue.top();
    int treeSize = getTreeSize(root);

    std::cout << "size of tree is " << treeSize << " bytes" << std::endl;

    /* 1111111111111(0)....whatever is here .... is good */
    buildEncodings(root, INT_MAX & ~1);

    /* write the size of tree as an int */
    fwrite(reinterpret_cast<const char *>(&treeSize), sizeof(int), 1, outputFile);

    /* This writes the huffman encoding in 4 bytes */
    for (int i = 0; i < inputSize; i++) {
        std::cout << std::bitset<32>(referece[buffer[i]]->code) << std::endl;
        fwrite(reinterpret_cast<const char *>(&referece[buffer[i]]->code), sizeof(int), 1, outputFile);
    }

    return 0;
}