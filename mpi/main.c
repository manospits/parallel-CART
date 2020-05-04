#include <stdio.h>
#include <mpi.h>
#include "io/include/io.h"
#include "tree/include/tree.h"
#include "forest/include/sampling.h"
#include "forest/include/forest.h"

// Forest and tree settings
int n_trees = 10;
int max_tree_height = 5;
double sample_ratio = 0.8;
double test_train_ratio = 0.8;

int main(void){
    // Settings
    /*char *s="species";*/
    char *s="Gender";
    char **b=&s;

    // Allocate memory
    Dataset dt;
    Dataset train_set;
    Dataset test_set;
    Dataset train_subset;
    Tree clf_tree;
    char **tree_votes;
    char **forest_predictions;
    int n_ranks, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("[%d] Reading dataset...\n", rank);
    dt = read_dataset("../data/7796666/dataset100.csv",",",1,b,1);
    /*dt = read_dataset("./toy_data.csv",",",1,b,1);*/

    printf("[%d] Creating test, train subset...\n", rank);
    train_test_split(dt, &train_set, &test_set, test_train_ratio);

    printf("[%d] Creating randomly sampled subset...\n", rank);
    create_random_subset(train_set, train_subset, train_set->rows, sample_ratio, rank);
    printf("[%d] Finito %d\n", rank, train_subset->rows);

    printf("Watering tree...\n");
    clf_tree = build_classification_tree(train_subset, "Gender", max_tree_height);
    printf("Tree grew up...\n");

    printf("[%d] Cleaning up...\n", rank);
    // free_predictions(tree_votes, n_trees);
    // free_predictions(forest_predictions, test_set->rows);
    free_subset_dataset(train_set);
    free_subset_dataset(test_set);
    free_dataset(dt);

    MPI_Finalize();
    return 0;
}
