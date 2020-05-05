#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include "io/include/io.h"
#include "tree/include/tree.h"
#include "forest/include/sampling.h"


// Forest and tree settings
int n_trees = 10;
int max_tree_height = 5;
double sample_ratio = 0.8;
double test_train_ratio = 0.8;

void free_predictions(char **predictions, int n_trees) {
    for(int i = 0; i < n_trees; i++) {
        free(predictions[i]);
    }

    free(predictions);
}

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
    char *forest_votes=NULL;
    int *recv_counts=NULL;
    int *dspls=NULL;
    int n_ranks, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("[%d] Reading dataset...\n", rank);
    dt = read_dataset("../data/7796666/dataset100.csv",",",1,b,1);
    /*dt = read_dataset("./toy_data.csv",",",1,b,1);*/

    printf("[%d] Creating test, train subset...\n", rank);
    train_test_split(dt, &train_set, &test_set, test_train_ratio);

    printf("[%d] %d\n", rank, dt->rows);
    printf("[%d] train %d test %d\n", rank, train_set->rows, test_set->rows);

    printf("[%d] Creating randomly sampled subset...\n", rank);
    create_random_subset(train_set, &train_subset, train_set->rows, sample_ratio, rank);
    printf("[%d] Finito %d\n", rank, train_subset->rows);

    printf("[%d] Watering tree...\n", rank);
    clf_tree = build_classification_tree(train_subset, "Gender", max_tree_height);
    printf("[%d] Tree grew up...\n", rank);

    // Allocate memory for tree votes
    char * tree_votes = malloc(sizeof(char) *STRING_SIZE * test_set->rows);

    printf("[%d] Gathering seeds...\n", rank);
    for(int i = 0; i < test_set->rows; i++) {
        strcpy(tree_votes+(STRING_SIZE*i), predict_row(clf_tree, test_set->data[i]));
    }

    // Allocate memory for forest votes
    if(rank == 0) {
        forest_votes = malloc(n_ranks*(test_set->rows)*STRING_SIZE*sizeof(char));
        recv_counts = malloc(sizeof(int) * n_ranks);
        dspls = malloc(sizeof(int) * n_ranks);
        for(int i=0; i < n_ranks; i++){
            dspls[i] = i * STRING_SIZE * test_set->rows;
            recv_counts[i] = test_set->rows * STRING_SIZE;
        }
    }

    printf("[%d] Sending votes\n", rank);

    MPI_Gatherv(tree_votes, test_set->rows * STRING_SIZE , MPI_CHAR, forest_votes, recv_counts, dspls, MPI_CHAR, 0, MPI_COMM_WORLD);

    printf("[%d] Chop chop chop the tree :3\n", rank);
    del_tree(clf_tree);

    printf("[%d] Cleaning up...\n", rank);
    if(rank == 0){
        // Allocate memory for votes
        char **forest_predictions = malloc(sizeof(char*) * test_set->rows);
        for(int i = 0; i < test_set->rows; i++) {
            forest_predictions[i] = malloc(sizeof(char) * STRING_SIZE);
        }

        // Iterate each predicted class
        for(int j = 0; j < test_set->rows; j++) {
            // Iterate each trees prediction and find most common
            int max_count = 0;

            for(int i = 0; i < n_ranks; i++) {
                int count = 0;

                for(int k = 0; k < n_ranks; k++) {
                    if(strcmp(forest_votes+sizeof(char)*(dspls[i]+j*(STRING_SIZE)), forest_votes+sizeof(char)*(dspls[k]+j*(STRING_SIZE))) == 0) {
                        count += 1;
                    }
                }

                // Set jth prediction to most common vote
                if(count > max_count) {
                    max_count = count;
                    strcpy(forest_predictions[j], forest_votes+sizeof(char)*(dspls[i]+j*(STRING_SIZE)));
                }
            }
        }

        for(int i = 0; i < test_set->rows; i++) {
            printf("[%d] %d: %s %s\n", rank, i, forest_predictions[i], get_element_by_col_name(test_set, i, "Gender"));
        }

        free_predictions(forest_predictions, n_ranks);
        free(forest_votes);
        free(recv_counts);
        free(dspls);
    }

    free(tree_votes);
    free_subset_dataset(train_set);
    free_subset_dataset(test_set);
    free_subset_dataset(train_subset);
    free_dataset(dt);

    MPI_Finalize();
    return 0;
}


