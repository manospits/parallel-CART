#include <stdio.h>
#include "./include/tree.h"
#include "./include/io.h"
int main(void){
    PTreeNode Tree = create_tree_node(0);
    PTreeNode Tree1 = create_tree_node(1);
    PTreeNode Tree2 = create_tree_node(2);
    insert_left(Tree,Tree1);
    insert_right(Tree,Tree2);
    free_tree(Tree);

    char *s="species";
    char **b=&s;
    printf("reading dataset\n");
    dataset dt = read_dataset("./toy_data.csv",",",1,b,1);
    free_dataset(dt);
    return 0;
}
