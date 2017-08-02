// 0000 file created by Yarib Nevarez @ Gdl IBM
#ifndef _UTILITIES_C_
#define _UTILITIES_C_

#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utilities.h"

void ASSERT_TRACE(int exp, char * msg)
{
    // TODO: ASSERT and TRACE_MANAGER
    assert(exp);
}

void * mem_alloc(size_t size)
{
    return malloc(size);
}

void * mem_calloc(size_t num, size_t size)
{
    return calloc(num, size);
}

void * mem_realloc(void * mem_ptr, size_t size)
{
    return realloc(mem_ptr, size);
}

void mem_free(void * mem_ptr)
{
    free(mem_ptr);
}

void mem_set(void * mem_ptr, int value, size_t size)
{
    memset(mem_ptr, value, size);
}

void mem_cpy(void * mem_ptr_destination, const void * mem_ptr_source, size_t size)
{
    memcpy(mem_ptr_destination, mem_ptr_source, size);
}

void mem_move(void * mem_ptr_destination, const void * mem_ptr_source, size_t size)
{
    memmove(mem_ptr_destination, mem_ptr_source, size);
}

void * new_buffer(size_t size)
{
    void * pBuffer = NULL;

    if (size > 0)
    {
        pBuffer = mem_alloc(size);

        if (pBuffer != NULL)
        {
            mem_set(pBuffer, 0x0, size);
        }
    }

    return pBuffer;
}

void delete_buffer(void ** ppBuffer)
{
    if ((ppBuffer != NULL) && (*ppBuffer != NULL))
    {
        mem_free(*ppBuffer);

        *ppBuffer = NULL;
    }
}

void * duplicate_buffer(void * pBuffer, size_t size)
{
    void * buffer = NULL;

    if ((pBuffer != NULL) && (size > 0))
    {
        buffer = new_buffer(size);

        if (buffer != NULL)
        {
            mem_cpy(buffer, pBuffer, size);
        }
    }

    return buffer;
}

void * realloc_buffer(void * pBuffer, size_t size)
{
    return mem_realloc(pBuffer, size);
}

char * get_file_string(char file_name[])
{
    char * file_string = NULL;

    ASSERT_TRACE(file_name != NULL, "file_name != NULL");
    ASSERT_TRACE((file_name == NULL) || (strlen(file_name) < FILENAME_MAX),
                 "strlen(file_name) < FILENAME_MAX");

    if ((file_name != NULL) && (strlen(file_name) < FILENAME_MAX))
    {
        FILE * file;

        file = fopen(file_name, "r");

        ASSERT_TRACE(file != NULL, "file != NULL");

        if ((void *)file != NULL)
        {
            long int file_length;
            long int result;

            //result = fseek(file, 0, SEEK_END);

            //ASSERT_TRACE(result == 0, "result == 0");

            //file_length = ftell(file);

            // Change this mechanism
            for (file_length = 0; getc(file) != EOF; file_length ++);

            ASSERT_TRACE(file_length != (long int)(-1), "Invalid file_length value");
            //ASSERT_TRACE(feof(file) == 0, "feof(file) == 0");

            file_string = (char *) mem_alloc(sizeof(char) * (file_length + 1));

            ASSERT_TRACE(file_string != NULL, "file_string != NULL");

            if (file_string != NULL)
            {
                mem_set(file_string, 0x0, sizeof(char) * (file_length + 1));

                result = fseek(file, 0, SEEK_SET);

                ASSERT_TRACE(result == 0, "result == 0");

                result = (long int) fread(file_string, sizeof(char), file_length, file);

                ASSERT_TRACE(result == file_length, "result == file_length");
                //ASSERT_TRACE(feof(file) == 0, "feof(file) == 0");
            }

            fclose(file);
        }
    }

    return file_string;
}

void char_string_copy(char ** str, char * str_src, int str_len)
{
    ASSERT_TRACE(str != NULL, "str != NULL");

    if (str != NULL)
    {
        delete_buffer((void **)str);

        if ((str_src != NULL) && (str_len > 0))
        {
            *str = duplicate_buffer(str_src, sizeof(char) * (str_len + 1));

            ASSERT_TRACE(*str != NULL, "*str != NULL");

            if (*str != NULL)
            {
                (*str)[str_len] = '\0';
            }
        }
    }
}

