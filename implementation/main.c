#include <stdio.h>

#include "io/include/io.h"
#include "forest/include/forest.h"

int main(void){
    // Settings
    char *s="species";
    char **b=&s;
    int n_trees = 4;
    double sample_ratio = 0.8;
    double test_train_ratio = 0.8;

    // Allocate memory
    Dataset dt;
    Dataset train_set;
    Dataset test_set;
    char **tree_votes;
    char **forest_predictions;

    printf("Reading dataset...\n");
    /*dt = read_dataset("../data/7796666/dataset.csv",",",1,b,1);*/
    dt = read_dataset("./toy_data.csv",",",1,b,1);

    printf("Creating test, train subset...\n");
    train_test_split(dt, &train_set, &test_set, test_train_ratio);

    printf("Data %d test %d train %d\n", dt->rows, test_set->rows, train_set->rows);

    printf("Training forest and collecting votes...\n");
    tree_votes = train_and_vote(train_set, test_set, n_trees, sample_ratio, "species", 5);

    printf("Amalgamating votes...\n");
    forest_predictions = forest_predict(tree_votes, n_trees, test_set->rows);

    printf("Forest votes...\n");
    for(int i = 0; i < test_set->rows; i++) {
        printf("%d: %s %s\n", i, forest_predictions[i], test_set->data[i]+test_set->attributes[test_set->attributes_number-1].offset);
    }



    printf("Cleaning up...\n");
    free_predictions(tree_votes, n_trees);
    free_predictions(forest_predictions, test_set->rows);
    free_subset_dataset(train_set);
    free_subset_dataset(test_set);
    free_dataset(dt);
    return 0;
}
