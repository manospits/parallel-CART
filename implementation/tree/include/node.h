#ifndef _NODE_
#define _NODE_

typedef struct Node_ {
    struct Node_ *left;
    struct Node_ *right;
    int key;
    int isleaf;
} Node_;

typedef Node_ * Node;

Node create_node(Node left, Node right, int key, int isleaf);
void del_node(Node node);
void insert_left(Node root, Node child);
void insert_right(Node root, Node child);
void free_nodes(Node node);

#endif
