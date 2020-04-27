#include <stdio.h>
#include <stdlib.h>
#include "../include/node.h"
#include "../include/tree.h"
#include "../../data_structures/include/list.h"
#include "../../data_structures/include/element.h"
#include "../../io/include/io.h"
#include "string.h"

double calc_entropy() {
    double entropy = 0;

    return entropy;
}

/*def gini(rows):*/
    /*total = len(rows)*/
    /*counts = uniqueCounts(rows)*/
    /*imp = 0.0*/

    /*for k1 in counts:*/
        /*p1 = float(counts[k1])/total*/
        /*for k2 in counts:*/
            /*if k1 == k2: continue*/
            /*p2 = float(counts[k2])/total*/
            /*imp += p1*p2*/
    /*return imp*/

double calc_gini_coefficient(Dataset dt, Attribute * class_attr) {
    int total = dt -> rows;
    double imp=0.0;

    phead counts=unique_counts(dt, class_attr);
    pnode iterator_i = get_list(counts);

    for(int i=0; i<get_size(counts); i++ ){
        value_count * label_count_i = ((value_count *) ret_data(iterator_i));
        double p1 = (double) label_count_i->count/(double) total;
        pnode iterator_j = get_list(counts);
        for(int j=0; j<get_size(counts); j++ ){
            value_count * label_count_j = ((value_count *) ret_data(iterator_j));
            if(strcmp(label_count_i->value, label_count_j->value)){
                double p2 = (double) label_count_j->count/ (double) total;
                imp += p1*p2;
            }
            iterator_j = next_node(iterator_j);
        }
        iterator_i = next_node(iterator_i);
    }

    ds_list_plus_data(counts, &free);

    return imp;
}

Tree build_classification_tree(Dataset train_dataset, char *class_field) {
    Tree clf_tree = create_tree(train_dataset, class_field);
    Attribute *class_attr;
    for(int i=0;i< train_dataset->attributes_number; i++){
        if(strcmp(train_dataset->attributes[i].name, class_field)==0){
            class_attr=&(train_dataset->attributes[i]);
        }
    }
    clf_tree->predict_attribute=class_attr;
    strcpy(clf_tree->predict_field, class_field);
    /*double gini_imp;*/
    /*gini_imp = calc_gini_coefficient(train_dataset, class_attr);*/
    /*printf("Gini is %f\n", gini_imp);*/

    //start with all training data instances
    pel_info int_type = create_type(sizeof(int), &intcmp );
    phead rows_list=cr_list(int_type);
    for(int i = 0; i < train_dataset->rows; i++){
        insert(rows_list, &i);
    }
    Dataset subset = get_subset(train_dataset, rows_list);
    ds_list_plus_data(rows_list, &free);


    //call recursive function to build tree
    clf_tree->root=grow_tree(clf_tree, subset);

    return clf_tree;
}

Node grow_tree(Tree clf_tree, Dataset subset){
    if(subset->rows == 0) return NULL;
    Node current=create_node();
    current->subset = subset;
    double current_score = calc_gini_coefficient(current->subset, clf_tree->predict_attribute);
    printf("Gini is %f\n", current_score);
    double best_gain = 0.0;
    Attribute * best_attribute;
    Dataset best_set_left;
    Dataset best_set_right;

    for (int i=0; i < current->subset->attributes_number; i++){
        if(!strcmp(subset->attributes[i].name, clf_tree->predict_field)) continue;

        phead unique_col_i_values = unique_values(current->subset, &(subset->attributes[i]));
        pnode iterator_j = get_list(unique_col_i_values);
        for(int j = 0; j < get_size(unique_col_i_values); j++){
            //TODO CONTINUE MISERY
            iterator_j = next_node(iterator_j);
        }
        ds_list_plus_data(unique_col_i_values, &free);
    }
    return current;
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


