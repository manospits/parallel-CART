#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 

void sample_indexes(int n_data, int n_samples, int **indexes) {
    srand(time(NULL));
    int i;

    *indexes = malloc(n_samples * sizeof(int));
    for(i = 0; i < n_samples; i++) {
        (*indexes)[i] = i;
    }

    // Randomly choose and replace indexes
    for (; i < n_data; i++) {
        int j = rand() % (i + 1); 
  
        if (j < n_samples) {
            (*indexes)[j] = i;
        }
    } 
}

void create_subset(Dataset dataset, int n_data, double sample_ratio) {
    int n_samples = n_data * sample_ratio;
}

void free_subsets() {

}
