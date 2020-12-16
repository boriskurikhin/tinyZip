#include <stdio.h>

class Node {
    public:
        unsigned int count;
        bool isLeaf;
        Node * left;
        Node * right;
        char value;
        unsigned int code;
        
        Node(unsigned int c, char v) {
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
