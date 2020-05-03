#include <stdio.h>
#include "hash.h"
#include "list.h"
#include "element.h"

typedef struct hash_info {
    phead* bins;
    int size;
    pel_info bin_type;
    pel_info data_info;
    int (*h)(int);
}hash_info;

typedef struct hash_node {
    pel el;
}hash_node;

pel_info hdata_info;

int hnode_cmp(const void *A,const void *B){
    phnode a =(phnode)A;
    phnode b =(phnode)B;
    return type_cmp(hdata_info,a->el,b->el);
}

phash create_hashtable(pel_info data_type,int size ,int(*h)(int)){
    phash tmp;
    int i;
    tmp=malloc(sizeof(struct hash_info));
    if(tmp==NULL){
        fprintf(stderr,"Error allocating memory -hash.c\n");
        exit(1);
    }
    tmp->size=size;
    tmp->data_info=data_type;
    tmp->h=h;
    tmp->bins=malloc(sizeof(phead)*size);
    if(tmp->bins==NULL){
        fprintf(stderr,"Error allocating memory -hash.c\n");
        exit(1);
    }
    tmp->bin_type=create_type(sizeof(struct hash_node),hnode_cmp);
    for(i=0;i<size;i++){
        tmp->bins[i]=cr_list(tmp->bin_type);
    }
    return tmp;
}

phnode create_phnode(pel_info data_info,void *el){
    phnode tmp;
    tmp=malloc(sizeof(struct hash_node));
    if(tmp==NULL){
        fprintf(stderr,"Error allocating memory -hash.c\n");
        exit(1);
    }
    tmp->el=create_elem(data_info,el);
    return tmp;
}

void phnode_destr(phnode a){
    type_destr(hdata_info,a->el);
    free(a);
}

void ph_destr(void *a){
    phnode_destr((phnode) a);
}

int h_delete(phash a,void* data,int val){
    int pos=a->h(val);
    phnode tmp;
    tmp=create_phnode(a->data_info,data);
    delete(a->bins[pos],tmp,&ph_destr);
    phnode_destr(tmp);
    return 0;
}

int in_hash(phash a,void* data,int val){
    int pos=a->h(val);
    phnode tmp;
    tmp=create_phnode(a->data_info,data);
    int stat = in(a->bins[pos],tmp);
    phnode_destr(tmp);
    return stat;
}

void* hget_data(phash a,void* data,int val){
    int pos=a->h(val);
    phnode tmp;
    tmp=create_phnode(a->data_info,data);
    pnode tmp2=get_node(a->bins[pos],tmp);
    if(tmp2==NULL){
        phnode_destr(tmp);
        return NULL;
    }
    void* tmpdata = get_ptr(((phnode)ret_data(tmp2))->el);
    phnode_destr(tmp);
    return tmpdata;
}

int h_insert(phash a,void* data,int val){
    int pos=a->h(val);
    phnode tmp=create_phnode(a->data_info,data);
    insert(a->bins[pos],tmp);
    return 0;
}

void ds_hash(phash a){
    int i;
    for(i=0;i<a->size;i++){
        ds_list_plus_data(a->bins[i],ph_destr);
    }
    free(a->bins);
    free(a->bin_type);
    free(a);
}
