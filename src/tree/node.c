#include "node.h"

Node *create_node(Node *left, Node *right, int key, int isleaf) {
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    node->left = left;
    node->right = right;
    node->key = key;
    node->isleaf = isleaf;
    return node;
}