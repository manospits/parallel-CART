#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "../../data_structures/include/list.h"
#include "../../data_structures/include/element.h"
#include "../../io/include/io.h"

phead sample_indexes(int rank, int n_data, int n_samples) {
    srand(time(NULL) ^ rank);

    int i;
    int *temp_indexes = malloc(n_samples * sizeof(int));
    for(i = 0; i < n_samples; i++) {
        temp_indexes[i] = i;
    }

    // Randomly choose and replace indexes
    for (; i < n_data; i++) {
        int j = rand() % (i + 1); 
  
        if (j < n_samples) {
            temp_indexes[j] = i;
        }
    }

    pel_info int_type = create_type(sizeof(int), &intcmp, &free);
    phead indexes = cr_list(int_type);
    for(int i = 0; i < n_samples; i++){
        insert(indexes, &temp_indexes[i]);
    }

    return indexes;
}

void create_random_subsets(Dataset dataset, Dataset **subsets, int n_trees, int n_data, double sample_ratio) {
    int i;
    int n_samples = n_data * sample_ratio;

    *subsets = malloc(n_trees * sizeof(Dataset));

    for(i = 0; i < n_trees; i++) {
        phead indexes;
        indexes = sample_indexes(i, n_data, n_samples);
        (*subsets)[i] = get_subset(dataset, indexes);
    }
}

void free_subsets(Dataset **subsets, int n_trees) {
    for(int i = 0; i < n_trees; i++) {
        free((*subsets)[i]->data);
    }
    free(*subsets);
}
