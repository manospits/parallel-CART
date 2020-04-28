#ifndef _SAMPLING_
#define _SAMPLING_

typedef struct _samples {
    double sample_ratio;
    int dataset_length;
    int n_sample;
    Dataset *subsets;
} _samples;

typedef _samples * Samples;

void initialise_samples();
void sample_indexes();
void create_samples();
void free_subsets();

#endif