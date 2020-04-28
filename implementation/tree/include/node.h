#ifndef _NODE_
#define _NODE_
#include "../../io/include/io.h"

typedef struct Node_ {
    struct Node_ *left;
    struct Node_ *right;
    char * most_common;
    char * split_val;
    Dataset subset;
    Attribute * attr;
    int key;
    int isleaf;
} Node_;

typedef Node_ * Node;

Node create_node();
void del_node(Node node);
void insert_left(Node root, Node child);
void insert_right(Node root, Node child);
void free_nodes(Node node);

#endif
