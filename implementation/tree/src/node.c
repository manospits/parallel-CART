#include "../include/node.h"
#include "../../io/include/io.h"
#include <stdlib.h>

Node create_node() {
    Node node;
    node = malloc(sizeof(Node_));
    node->left = NULL;
    node->right = NULL;
    node->split_val = NULL;
    node->most_common = NULL;
    return node;
}

void del_node(Node node){
    if(node){
        free_subset_dataset(node->subset);

    }
    if(node->most_common)
        free(node->most_common);
    free(node);
}
void free_nodes(Node node){
    if (!node) return;
    if (node->left)
        free_nodes(node->left);
    if (node->right)
        free_nodes(node->right);
    del_node(node);
}
