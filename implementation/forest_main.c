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

    printf("Reading dataset...\n");
    dt = read_dataset("./toy_data.csv",",",1,b,1);

    printf("Training forest and collecting votes...\n");
    tree_votes = train_and_vote(dt, n_trees, sample_ratio);

    printf("Amalgamating votes...\n");


    printf("Cleaning up...\n");
    free(tree_votes);
    free_dataset(dt);
    return 0;
}