char * duplicate_char_buffer(char * str)
{
    char * copy_str = NULL;

    if (str != NULL)
    {
        copy_str = (char *) duplicate_buffer((void *) str, sizeof(char) * (strlen(str) + 1));

        ASSERT_TRACE(memcmp(copy_str, str, sizeof(char) * (strlen(str) + 1)) == 0,
                    "memcmp(copy_str, str, sizeof(char) * (strlen(str) + 1)) == 0");
    }

    return copy_str;
}

void delete_char_buffer(char * str)
{
    if (str != NULL)
    {
        delete_buffer((void **)&(str));
    }
}

// TextViewPort object declaration

typedef struct
{
    char *  buffer;
    size_t  size;
} TextViewPort;

TextViewPort * pTextViewPort = NULL;

TextViewPort * new_TextViewPort()
{
    TextViewPort * text_view_port = new_buffer(sizeof(TextViewPort));

    ASSERT_TRACE(text_view_port != NULL, "text_view_port != NULL");

    return text_view_port;
}

void delete_TextViewPort(TextViewPort * text_view_port)
{
    if (text_view_port != NULL)
    {
        delete_buffer((void **) &(text_view_port->buffer));
        delete_buffer((void **) &text_view_port);
    }
}

void TextViewPort_appendText(char * text)
{
    TextViewPort * text_view_port = NULL;

    if (pTextViewPort == NULL)
    {
        pTextViewPort = new_TextViewPort();
    }

    text_view_port = pTextViewPort;

    ASSERT_TRACE(text_view_port != NULL, "text_view_port != NULL");
    ASSERT_TRACE(text != NULL, "text != NULL");

    if ((text_view_port != NULL) && (text != NULL) && (strlen(text) > 0))
    {
        char *  buffer = text_view_port->buffer;
        size_t  size = text_view_port->size;

        ASSERT_TRACE((buffer == NULL) || (strlen(buffer) <= size), "strlen(buffer) <= size");

        if ((buffer != NULL) && (strlen(text) + 1) < (size - strlen(buffer)))
        {
            mem_cpy(&buffer[strlen(buffer)], text, sizeof(char) * strlen(text));
        }
        else
        {
            char *  nBuffer;
            size_t  nSize;

            for (nSize = 1;
                 ((buffer == NULL) && (nSize < (sizeof(char) * (strlen(text) + 1))))
                 || ((buffer != NULL) && (nSize < (sizeof(char) * (strlen(buffer) + strlen(text) + 1))));
                 nSize *= 2);

            nBuffer = new_buffer(nSize);

            ASSERT_TRACE(nBuffer != NULL, "nBuffer != NULL");

            if (nBuffer != NULL)
            {
                if (buffer != NULL)
                {
                    mem_cpy(nBuffer, buffer, sizeof(char) * strlen(buffer));
                }

                mem_cpy(&nBuffer[strlen(nBuffer)], text, sizeof(char) * strlen(text));

                delete_buffer((void **) &buffer);

                text_view_port->buffer = nBuffer;
                text_view_port->size = nSize;
            }
        }
    }
}

char * TextViewPort_getBuffer()
{
    char * buffer = NULL;
    if (pTextViewPort != NULL)
    {
        buffer = pTextViewPort->buffer;
    }
    return buffer;
}

void TextViewPort_clear(void)
{
    delete_TextViewPort(pTextViewPort);
    pTextViewPort = NULL;
}

void error(Node * line, char * msg)
{
    char text[128];

    if (line != NULL)
    {
        Node * word;

        sprintf(text, "\n\n ** Error found in line: %d \n", get_node_index(line) + 1);

        TextViewPort_appendText(text);

        for (word = line->first_child; word != NULL; word = word->next)
        {
            sprintf(text, "%s ", (char *)word->data);
            TextViewPort_appendText(text);
        }
    }
    if (msg != NULL)
    {
        sprintf(text, "\n\"%s\"", msg);
        TextViewPort_appendText(text);
    }
}

// Array Class definition

typedef struct
{
    ARRAY_CLASS_PUBLIC_MEMBERS

    Array_object_destroyer  object_destroyer;
    Array_object_duplicator object_duplicator;
    Object *                data_objects;
    int                     count_objects;
} ArrayPrivate;

