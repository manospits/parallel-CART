#ifndef _SAMPLING_
#define _SAMPLING_

phead sample_indexes(int n_data, int n_samples, int rank);
void create_random_subset(Dataset dataset, Dataset *subset, int n_data, double sample_ratio, int rank);

#endif