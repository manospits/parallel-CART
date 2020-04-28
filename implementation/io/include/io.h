#ifndef _IO_
#define _IO_

#include <stdio.h>
#include "../../data_structures/include/list.h"
#define ATTRIBUTE_LEN 32
#define STRING_SIZE 64
#define DATA_ARRAY_INITIAL_SIZE 128


typedef struct Attribute{
    char name[ATTRIBUTE_LEN];               //name of the field
    size_t size;                            //size of the values of the field
    int (*cmp)(const void*,const void *);   //comparator to use (maybe useful)
    char dtype;                             // type identifier for printing and memcpy  d (double) or s (string)
    size_t offset;                          //offset of the field value in data row
}Attribute;

typedef struct Dataset_{
    char **data;                            //char (byte) 2d dynamically allocated array for storing data
    Attribute *attributes;                  //array of attribute structs for field information
    int attributes_number;                  //number of columns
    size_t row_size;                        //size to allocate for each row in data array
    int rows;                               //number of rows in data array with actual data
    int allocated_rows;                     //allocated rows
}Dataset_;

typedef struct value_count{
    char *value;
    int count;
}value_count;

typedef struct Dataset_ * Dataset;

void initialize_attribute(Attribute * attr, char * name, size_t size, int (*cmp)(const void*,const void *), char dtype, size_t offset);
/* initialize an attribute structure
 * !!! it doesn't allocate the memory it just sets the values */

Dataset read_dataset(const char * path, const char * sep, int header, char ** string_fields, int cs);
/* input: path is the location of the dataset
 *        sep is the separator to be uses
 *        header 0 if header is not included
 *
 * output: a pointer to a Dataset structure
 */

void free_dataset(Dataset dt);
/*
 * Frees the resources of the Dataset (input)
 */

void free_subset_dataset(Dataset dt);

char ** get_row(Dataset dt, int row);
/*
 * returns a pointer to the row with number `row'
 */

char * get_element(Dataset dt, int row, int col);
/*
 * returns the address of the element in row col
 */

char * get_element_by_col_name(Dataset dt, int row, char* name);
/*
 * returns the adress of element specified by row and field name
 */

int in_array(char *string, char **array, int s);
/*
 * checks if string is in array (0 False)
 */

Dataset get_subset(Dataset dt, phead rows);
phead unique_values(Dataset dt, Attribute * attribute);
phead unique_counts(Dataset dt, Attribute * attribute);
#endif