Object array_get_object(Array *, int);
void array_append_object(Array *, Object);
void array_delete_object(Array *, int);
int  array_get_count(Array *);
Array * duplicate_array(Array * array);

Array * new_array(Array_object_destroyer object_destroyer,
                  Array_object_duplicator object_duplicator)
{
    ArrayPrivate * array = NULL;

    ASSERT_TRACE(object_destroyer != NULL, "object_destroyer != NULL");

    if (object_destroyer != NULL)
    {
        array = new_buffer(sizeof(ArrayPrivate));

        ASSERT_TRACE(array != NULL, "array != NULL");

        if (array != NULL)
        {
            array->get_object = array_get_object;
            array->append_object = array_append_object;
            array->delete_object = array_delete_object;
            array->get_count = array_get_count;
            array->duplicate = duplicate_array;
            array->object_destroyer = object_destroyer;
            array->object_duplicator = object_duplicator;
        }
    }

    return (Array *) array;
}

void delete_array(Array * array)
{
    if (array != NULL)
    {
        ArrayPrivate *  array_private = (ArrayPrivate *) array;
        
        if (array_private->object_destroyer != NULL)
        {
            int i;

            for (i = 0; i < array_private->count_objects; i ++)
            {
                array_private->object_destroyer((array_private->data_objects)[i]);
            }
        }

        delete_buffer((void **)&(array_private->data_objects));

        delete_buffer((void **)&(array_private));
    }
}

Array * duplicate_array(Array * array)
{
    ArrayPrivate * copy_array = NULL;

    if ((array != NULL) && (((ArrayPrivate *)array)->object_duplicator != NULL))
    {
        ArrayPrivate * array_private = (ArrayPrivate *) array;

        copy_array = (ArrayPrivate *) new_array(array_private->object_destroyer,
                                                array_private->object_duplicator);

        ASSERT_TRACE(copy_array != NULL, "copy_array != NULL");

        if ((copy_array != NULL) && (array_private->count_objects > 0))
        {
            if (copy_array->object_duplicator != NULL)
            {
                Object  copy_object;
                int     i;

                for (i = 0; i < array_private->count_objects; i ++)
                {
                    copy_object = copy_array->object_duplicator(array_private->data_objects[i]);
                    array_append_object((Array *) copy_array, copy_object);
                }
            }
            else
            {
                ASSERT_TRACE(copy_array->data_objects == NULL, "copy_array->data_objects == NULL");
                copy_array->data_objects = duplicate_buffer(array_private->data_objects, sizeof(Object) * array_private->count_objects);

                ASSERT_TRACE(copy_array->data_objects != NULL, "copy_array->data_objects != NULL");

                if (copy_array->data_objects == NULL)
                {
                    ASSERT_TRACE(memcmp(copy_array->data_objects, array_private->data_objects, array_private->count_objects) == 0,
                                "memcmp(copy_array->data_objects, array_private->data_objects, array_private->count_objects) == 0");

                    copy_array->count_objects = array_private->count_objects;
                }
            }
        }
    }

    return (Array *) copy_array;
}

Object array_get_object(Array * array, int index)
{
    Object object = NULL;

    ASSERT_TRACE(array != NULL, "array != NULL");
    ASSERT_TRACE(0 <= index, "0 <= index");

    if ((array != NULL) && (0 <= index))
    {
        ArrayPrivate * array_private = (ArrayPrivate *) array;

        ASSERT_TRACE(index < array_private->count_objects, "index < array_private->count_objects");

        if (index < array_private->count_objects)
        {
            object = array_private->data_objects[index];
        }
    }

    return object;
}

