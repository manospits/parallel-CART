#ifndef _NODE_
#define _NODE_

typedef struct Node {
    struct Node *left;
    struct Node *right;
    int key;
    int isleaf;
} Node;

Node *create_node(int value);
void del_node(Node node);
void insert_left(Node root, Node child);
void insert_right(Node root, Node child);

#endif
