#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../io/include/io.h"
#include "../../tree/include/tree.h"
#include "../include/sampling.h"

char **train_and_vote(Dataset dataset, int n_trees, double sample_ratio) {

    // Dummy length of predict dataset
    int predict_len = 5;

    // Allocate memory for output table
    size_t offset = sizeof(char*) * STRING_SIZE;
    char **tree_votes = malloc(sizeof(char**) * n_trees);
    for(int i=0; i < n_trees; i++){
        tree_votes[i] = malloc(sizeof(char*) * STRING_SIZE * predict_len);
    }

    // Iterate over n_trees and form n_trees votes
    for(int i = 0; i < n_trees; i++) {
        printf("Creating randomly sampled subset...\n");
        Dataset subset = create_random_subset(dataset, dataset->rows, sample_ratio, i);
        printf("Finito %d\n", subset->rows);

        printf("Watering tree...\n");
        Tree clf_tree = build_classification_tree(subset, "species");
        printf("Tree grew up...\n");

        printf("Gathering a seed...\n");

        for(int j = 0; j < predict_len; j++) {
            strcpy(tree_votes[i] + (j * offset), predict_row(clf_tree, dataset->data[j * 20]));
        }

        printf("Chopping the tree :\\\n");
        del_tree(clf_tree);
        free_subset_dataset(subset);
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