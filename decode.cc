#include <iostream>
#include <map>
#include <queue>
#include <bitset>
#include <stack>
#include <string>
#include "node.h"

void undoLZ77 (char * input, int fileSize, FILE * output ) {
    int index = 0, j = 0;
    char buffer[fileSize];
    std::string running = "";

    while (index < fileSize) {
        int relativePos = input[index] | ((input[index+1] & 0x0F) << 8);
        int matchLength = (input[index + 1] & 0xF0) >> 4;
        char nextChar = input[index + 2];

        std::cout << running << std::endl;
        std::cout<< "printing " << relativePos << " " << matchLength << " done" << std::endl;

        if (matchLength) {
            int subsrIndex = j - relativePos;
            std::cout << running << std::endl;
            std::cout << running.size() - relativePos << std::endl;
            std::string ss = running.substr(running.size() - relativePos, matchLength);
            fwrite(ss.c_str(), matchLength, 1, output);
            running += ss;
            j += matchLength;
        } else if (nextChar) {
            fwrite(&nextChar, 1, 1, output);
            running += nextChar;
            j++;
        }

        index += 3;
    }
}

void buildEncodings(Node * root, std::string encoding) {
    if (root->isLeaf) {
        root->code = encoding;
        std::cout << encoding << std::endl;
        return;
    }
    if (root->left != NULL) buildEncodings(root->left, encoding + "0");
    if (root->right != NULL) buildEncodings(root->right, encoding + "1");
}

std::stack<Node*> stack;

int main()
{

    FILE *inputFile = fopen("output.b", "rb");
    FILE *ouputFile = fopen("output.txt", "wb");

    int fileSize;

    fseek(inputFile, 0L, SEEK_END);
    fileSize = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);

    /* read the encoded file */
    char buffer[fileSize];
    fread(buffer, 1, fileSize, inputFile);

    undoLZ77(buffer, fileSize, ouputFile);


    return 0;

    

    // first 4 bytes represent the tree size
    int treeSize;
    fseek(inputFile, 0L, SEEK_SET);
    fread(&treeSize, sizeof(int), 1, inputFile);
    std::cout << treeSize << std::endl;
    int i = 4;

    /* Start reading in the tree */
    /* And re-building it */

    while (i - 4 < treeSize) {
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

    buildEncodings(root, "");

    unsigned char read = 0;
    int charsRead = 0;

    Node * cur = root;

    while (i < fileSize) {
        /* go to line */
        fseek(inputFile, 0L + i, SEEK_SET);
        fread(&read, sizeof(unsigned char), 1, inputFile);

        for (int j = 0; j < 8; j++) {
            
            if (cur->isLeaf) {
                fwrite(&cur->value, 1, 1, ouputFile);
                cur = root;
            }

            // move in the tree 
            if (read & (1 << (7 - j))) cur = cur->right;
            else cur = cur->left;

            if (cur->isLeaf) {
                fwrite(&cur->value, 1, 1, ouputFile);
                cur = root;
            }

        }

        i += 1;
    }

    fclose(inputFile);
    fclose(ouputFile);

    return 0;
}