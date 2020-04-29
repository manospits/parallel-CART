#ifndef _FOREST_
#define _FOREST_

char **train_and_vote(Dataset dataset, int n_trees, double sample_ratio);
char **forest_predict(char **tree_votes, int n_trees, int predict_len);
void free_predictions(char **predictions, int n_trees);

#endif