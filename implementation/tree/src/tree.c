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

void divide_dataset(Dataset subset, int attribute_index, void * value, Dataset * leftSubset, Dataset * rightSubset){
    phead left;
    phead right;

    pel_info int_type = create_type(sizeof(int), &intcmp );
    left = cr_list(int_type);
    right = cr_list(int_type);
    for(int i = 0; i < subset->rows; i++){
	if (subset->attributes[attribute_index].dtype=='s'){
		char * value2 = subset->data[i]+subset->attributes[attribute_index].offset; 
        	if((subset->attributes[attribute_index].cmp(value, value2))>=0)
            		insert(right, &i);
        	else
            		insert(left, &i);

	}
	else{
		double value2 = *(double *) subset->data[i]+subset->attributes[attribute_index].offset;
		//printf("---- %f---\n", value2);
		if((subset->attributes[attribute_index].cmp(value, &value2 ))>=0)
		    insert(right, &i);
		else
		    insert(left, &i);
	}
    }
    printf("right %d left %d\n", get_size(right), get_size(left));
    *leftSubset = get_subset(subset, left);
    *rightSubset = get_subset(subset, right);
    ds_list_plus_data(left, &free);
    ds_list_plus_data(right, &free);
}

Node grow_tree(Tree clf_tree, Dataset subset){
    if(subset->rows == 0) return NULL;
    Node current=create_node();
    current->subset = subset;
    /*for(int i = 0; i < current->subset->rows; i++){*/
        /*for (int j =0; j < current->subset->attributes_number; j++){*/
            /*if(subset->attributes[j].dtype=='s')*/
                /*printf("%s ", current->subset->data[i]+current->subset->attributes[j].offset);*/
             /*if(subset->attributes[j].dtype=='d')*/
                /*printf("%f ",*(double*) current->subset->data[i]+current->subset->attributes[j].offset);*/
        /*}*/
        /*printf("\n");*/
    /*}*/
    double current_score = calc_gini_coefficient(current->subset, clf_tree->predict_attribute);
    printf("Gini is %f\n", current_score);
    double best_gain = 0.0;
    Attribute * best_attribute;
    Dataset best_set_left;
    Dataset best_set_right;
    phead unique_col_i_values;
    pnode iterator_j;
    for (int i=0; i < current->subset->attributes_number; i++){
        printf("Column: %s\n", subset->attributes[i].name);
        if(!strcmp(subset->attributes[i].name, clf_tree->predict_field)) continue;

        unique_col_i_values = unique_values(current->subset, &(current->subset->attributes[i]));
        iterator_j = get_list(unique_col_i_values);

        for(int j = 0; j < get_size(unique_col_i_values); j++){
            //TODO CONTINUE MISERY
            Dataset tmp_left;
            Dataset tmp_right;
            divide_dataset(current->subset, i, ret_data(iterator_j), &tmp_left, &tmp_right);
            int len_right = tmp_right->rows;
            int len_left = tmp_left->rows;
            printf("value: %f\n", *(double *)ret_data(iterator_j));
            printf("right %d left %d\n", len_right, len_left);
            if(1){
                free_subset_dataset(tmp_left);
                free_subset_dataset(tmp_right);
            }

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


