#include <stdio.h>

#include "io/include/io.h"
#include "forest/include/forest.h"

int main(void){
    // Settings
    char *s="species";
    char **b=&s;
    int n_trees = 4;
    double sample_ratio = 0.8;

    // Allocate memory
    Dataset dt;
    char **tree_votes;
    char **forest_predictions;

    printf("Reading dataset...\n");
    dt = read_dataset("./toy_data.csv",",",1,b,1);

    printf("Training forest and collecting votes...\n");
    tree_votes = train_and_vote(dt, n_trees, sample_ratio);

    printf("Amalgamating votes...\n");
    forest_predictions = forest_predict(tree_votes, n_trees, 5);

    printf("Forest votes...\n");
    for(int i = 0; i < 5; i++) {
        size_t offset = sizeof(char*) + STRING_SIZE;
        printf("%d: %s\n", i, forest_predictions[i] + offset);
    }

    printf("Cleaning up...\n");
    free(tree_votes);
    free(forest_predictions);
    free_dataset(dt);
    return 0;
}
