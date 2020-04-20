#ifndef _IO_
#define _IO_

#include <stdio.h>
#define ATTRIBUTE_LEN 32
#define STRING_SIZE 64
#define DATA_ARRAY_INITIAL_SIZE 128

typedef struct attribute{
    char name[ATTRIBUTE_LEN];
    size_t size;
    int (*cmp)(const void*,const void *);
    char dtype;//d or s
    size_t offset;
}attribute;

typedef struct dataset_{
    char **data;
    attribute *attributes;
    int attributes_number;
    size_t row_size;
    int rows;
    int allocated_rows;
}dataset_;

typedef struct dataset_ * dataset;

void initialize_attribute(attribute * attr, char * name, size_t size, int (*cmp)(const void*,const void *), char dtype, size_t offset);

dataset read_dataset(const char * path, const char * sep, int header);
/* input: path is the location of the dataset
 *        sep is the separator to be uses
 *        header 0 if header is not included
 *
 * output: a pointer to a dataset structure
 */

void free_dataset(dataset dt);
/*
 * Frees the resources of the dataset (input)
 */

char ** get_row(dataset dt, int row);
char * get_element(dataset dt, int row, int col);
char * get_element_by_col_name(dataset dt, int row, char* name);

#endif
