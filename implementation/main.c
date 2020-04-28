#include <stdio.h>

#include "io/include/io.h"
#include "tree/include/tree.h"

int main(void){
    char *s="species";
    char **b=&s;
    printf("Reading dataset...\n");
    Dataset dt = read_dataset("./toy_data.csv",",",1,b,1);

    printf("Watering tree...\n");
    Tree clf_tree = build_classification_tree(dt, "species");
    printf("Tree grew up...\n");

    printf("Gathering a seed...\n");
    printf("%s\n", predict_row(clf_tree, dt->data[0]));

    printf("Chopping the tree :\\\n");
    del_tree(clf_tree);
    free_dataset(dt);
    return 0;
}
