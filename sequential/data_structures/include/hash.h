#include "element.h"
#include "list.h"

typedef struct hash_info *phash;
typedef struct hash_node *phnode;
// hash table
// __
//|__|->N->....->|-
//|__|->N->N->..|-
//| .|
//| .|
//| .|
//|__|->N->...|-

phash create_hashtable(pel_info,int,int(*h)(int));
phnode create_phnode(pel_info,void* el);            //creates a hash_node
int h_insert(phash a,void * data,int val);          //inserts data in hash size
int in_hash(phash a,void* data, int val);           //takes data and checks if it exists in hash
void * hget_data(phash a,void* data, int val);      //returns data ptr
int h_delete(phash a,void* data,int val);           //deletes node with data
void ds_hash(phash a);                              //destroys hash table a