void array_append_object(Array * array, Object object)
{
    ASSERT_TRACE(array != NULL, "array != NULL");
    ASSERT_TRACE(object != NULL, "object != NULL");

    if ((array != NULL) && (object != NULL))
    {
        ArrayPrivate *  array_private = (ArrayPrivate *) array;
        Object *        new_data_object;

        ASSERT_TRACE((array_private->data_objects != NULL) || (array_private->count_objects == 0),
                    "(array_private->data_objects != NULL) || (array_private->count_objects == 0)");
        ASSERT_TRACE((array_private->data_objects == NULL) || (0 < array_private->count_objects),
                    "(array_private->data_objects == NULL) || (0 < array_private->count_objects)");

        if (((array_private->data_objects != NULL) &&  (0 < array_private->count_objects))
         || ((array_private->data_objects == NULL) && (array_private->count_objects == 0)))
        {
            new_data_object = new_buffer(sizeof(Object) * (array_private->count_objects + 1));

            ASSERT_TRACE(new_data_object != NULL, "new_data_object != NULL");

            if (new_data_object != NULL)
            {
                if (0 < array_private->count_objects)
                {
                    ASSERT_TRACE(array_private->data_objects != NULL,
                                "array_private->data_objects != NULL");

                    if (array_private->data_objects != NULL)
                    {
                        mem_cpy(new_data_object,
                                array_private->data_objects,
                                sizeof(Object) * (array_private->count_objects));
                        
                        delete_buffer((void **)&(array_private->data_objects));
                    }
                }

                new_data_object[array_private->count_objects] = object;

                array_private->data_objects = new_data_object;

                array_private->count_objects ++;
            }
        }
    }
}

void array_delete_object(Array * array, int index)
{
    ASSERT_TRACE(array != NULL, "array != NULL");
    ASSERT_TRACE(0 <= index, "0 <= index");

    if ((array != NULL) && (0 <= index))
    {
        ArrayPrivate * array_private = (ArrayPrivate *) array;

        ASSERT_TRACE(index < array_private->count_objects, "index < array_private->count_objects");

        if ((index < array_private->count_objects))
        {
            if (1 < array_private->count_objects) // This should be done because of the ASSERT test
            {
                Object * new_data_object;

                new_data_object = new_buffer(sizeof(Object) * (array_private->count_objects - 1));

                ASSERT_TRACE(new_data_object != NULL, "new_data_object != NULL");

                if (new_data_object != NULL)
                {
                    if (array_private->object_destroyer != NULL)
                    {
                        array_private->object_destroyer((array_private->data_objects)[index]);
                    }

                    if (0 < index)
                    {
                        mem_cpy(new_data_object, array_private->data_objects, sizeof(Object) * index);
                    }

                    ASSERT_TRACE(!(0 < index) || (memcmp(new_data_object, array_private->data_objects, sizeof(Object) * index) == 0),
                                "!(0 < index) || (memcmp(new_data_object, array_private->data_objects, sizeof(Object) * index) == 0)");

                    if (index < (array_private->count_objects - 1))
                    {
                        mem_cpy(&new_data_object[index],
                                &array_private->data_objects[index + 1],
                                sizeof(Object) * (array_private->count_objects - (index + 1)));
                    }

                    ASSERT_TRACE(!(index < (array_private->count_objects - 1)) || (memcmp(&(new_data_object)[index], &(array_private->data_objects)[index + 1], sizeof(Object) * (array_private->count_objects - (index + 1))) == 0),
                                "!(index < (array_private->count_objects - 1)) || (memcmp(&(new_data_object)[index], &(array_private->data_objects)[index + 1], sizeof(Object) * (array_private->count_objects - (index + 1))) == 0)");

                    if ((!(0 < index) || (memcmp(new_data_object, array_private->data_objects, sizeof(Object) * index) == 0))
                    && (!(index < (array_private->count_objects - 1))
                        || (memcmp(&(new_data_object)[index], &(array_private->data_objects)[index + 1], sizeof(Object) * (array_private->count_objects - (index + 1))) == 0)))
                    {
                        delete_buffer((void **)&(array_private->data_objects));

                        array_private->data_objects = new_data_object;
                        array_private->count_objects --;
                    }
                }
            }
            else
            {
                if (array_private->object_destroyer != NULL)
                {
                    array_private->object_destroyer((array_private->data_objects)[0]);
                }
                delete_buffer((void **)&(array_private->data_objects));
                array_private->count_objects = 0;
            }
        }
    }
}

int array_get_count(Array * array)
{
    int count = (-1);
    
    ASSERT_TRACE(array != NULL, "array != NULL");

    if (array != NULL)
    {
        ArrayPrivate * array_private = (ArrayPrivate *) array;

        count = array_private->count_objects;
    }

    return count;
}

// End Array Classs definition


