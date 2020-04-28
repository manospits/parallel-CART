#ifndef _FOREST_
#define _FOREST_

int forest_prediction(int tree_votes, int n_trees) {
    int max_count = 0, prediction = 0;

    for(int i = 0; i < n_trees; i++) {
        int count  = 0;

        for(int j = 0; j < n_trees; j++) {
            if(tree_votes[i] = tree_votes[j]) {
                count += 1;
            }
        }

        if(count > max_count) {
            max_count = count;
            prediction = tree_votes[i];
        }
    }

    return prediction;
}

#endif