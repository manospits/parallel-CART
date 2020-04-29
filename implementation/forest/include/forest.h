#ifndef _FOREST_
#define _FOREST_

char **train_and_vote(Dataset dataset, int n_trees, double sample_ratio);
char *forest_prediction(int *tree_votes, int n_trees);

#endif