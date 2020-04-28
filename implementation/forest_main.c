#include <stdio.h>

#include "io/include/io.h"
// #include "tree/include/tree.h"
#include "forest/include/sampling.h"

int main(void){
    char *s="species";
    char **b=&s;
    int n_trees = 4;
    double sample_ratio = 0.5;
    printf("Reading dataset...\n");
    Dataset dt = read_dataset("./toy_data.csv",",",1,b,1);

    create_random_subsets(dt, n_trees, dt->rows, sample_ratio);

    // Tree clf_tree = build_classification_tree(dt, "species");
    // del_tree(clf_tree);

    free_dataset(dt);
    return 0;
}
