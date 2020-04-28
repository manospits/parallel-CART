#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "../../data_structures/include/list.h"
#include "../../data_structures/include/element.h"
#include "../../io/include/io.h"

void sample_indexes(int rank, int n_data, int n_samples, int **indexes) {
    srand(time(NULL) ^ rank);
    int i;
    int *debug = malloc(n_samples * sizeof(int));
    *indexes = malloc(n_samples * sizeof(int));
    for(i = 0; i < n_samples; i++) {
        (*indexes)[i] = i;
        debug[i] = i;
    }

    // Randomly choose and replace indexes
    for (; i < n_data; i++) {
        int j = rand() % (i + 1); 
  
        if (j < n_samples) {
            (*indexes)[j] = i;
            debug[j] = i;
        }
    }
}

void create_random_subsets(Dataset dataset, int n_trees, int n_data, double sample_ratio) {
    int i, j;
    int n_samples = n_data * sample_ratio;

    Dataset *subsets =  malloc(n_trees * sizeof(Dataset));

    for(i = 0; i < n_trees; i++) {
        int *indexes;
        sample_indexes(i, n_data, n_samples, &indexes);
    }
}

void free_subsets() {

}
