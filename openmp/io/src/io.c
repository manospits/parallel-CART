#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/io.h"


void initialize_attribute(Attribute * attr, char * name, size_t size, int (*cmp)(const void*,const void *), char dtype, size_t offset){
    strcpy(attr->name, name);
    attr->size = size;
    attr->cmp = cmp;
    attr->dtype = dtype;
    attr->offset = offset;
}

Dataset read_dataset(const char * path, const char * sep, int header, char ** string_fields, int cs){
    Dataset tmp_dt;
    FILE * dataset_fp;
    char buff[LINE_READ];
    int line_num=0;
    char *ptr;
    int num_of_attributes=0;

    if ((dataset_fp = fopen(path, "r"))==NULL){
        return NULL;
    }

    tmp_dt = malloc(sizeof(Dataset_));

    while(fgets(buff, LINE_READ, dataset_fp)){
        //remove newline
        size_t ln = strlen(buff) - 1;
        if (*buff && buff[ln] == '\n')
            buff[ln] = '\0';
        /*printf("%s\n", buff);*/
        //get attributes from header and store them
        if(header && line_num==0){
            char buff_tmp[LINE_READ];

            strcpy(buff_tmp, buff);
            ptr = strtok(buff, sep);
            while(ptr != NULL){
                ptr = strtok(NULL, sep);
                num_of_attributes++;
            }

            //allocating memory for attributes array
            tmp_dt->attributes_number=num_of_attributes;
            tmp_dt->attributes=malloc(num_of_attributes*sizeof(Attribute));

            int attribute_index=0;
            size_t offset=0;
            ptr = strtok(buff_tmp, sep);
            while(ptr != NULL){
                size_t current_size=0;
                if(in_array(ptr,string_fields,cs)){
                    current_size=sizeof(char)*STRING_SIZE;
                    initialize_attribute(&(tmp_dt->attributes[attribute_index]), ptr, current_size, &double_cmp, 's', offset);
                }
                else{
                    current_size=sizeof(double);
                    initialize_attribute(&(tmp_dt->attributes[attribute_index]), ptr, current_size, &double_cmp, 'd', offset);
                }
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
        ptr = strtok(buff, sep);
        while(ptr != NULL){
            offset = tmp_dt->attributes[attribute_index].offset;
            if(tmp_dt->attributes[attribute_index].dtype=='d'){
                double tmp_num;
                tmp_num = strtod(ptr, NULL);
                memcpy(tmp_dt->data[tmp_dt->rows]+offset, &tmp_num, sizeof(double));
            }
            else{
                strncpy(tmp_dt->data[tmp_dt->rows]+offset,ptr, STRING_SIZE-1);
            }
            ptr = strtok(NULL, sep);
            attribute_index++;
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
    /*for(int i = 0; i < tmp_dt->rows; i++){*/
    /*for (int j =0; j < tmp_dt->attributes_number; j++){*/
    /*if(tmp_dt->attributes[j].dtype=='s')*/
    /*printf("%s ", tmp_dt->data[i]+tmp_dt->attributes[j].offset);*/
    /*if(tmp_dt->attributes[j].dtype=='d')*/
    /*printf("%f ",*(double*)(tmp_dt->data[i]+tmp_dt->attributes[j].offset));*/
    /*}*/
    /*printf("\n");*/
    /*}*/
    fclose(dataset_fp);
    return tmp_dt;
}

void free_dataset(Dataset dt){

    //free data
    for(int i=0; i<dt->allocated_rows; i++){
        free(dt->data[i]);
    }
    free(dt->data);

    //free attributes array
    free(dt->attributes);

    free(dt);
}

void free_subset_dataset(Dataset dt){
    free(dt->data);
    free(dt);
}

char ** get_row(Dataset dt, int row){
    return &dt->data[row];
}

char * get_element(Dataset dt, int row, int col){
    return dt->data[row]+dt->attributes[col].offset;
}

char * get_element_by_col_name(Dataset dt, int row, char* name){
    for (int i = 0; i< dt->attributes_number;i++){
        if(strcmp(dt->attributes[i].name,name)==0)
            return dt->data[row]+dt->attributes[i].offset;

    }
    return NULL;
}

int in_array(char *string, char **array, int s){
    for(int i=0; i < s; i++){
        /*printf("%s\n",array[i]);*/
        if(!strcmp(string,array[i])) return 1;
    }
    return 0;
}

Dataset get_subset(Dataset dt, phead rows){
    int rows_size=get_size(rows);
    Dataset tmp_dt;

    tmp_dt = malloc(sizeof(Dataset_));
    tmp_dt->attributes=dt->attributes;
    tmp_dt->attributes_number = dt->attributes_number;
    tmp_dt->row_size = dt->row_size;
    tmp_dt->rows=rows_size;
    tmp_dt->data = malloc(sizeof(char**)*tmp_dt->rows);

    pnode iterator = get_list(rows);

    for(int i=0; i<rows_size; i++ ){
        int row_num = *((int *) ret_data(iterator));
        tmp_dt->data[i]=dt->data[row_num];
        iterator = next_node(iterator);
    }
    return tmp_dt;
}

int vlccmp(const void * a, const void * b){
    char *aa = ((value_count *) a)->value;
    char *bb = ((value_count *) b)->value;
    return strcmp(aa,bb);
}

void modifier(void * a){
    value_count * tmp_vlc= (value_count *) a;
    tmp_vlc->count++;
}


phead unique_values(Dataset dt, Attribute * attribute){
    phead values_list;
    if(attribute->dtype == 's'){
        pel_info string_type = create_type(sizeof(char)*STRING_SIZE, &str_cmp, &free);
        values_list=cr_list(string_type);
        for (int i  = 0; i < dt->rows ; i++){
            char * value = dt->data[i]+attribute->offset;
            not_in_insert_sorted(values_list, &value);
        }
    }
    else{
        pel_info double_type = create_type(sizeof(double), &double_cmp, &free);
        values_list=cr_list(double_type);
        for (int i  = 0; i < dt->rows ; i++){
            double value = * (double *) (dt->data[i]+attribute->offset);
            not_in_insert_sorted(values_list, &value);
        }
    }
    return values_list;
}

phead unique_counts(Dataset dt, Attribute * attribute){
    phead labels_list;
    pel_info string_type = create_type(sizeof(value_count), &vlccmp, &free);
    labels_list=cr_list(string_type);

    if(attribute->dtype == 's'){
        for (int i  = 0; i < dt->rows ; i++){
            char * value = dt->data[i]+attribute->offset;
            value_count tmp_vlc;
            tmp_vlc.value=value;
            tmp_vlc.count=1;
            if(in_and_update(labels_list, &tmp_vlc, &modifier)){
                continue;
            }
            else{
                insert(labels_list, &tmp_vlc);
            }
        }
    }
    else{
        printf("Error....");
    }
    return labels_list;
}

phead unique_counts_between(Dataset dt, Attribute * attribute, int s, int e){
    phead labels_list;
    pel_info string_type = create_type(sizeof(value_count), &vlccmp, &free);
    labels_list=cr_list(string_type);

    if(attribute->dtype == 's'){
        for (int i  = s; i < e ; i++){
            char * value = dt->data[i]+attribute->offset;
            value_count tmp_vlc;
            tmp_vlc.value=value;
            tmp_vlc.count=1;
            if(in_and_update(labels_list, &tmp_vlc, &modifier)){
                continue;
            }
            else{
                insert(labels_list, &tmp_vlc);
            }
        }
    }
    else{
        printf("Error....");
    }
    return labels_list;
}

Dataset get_sorted_version(Dataset dt, int attribute_index){
    int rows_size=dt->rows;
    Dataset tmp_dt;

    tmp_dt = malloc(sizeof(Dataset_));
    tmp_dt->attributes=dt->attributes;
    tmp_dt->attributes_number = dt->attributes_number;
    tmp_dt->row_size = dt->row_size;
    tmp_dt->rows=rows_size;
    tmp_dt->data = malloc(sizeof(char**)*tmp_dt->rows);


    for(int i=0; i<rows_size; i++ ){
        tmp_dt->data[i]=dt->data[i];
    }
    dtquicksort(tmp_dt, attribute_index, 0, rows_size-1);
    return tmp_dt;
}

void dtquicksort(Dataset dt, int ai, int first, int last){
    /*printf("%d %d\n", first,last);*/
    int i, j, pivot;
    char *temp;

    if(first<last){
        pivot=first;
        i=first;
        j=last;

        while(i<j){
            /*printf("%d %d\n", i, j );*/
            if(dt->attributes[ai].dtype=='s'){
                while(dt->attributes[ai].cmp(dt->data[i]+dt->attributes[ai].offset,dt->data[pivot]+dt->attributes[ai].offset )<= 0 && i<last){
                    i++;
                }
                while(dt->attributes[ai].cmp(dt->data[j]+dt->attributes[ai].offset,dt->data[pivot]+dt->attributes[ai].offset )> 0){
                    j--;
                }
            }
            if(dt->attributes[ai].dtype=='d'){
                while(dt->attributes[ai].cmp((double *)(dt->data[i]+dt->attributes[ai].offset),(double *)(dt->data[pivot]+dt->attributes[ai].offset))<= 0 && i<last){
                    i++;
                }
                while(dt->attributes[ai].cmp((double *)(dt->data[j]+dt->attributes[ai].offset),(double *)(dt->data[pivot]+dt->attributes[ai].offset)) > 0){
                    j--;
                }
            }
            if(i<j){
                temp=dt->data[i];
                dt->data[i]=dt->data[j];
                dt->data[j]=temp;
            }
        }

        temp=dt->data[pivot];
        dt->data[pivot]=dt->data[j];
        dt->data[j]=temp;
        dtquicksort(dt,ai,first,j-1);
        dtquicksort(dt,ai,j+1,last);

    }
}

void train_test_split(Dataset dataset, Dataset *train_set, Dataset *test_set, double test_train_ratio) {
    int split_row = dataset->rows * test_train_ratio;
    phead train_subset, test_subset;

    pel_info int_type = create_type(sizeof(int), &intcmp, &free );
    train_subset = cr_list(int_type);
    test_subset = cr_list(int_type);

    for(int i = 0; i < dataset->rows; i++) {
        if(i < split_row) {
            insert(train_subset, &i);
        } else {
            insert(test_subset, &i);
        }
    }

    *train_set = get_subset(dataset, train_subset);
    *test_set = get_subset(dataset, test_subset);
    ds_list(train_subset);
    ds_list(test_subset);
    free(int_type);
}
