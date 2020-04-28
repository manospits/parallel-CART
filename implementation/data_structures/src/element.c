#include "../include/element.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct element_info {
    size_t size;
    int flag;
    int (*cmp)(const void*,const void *);
}element_info;

typedef struct element{
    void * elem;
}element;

pel_info create_type(size_t size,
                 int (*cmp)(const void*,const void *)){
    pel_info tmp=malloc(sizeof(struct element_info));
    if(tmp==NULL){
        fprintf(stderr,"Error allocating space -element.c.\n");
        exit(1);
    }
    tmp->size=size;
    tmp->cmp=cmp;
    return tmp;
}

pel create_elem(pel_info pinfo,void * a){
    pel tmp=malloc(sizeof(struct element));
    if(tmp==NULL){
        fprintf(stderr,"Error allocating space -element.c.\n");
        exit(1);
    }
    tmp->elem=malloc(pinfo->size);
    memcpy(tmp->elem, a, pinfo->size);
    return tmp;
}

int type_cmp(pel_info pinfo,pel a,pel b){
    return pinfo->cmp(a->elem,b->elem);
}

void type_destr(pel_info pinfo, pel a){
    free(a);
}

size_t get_type_size(pel_info a){
    return a->size;
}

int (*get_cmp(pel_info a))(const void *,const void *)
{
    return a->cmp;
}
void *get_ptr(pel a){
   return a->elem;
}

int int_cmp(const void * a, const void * b){
    int aa = *(int *) a;
    int bb = *(int *) b;
    if (aa == bb)
        return 0;
    else if(aa < bb)
        return -1;
    else
        return 1;
}

int double_cmp(const void * a, const void * b){
    double aa = *(double *) a;
    double bb = *(double *) b;
    if (aa == bb)
        return 0;
    else if(aa < bb)
        return -1;
    else
        return 1;
}

int str_cmp(const void * a, const void * b){
    char * aa = (char *)a;
    char * bb = (char *)b;
    return strcmp(aa,bb);
}
