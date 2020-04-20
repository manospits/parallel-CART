#ifndef _TREE_
#define _TREE_

typedef struct TreeNode{
    int val;//temp TODO find CART node vars
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

typedef struct TreeNode * PTreeNode;

void insert_left(PTreeNode root, PTreeNode child);
void insert_right(PTreeNode root, PTreeNode child);
PTreeNode create_tree_node(int value);

void free_tree(PTreeNode node);

#endif
