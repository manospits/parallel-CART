#include <stdio.h>
#include <stdlib.h>
#include "../include/node.h"
#include "../include/tree.h"
#include "../../data_structures/include/list.h"
#include "../../data_structures/include/element.h"
#include "../../io/include/io.h"
#include "string.h"

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

    ds_list_and_type(counts);

    return imp;
}

double calc_gini_coefficient_between(Dataset dt, Attribute * class_attr, int s, int e) {
    int total = dt -> rows;
    double imp=0.0;

    phead counts=unique_counts_between(dt, class_attr, s, e);
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

    ds_list_and_type(counts);

    return imp;
}

char * traverse(Tree clf_tree, Node current, char * row){
    int res;

    if(current->split_val==NULL){
        return current->most_common;
    }
    if(current->attr->dtype=='s'){
        char * row_val= row+current->attr->offset;
        char * split_val= current->split_val;
        res = current->attr->cmp(&split_val, &row_val);
    }
    else{
        double row_val= *(double *) (row+current->attr->offset);
        double split_val= *(double *) (current->split_val);
        res = current->attr->cmp( &row_val, &split_val);
    }
    if(res>=0)
        return traverse(clf_tree, current->right, row);
    else
        return traverse(clf_tree, current->left, row);
}

char * predict_row(Tree clf_tree, char * row){
    return traverse(clf_tree, clf_tree->root, row);
}

Tree build_classification_tree(Dataset train_dataset, char *class_field, int max_height) {
    Tree clf_tree = create_tree(train_dataset, class_field);
    Attribute *class_attr;
    for(int i=0;i< train_dataset->attributes_number; i++){
        if(strcmp(train_dataset->attributes[i].name, class_field)==0){
            class_attr=&(train_dataset->attributes[i]);
        }
    }
    clf_tree->predict_attribute=class_attr;
    strcpy(clf_tree->predict_field, class_field);
    //start with all training data instances
    pel_info int_type = create_type(sizeof(int), &intcmp, &free );
    phead rows_list=cr_list(int_type);
    for(int i = 0; i < train_dataset->rows; i++){
        insert(rows_list, &i);
    }
    Dataset subset = get_subset(train_dataset, rows_list);
    ds_list(rows_list);
    free(int_type);

    //call recursive function to build tree
    clf_tree->root=grow_tree(clf_tree, subset, 0, max_height);
    /*printf("%s\n", predict_row(clf_tree, train_dataset->data[0]));*/
    return clf_tree;
}

void divide_dataset(Dataset subset, int attribute_index, void * value, Dataset * leftSubset, Dataset * rightSubset){
    phead left;
    phead right;

    pel_info int_type = create_type(sizeof(int), &intcmp, &free );
    left = cr_list(int_type);
    right = cr_list(int_type);
    for(int i = 0; i < subset->rows; i++){
        if (subset->attributes[attribute_index].dtype=='s'){
            char * value2 = subset->data[i]+subset->attributes[attribute_index].offset;
            if(subset->attributes[attribute_index].cmp(&value2 , value )>=0)
                insert(right, &i);
            else
                insert(left, &i);

        }
        else{
            double value1 = *(double *) (value);
            double value2 = *(double *) (subset->data[i]+subset->attributes[attribute_index].offset);
            /*printf("---- %f - %f---\n", value1, value2);*/
            if(subset->attributes[attribute_index].cmp(&value2 , &value1 )>=0)
                insert(right, &i);
            else
                insert(left, &i);
        }
    }
    /*printf("right %d left %d\n", get_size(right), get_size(left));*/
    *leftSubset = get_subset(subset, left);
    *rightSubset = get_subset(subset, right);
    ds_list(left);
    ds_list(right);
    free(int_type);
}