int evaluate_expression(char * str, int str_len, long int * value)
{
    int result = FAIL;

    if ((str != NULL) && (str_len > 0))
    {
        long int    val;
        long int    member;
        int         i;
        int         j;
        int         hex;
        int         depth = 1;

        typedef enum{
            OPTOKEN_START,
            OPTOKEN_NOTOKEN,
            OPTOKEN_PLUS,
            OPTOKEN_MUL,
            OPTOKEN_UNKNOUN,
            OPTOKEN_END
        } OpToken;

        OpToken op = OPTOKEN_NOTOKEN;

        result = SUCCESS;

        (*value) = 0;
        member   = 0;

        for (i = 0; (i < str_len) && (0 < depth) && (result == SUCCESS); i ++)
        {
            ASSERT_TRACE((OPTOKEN_START < op) && (op < OPTOKEN_END),
                        "(OPTOKEN_START < op) && (op < OPTOKEN_END)");

            if (isspace(str[i])); // To eat the spaces
            else if (str[i] == '(')
            {   // Evaluate subexpression
                depth ++;
                i ++;

                result = evaluate_expression(&str[i], str_len, &val);

                if (result == SUCCESS)
                {
                    if (op == OPTOKEN_NOTOKEN)
                    {   // set value member
                        if (member != 0)
                        {
                            result = FAIL;
                        }

                        member = val;
                    }
                    else if (op == OPTOKEN_MUL)
                    {   // Increase member PUSH
                        member *= val;
                        op = OPTOKEN_NOTOKEN;
                    }
                    else if (op == OPTOKEN_PLUS)
                    {   // Download member and set value PULL SET
                        (*value) += member;
                        member = val;
                        op = OPTOKEN_NOTOKEN;
                    }

                    while ((i < str_len) && (depth > 1))
                    {   // Adjust index according to the expression that has been evaluated
                        if (str[i] == '(')
                        {
                            depth ++;
                        }
                        if (str[i] == ')')
                        {
                            depth --;
                        }

                        i ++;
                    }

                    i--;
                }
            }
            else if (str[i] == ')')
            {
                depth --;
            }
            else if (str[i] == '+')
            {
                if (op != OPTOKEN_NOTOKEN)
                {
                    result = FAIL;
                }

                op = OPTOKEN_PLUS;
            }
            else if (str[i] == '*')
            {
                if (op != OPTOKEN_NOTOKEN)
                {
                    result = FAIL;
                }

                op = OPTOKEN_MUL;
            }
            else
            {
                hex = 0;
                if ((str[i] == '0') && ((str[i + 1] == 'x') || (str[i + 1] == 'X')))
                {
                    hex = 1;
                    i += 2;
                }

                for (j = i;
                    (j < str_len)
                    && ((('0' <= str[j]) && (str[j] <= '9'))
     || ((hex == 1) && ((('a' <= str[j]) && (str[j] <= 'f'))
                     || (('A' <= str[j]) && (str[j] <= 'F')))));
                    j ++);

                if (i < j)
                {
                    char * str_val = duplicate_buffer(&str[i], sizeof(char) * (j - i + 1));

                    ASSERT_TRACE(str_val != NULL, "str_val != NULL");

                    if (str_val != NULL)
                    {
                        str_val[j - i] = '\0';

                        if (hex == 0)
                        {
                            val = strtoul(str_val, NULL, 10);
                        }
                        else
                        {
                            val = strtoul(str_val, NULL, 16);
                        }

                        i = j - 1;

                        delete_buffer((void **)&str_val);

                        if (op == OPTOKEN_NOTOKEN)
                        {   // Set
                            if (member != 0)
                            {
                                result = FAIL;
                            }

                            member = val;
                        }
                        else if (op == OPTOKEN_MUL)
                        {   // Push
                            member *= val;
                            op = OPTOKEN_NOTOKEN;
                        }
                        else if (op == OPTOKEN_PLUS)
                        {   // Pull and Set
                            (*value) += member;
                            member = val;
                            op = OPTOKEN_NOTOKEN;
                        }
                    }
                }
                else
                {
                    result = FAIL;
                }
            }
        }
        // Pull
        (*value) += member;
    }

    return result;
}

#endif // _UTILITIES_C_
