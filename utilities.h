// 0000 file created by Yarib Nevarez @ Gdl IBM
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdlib.h>
#include "node.h"

#define SUCCESS 0
#define FAIL    1

#define UNSIGNED_INT_LIMIT (unsigned int)(-1)

void ASSERT_TRACE(int exp, char * msg);

void * mem_alloc(size_t size);
void * mem_calloc(size_t num, size_t size);
void * mem_realloc(void * mem_ptr, size_t size);
void mem_free(void * mem_ptr);
void mem_set(void * mem_ptr, int value, size_t size);
void mem_cpy(void * mem_ptr_destination, const void * mem_ptr_source, size_t size);
void mem_move(void * mem_ptr_destination, const void * mem_ptr_source, size_t size);

void * new_buffer(size_t size);
void delete_buffer(void ** ppBuffer);
void * duplicate_buffer(void * pBuffer, size_t size);
void * realloc_buffer(void * pBuffer, size_t size);

char * get_file_string(char file_name[]);

void char_string_copy(char ** str, char * str_src, int str_len);
char * duplicate_char_buffer(char * str);
void delete_char_buffer(char * str);

int evaluate_expression(char * str, int str_len, long int * value);

void error(Node * line, char * msg);

// TextViewPort declaration

char * TextViewPort_getBuffer();
void TextViewPort_clear(void);

// Array Class declaration

typedef struct _ArrayClass  Array;
typedef void *              Object;

typedef Object  (*Array_get_object)(Array *, int);
typedef void    (*Array_append_object)(Array *, Object);
typedef void    (*Array_delete_object)(Array *, int);
typedef int     (*Array_get_count)(Array *);
typedef Array * (*Array_duplicate)(Array *);

#define ARRAY_CLASS_PUBLIC_MEMBERS          \
        Array_get_object    get_object;     \
        Array_append_object append_object;  \
        Array_delete_object delete_object;  \
        Array_get_count     get_count;      \
        Array_duplicate     duplicate;

struct _ArrayClass
{
    ARRAY_CLASS_PUBLIC_MEMBERS
};

typedef void    (*Array_object_destroyer)(Object);
typedef Object  (*Array_object_duplicator)(Object);

Array * new_array(Array_object_destroyer object_destroyer,
                  Array_object_duplicator object_duplicator);
void delete_array(Array * array);

// End Array Class declaration

#endif // _UTILITIES_H_
