#ifndef _TREE_
#define _TREE_
#include "./node.h"
#include "../../io/include/io.h"

typedef struct Tree_{
    Node root;
    Dataset train_dataset;
    char predict_field[STRING_SIZE];
    Attribute * predict_attribute;
}Tree_;

typedef Tree_ * Tree;

double calc_entropy();

double calc_gini_coefficient();

void find_best_split();

Tree build_classification_tree(Dataset dataset, char *class_field);

char * traverse(Tree clf_tree, Node current, char * row);

char * predict_row(Tree clf_tree, char * row);

void divide_dataset(Dataset subset, int attribute_index, void * value, Dataset * leftSubset, Dataset * rightSubset);

Node grow_tree(Tree clf_tree, Dataset subset);

double classification_predict();

void build_regression_tree();

double regression_predict();

Tree create_tree(Dataset train_dataset, char *class_field);

void del_tree(Tree tree);

#endif
