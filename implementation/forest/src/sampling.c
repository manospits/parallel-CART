#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "../../data_structures/include/list.h"
#include "../../data_structures/include/element.h"
#include "../../io/include/io.h"

phead sample_indexes(int n_data, int n_samples, int rank) {
    srand(time(NULL) ^ rank);

    int i;
    int *temp_indexes = malloc(n_samples * sizeof(int));
    for(i = 0; i < n_samples; i++) {
        temp_indexes[i] = i;
    }

    // Randomly choose and replace indexes
    for(i = n_samples; i < n_data; i++) {
        int j = rand() % (i + 1); 
  
        if (j < n_samples) {
            temp_indexes[j] = i;
        }
    }

    pel_info int_type = create_type(sizeof(int), &intcmp, &free);
    phead indexes = cr_list(int_type);
    for(i = 0; i < n_samples; i++){
        insert(indexes, &temp_indexes[i]);
    }

    free(temp_indexes);
    free(int_type);
    return indexes;
}

Dataset create_random_subset(Dataset dataset, int n_data, double sample_ratio, int rank) {
    int n_samples = n_data * sample_ratio;

    phead indexes;
    indexes = sample_indexes(n_data, n_samples, rank);
    Dataset subset = get_subset(dataset, indexes);

    // ds_list(indexes); // Causes seg fault, without causes mem leak
    return subset;
}
