#include <stdio.h>
#include <stdlib.h>
#include "../include/node.h"
#include "../include/tree.h"
#include "../../io/include/io.h"
#include "string.h"

double calc_entropy() {
    double entropy = 0;

    return entropy;
}

double calc_gini_coefficient() {
    double gini_coefficient = 0;

    return gini_coefficient;
}

void find_best_split() {

    /*return best_split;*/
}

Tree build_classification_tree(Dataset train_dataset, char *class_field) {
    Tree clf_tree = create_tree(train_dataset, class_field);
    //add root node with all training data instances

    return clf_tree;
}

double classification_predict() {

    return 0.0;
}

void build_regression_tree() {

}

double regression_predict() {

    return 0.0;
}

Tree create_tree(Dataset train_dataset, char *class_field){
    Tree tmp_tree = malloc(sizeof(Tree_));
    tmp_tree->root = NULL;
    tmp_tree->train_dataset = train_dataset;
    strcpy(tmp_tree->predict_field, class_field);
    return tmp_tree;
}

void del_tree(Tree tree){
    printf("Freeing tree\n");
    free_nodes(tree->root);
    free(tree);
}


