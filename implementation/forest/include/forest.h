#ifndef _FOREST_
#define _FOREST_

void create_forest(Dataset dataset, int n_trees, double sample_ratio);
int forest_prediction(int *tree_votes, int n_trees);

#endif