Node grow_tree(Tree clf_tree, Dataset subset, int current_height, int max_height){
    if(subset->rows == 0) return NULL;
    if (current_height>max_height ) return NULL;
    Node current=create_node();
    current->subset = subset;

    char * most_common=NULL;
    int max_count=-1;
    phead counts=unique_counts(current->subset, clf_tree->predict_attribute);
    pnode iterator_c = get_list(counts);
    for(int i=0; i<get_size(counts); i++ ){
        value_count * label_count_i = ((value_count *) ret_data(iterator_c));
        if(label_count_i->count > max_count){
            max_count=label_count_i->count;
            most_common = label_count_i->value;
        }
        iterator_c = next_node(iterator_c);
    }
    current->most_common = malloc(clf_tree->predict_attribute->size);
    memcpy(current->most_common, most_common, clf_tree->predict_attribute->size);
    ds_list_and_type(counts);
    double current_score = calc_gini_coefficient(current->subset, clf_tree->predict_attribute);
    /*printf("Gini is %f\n", current_score);*/
    double best_gain = 0.0;
    Attribute * best_attribute;
    int best_attribute_i;

    /*phead unique_col_i_values;*/
    /*pnode iterator_j;*/
    for (int i=0; i < current->subset->attributes_number; i++){
        /*printf("%d/%d\n",i,current->subset->attributes_number);*/
        if(!strcmp(subset->attributes[i].name, clf_tree->predict_field)) continue;
        Dataset sorted_i = get_sorted_version(current->subset,i);
        int j = sorted_i->rows-1;
        /*puts("------------------------------------------------------");*/
        while(j >=0){
            /*printf("%f\n",*(double *)(sorted_i->data[j]+sorted_i->attributes[i].offset));*/
            if(sorted_i->attributes[i].dtype=='d' && j-1>=0){
                double val1=*(double*) (sorted_i->data[j]+sorted_i->attributes[i].offset);
                double val2=*(double*) (sorted_i->data[j-1]+sorted_i->attributes[i].offset);
                if(sorted_i->attributes[i].cmp(&val1, &val2)==0){
                    j--;
                    continue;
                }
            }
            if(sorted_i->attributes[i].dtype=='s' && j-1>=0){
                char *val1 = sorted_i->data[j]+sorted_i->attributes[i].offset;
                char *val2 =sorted_i->data[j-1]+sorted_i->attributes[i].offset;
                if(sorted_i->attributes[i].cmp(val1,val2)==0){
                    j--;
                    continue;
                }
            }
            int len_right = sorted_i->rows-j;
            int len_left = j;
            double p = (double) len_right / (double) current->subset->rows;
            double gain = current_score - p*calc_gini_coefficient_between(sorted_i, clf_tree->predict_attribute,j,sorted_i->rows) -
                (1-p)*calc_gini_coefficient_between(sorted_i, clf_tree->predict_attribute, 0, j);
            /*printf("-------------Gini calc----------\n");*/
            if(gain >  best_gain && len_right > 0 & len_left >0){
                best_gain=gain;
                best_attribute = &(subset->attributes[i]);
                best_attribute_i =i;
                if(current->split_val)
                    free(current->split_val);
                current->split_val = malloc(current->subset->attributes[i].size);
                memcpy(current->split_val, sorted_i->data[j]+sorted_i->attributes[i].offset,current->subset->attributes[i].size);
                /*printf("%d %d %f\n",len_left, len_right, *(double*)(current->split_val));*/
            }
            j--;
        }
        free_subset_dataset(sorted_i);
    }
    if (best_gain > 0){
        if(current_height + 1> max_height){
            free(current->split_val);
            current->split_val=NULL;
        }
        else{
            current->attr=best_attribute;
            Dataset best_set_left, best_set_right;
            divide_dataset(current->subset, best_attribute_i ,current->split_val, &best_set_left, &best_set_right);
            /*printf("%f\n",*(double *)(current->split_val));*/
            /*printf("Height is: %d best gain is: %f best attribute: %s right size: %d left size: %d\n",current_height, best_gain, best_attribute->name, best_set_right->rows, best_set_left->rows);*/
            current->right = grow_tree(clf_tree, best_set_right, current_height+1, max_height);
            current->left = grow_tree(clf_tree, best_set_left, current_height+1, max_height);
        }
    }
    return current;
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


