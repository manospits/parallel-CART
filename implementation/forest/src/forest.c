#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../io/include/io.h"
#include "../../tree/include/tree.h"
#include "../include/sampling.h"

char **train_and_vote(Dataset train_set, Dataset test_set, int n_trees, double sample_ratio) {
    // Allocate memory for output table
    size_t offset = sizeof(char*) * STRING_SIZE;
    char **tree_votes = malloc(sizeof(char**) * n_trees);
    for(int i=0; i < n_trees; i++){
        tree_votes[i] = malloc(sizeof(char*) * STRING_SIZE * test_set->rows);
    }

    // Iterate over n_trees and form n_trees votes
    for(int i = 0; i < n_trees; i++) {
        printf("Creating randomly sampled subset...\n");
        Dataset train_subset = create_random_subset(train_set, train_set->rows, sample_ratio, i);
        printf("Finito %d\n", train_subset->rows);

        printf("Watering tree...\n");
        Tree clf_tree = build_classification_tree(train_subset, "species");
        printf("Tree grew up...\n");

        printf("Gathering seeds...\n");
        for(int j = 0; j < test_set->rows; j++) {
            strcpy(tree_votes[i] + (j * offset), predict_row(clf_tree, test_set->data[j]));
        }

        printf("Chopping the tree :\\\n");
        del_tree(clf_tree);
        free_subset_dataset(train_subset);
        printf("\n\n");
    }

    return tree_votes;
}

char **forest_predict(char **tree_votes, int n_trees, int predict_len) {
    // Allocate memory for votes
    size_t offset = sizeof(char*) * STRING_SIZE;
    char **predictions = malloc(sizeof(char*) * predict_len);
    for(int i = 0; i < predict_len; i++) {
        predictions[i] = malloc(sizeof(char) * STRING_SIZE);
    }

    // Iterate each predicted class
    for(int j = 0; j < predict_len; j++) {
        // Iterate each trees prediction and find most common
        int max_count = 0;

        for(int i = 0; i < n_trees; i++) {
            int count = 0;

            for(int k = 0; k < n_trees; k++) {
                if(strcmp(tree_votes[i] + (j * offset), tree_votes[k] + (j * offset)) == 0) {
                    count += 1;
                }
            }

            // Set jth prediction to most common vote
            if(count > max_count) {
                max_count = count;
                strcpy(predictions[j], tree_votes[i] + (j * offset));
            }
        }
    }

    return predictions;
}

void free_predictions(char **predictions, int n_trees) {
    for(int i = 0; i < n_trees; i++) {
        free(predictions[i]);
    }

    free(predictions);
}