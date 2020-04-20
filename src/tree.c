#include <stdio.h>
#include <stdlib.h>
#include "./include/tree.h"

PTreeNode create_tree_node(int value){
    PTreeNode tmp;
    tmp = malloc(sizeof(TreeNode));
    tmp->val=value;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void insert_left(PTreeNode root, PTreeNode child){
    root->left=child;
}

void insert_right(PTreeNode root, PTreeNode child){
    root->right=child;
}

void free_tree(PTreeNode node){
    if (node->left)
        free_tree(node->left);
    if (node->right)
        free_tree(node->right);
    printf("Freeing node with value %d. \n",node->val);
    free(node);
}
