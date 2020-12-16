#include <iostream>
#include <map>
#include <queue>
#include <bitset>
#include <vector>
#include "node.h"

void buildEncodings(Node * root, std::string encoding) {
    if (root->isLeaf) {
        root->code = encoding;
        return;
    }
    if (root->left != NULL) buildEncodings(root->left, encoding + "0");
    if (root->right != NULL) buildEncodings(root->right, encoding + "1");
}

int getTreeSize (Node * root) {
    if (root->isLeaf)
        return 2; // 2 bytes per node
    return 1 + getTreeSize(root->left) + getTreeSize(root->right);
}

void writeTreeToFile (Node * root, FILE * outFile) {
    if (root->isLeaf) {
        fwrite("0", 1, 1, outFile);
        fwrite(&root->value, 1, 1, outFile);
    } else {
        writeTreeToFile(root->left, outFile);
        writeTreeToFile(root->right, outFile);
        fwrite("1", 1, 1, outFile);
    }
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
    FILE *outputFile = fopen("output.boris", "wb");

    char *buffer = (char *)readInput(inputFile);
    int inputSize = strlen(buffer);


    for (int i = 0; i < inputSize; i++)
        freqency[buffer[i]] += 1;

    // create references and add to the queue
    for (const auto &[k, v] : freqency) {
        // add to reference map
        Node * node = new Node(v, k);
        node->isLeaf = true;
        queue.emplace(node);
        referece[k] = node;
    }

    while (queue.size() >= 2) {
        Node * first = queue.top();
        queue.pop();
        Node * second = queue.top();
        queue.pop();

        Node * n = new Node(first->count + second->count, 0xFF & (first->value + second->value));
        n->isLeaf = false;
        n->left = first;
        n->right = second;

        queue.emplace(n);
    }

    Node * root = queue.top();
    int treeSize = getTreeSize(root);

    // std::cout << treeSize << std::endl;

    buildEncodings(root, "");

    /* write the size of tree as an int */
    fwrite(reinterpret_cast<const char *>(&treeSize), sizeof(int), 1, outputFile);
    /* write tree to file */
    writeTreeToFile(root, outputFile);

    /* This writes the huffman encoding in 4 bytes */
    unsigned int * write_value = (unsigned int *) malloc(sizeof(unsigned int));
    
    unsigned char write = 0;
    int written = 0;

    for (int i = 0; i < inputSize; i++) {
        std::string code = referece[buffer[i]]->code;
        for (int j = 0; j < code.size(); j++) {
            /* write a byte at a time */
            if (written == 8) {
                fwrite(&write, sizeof(unsigned char), 1, outputFile);
                write = 0;
                written = 0;
            }
            write |= (code[j] == '1') << (7 - written);
            written++;
        }
    }

    /* write the rest */
    if (written < 8)
        fwrite(&write, sizeof(unsigned char), 1, outputFile);

    fclose(inputFile);
    fclose(outputFile);
    free(buffer);

    return 0;
}