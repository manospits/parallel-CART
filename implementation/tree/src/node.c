#include "../include/node.h"
#include <stdlib.h>

Node create_node() {
    Node node;
    node = malloc(sizeof(Node_));
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_nodes(Node node){
    if (!node) return;
    if (node->left)
        free_nodes(node->left);
    if (node->right)
        free_nodes(node->right);
    free(node);
}
