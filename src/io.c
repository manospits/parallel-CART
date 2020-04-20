#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/io.h"


int double_cmp(const void * a, const void * b){
    double aa, bb;
    aa = *((double *) a);
    bb = *((double *) b);
    if (aa == bb) return 0;
    if (aa < bb) return -1;
    return 1;
}


void initialize_attribute(attribute * attr, char * name, size_t size, int (*cmp)(const void*,const void *), char dtype, size_t offset){
    strcpy(attr->name, name);
    attr->size = size;
    attr->cmp = cmp;
    attr->dtype = dtype;
    attr->offset = offset;
}

dataset read_dataset(const char * path, const char * sep, int header){
    dataset tmp_dt;
    FILE * dataset_fp;
    char buff[255];
    int line_num=0;
    char *ptr;
    int num_of_attributes=0;

    if ((dataset_fp = fopen(path, "r"))==NULL){
        return NULL;
    }

    tmp_dt = malloc(sizeof(dataset_));

    while(fgets(buff, 255, dataset_fp)){
        //remove newline
        size_t ln = strlen(buff) - 1;
        if (*buff && buff[ln] == '\n')
            buff[ln] = '\0';

        //get attributes from header and store them
        if(header && line_num==0){
            char buff_tmp[255];

            strcpy(buff_tmp, buff);
            ptr = strtok(buff, sep);
            while(ptr != NULL){
                ptr = strtok(NULL, sep);
                num_of_attributes++;
            }

            //allocating memory for attributes array
            tmp_dt->attributes_number=num_of_attributes;
            tmp_dt->attributes=malloc(num_of_attributes*sizeof(attribute));

            int attribute_index=0;
            size_t offset=0, total_size;
            ptr = strtok(buff_tmp, sep);
            while(ptr != NULL){
                size_t current_size=sizeof(double);
                initialize_attribute(&(tmp_dt->attributes[attribute_index]), ptr, current_size, &double_cmp, 'd', offset);
                //advance
                ptr = strtok(NULL, sep);
                offset+=current_size;
                attribute_index++;
            }
            tmp_dt->row_size=offset;
            //ALLOCATE MEMORY FOR DATA
            tmp_dt->data = malloc(sizeof(char**)*DATA_ARRAY_INITIAL_SIZE);
            for(int i=0; i < DATA_ARRAY_INITIAL_SIZE; i++){
                tmp_dt->data[i]=malloc(tmp_dt->row_size);
            }
            tmp_dt->rows = 0;
            tmp_dt->allocated_rows = DATA_ARRAY_INITIAL_SIZE;
            line_num++;
            continue;
        }


        int attribute_index=0;
        size_t offset;
        char tmp_data[STRING_SIZE];
        ptr = strtok(buff, sep);
        while(ptr != NULL){
            ptr = strtok(NULL, sep);


            offset = tmp_dt->attributes[attribute_index].offset;
            if(tmp_dt->attributes[attribute_index].dtype=='d'){
                double tmp_num;
                tmp_num = strtod(ptr, NULL);
                memcpy(tmp_dt->data[tmp_dt->rows]+offset, &tmp_num, sizeof(double));
            }
            num_of_attributes++;
        }
        tmp_dt->rows++;
        line_num++;

        //check if more memory is needed and reallocate
        if(tmp_dt->rows == tmp_dt->allocated_rows){
            int new_rows_num = tmp_dt->allocated_rows*2;
            tmp_dt->data = realloc(tmp_dt->data, sizeof(char**)*new_rows_num);
            for(int i = tmp_dt->allocated_rows; i < new_rows_num; i++){
                tmp_dt->data[i]=malloc(tmp_dt->row_size);
            }
            tmp_dt->allocated_rows=new_rows_num;
        }
    }
    fclose(dataset_fp);

    return tmp_dt;
}

void free_dataset(dataset dt){

    //free data
    for(int i=0; i<dt->allocated_rows; i++){
        free(dt->data[i]);
    }
    free(dt->data);

    //free attributes array
    for(int i=0; i < dt->attributes_number; i++){
        printf("contains attribute: %s.\n", dt->attributes[i].name);
    }
    free(dt->attributes);

    free(dt);
}

char ** get_row(dataset dt, int row){
    return &dt->data[row];
}

char * get_element(dataset dt, int row, int col){
    return &dt->data[row][col];
}
