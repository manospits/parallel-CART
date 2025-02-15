#ifndef _FOREST_
#define _FOREST_

char **train_and_vote(Dataset train_set, Dataset test_set, int n_trees, double sample_ratio, char * class_field, int max_height);
char **forest_predict(char **tree_votes, int n_trees, int predict_len);
void free_predictions(char **predictions, int n_trees);

#endif
