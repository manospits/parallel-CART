#include <stdio.h>
#include <stdlib.h>
#include "../../io/include/io.h"
#include "../../tree/include/tree.h"
#include "../include/sampling.h"

void create_forest(Dataset dataset, int n_trees, double sample_ratio) {

    for(int i = 0; i < n_trees; i++) {
        printf("Creating randomly sampled subset...\n");
        Dataset subset = create_random_subset(dataset, dataset->rows, sample_ratio, i);
        printf("Finito %d\n", subset->rows);

        printf("Watering tree...\n");
        Tree clf_tree = build_classification_tree(subset, "species");
        printf("Tree grew up...\n");

        printf("Gathering a seed...\n");
        printf("%s\n", predict_row(clf_tree, dataset->data[0]));

        printf("Chopping the tree :\\\n");
        del_tree(clf_tree);

        free_subset_dataset(subset);
    }
}

int forest_prediction(int *tree_votes, int n_trees) {
    int max_count = 0, prediction = 0;

    for(int i = 0; i < n_trees; i++) {
        int count  = 0;

        for(int j = 0; j < n_trees; j++) {
            if(tree_votes[i] == tree_votes[j]) {
                count += 1;
            }
        }

        if(count > max_count) {
            max_count = count;
            prediction = tree_votes[i];
        }
    }

    return prediction;
}
