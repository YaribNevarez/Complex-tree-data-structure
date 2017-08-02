// 0000 file created by Yarib Nevarez @ Gdl IBM
///////////////////////////////////////////////////////////////////////////////
// General overview.
// The parser_engine module was created to parse structure templates
// and map templates written in common C language with another specific
// instructions. The templates and the maps are used to format binary data.
// Module designed and written by Yarib Israel Nevarez IBM Gdl.
///////////////////////////////////////////////////////////////////////////////

#ifndef _parser_engine_c_
#define _parser_engine_c_

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utilities.h"
#include "parser_engine.h"
#include "node.h"

#define NUM_ELEMENTS_TOKEN_INFO_GROUP (sizeof(token_info_group) / sizeof(TokenInfo))

#define MAX_LENGTH_OF_TOKEN get_max_length_of_token(token_info_group, NUM_ELEMENTS_TOKEN_INFO_GROUP)

#define BITS_PER_BYTE 8

// Functions to handle NodeVTable data as char string

void vtable_data_set_string(const NodeVTable * vtable, Node * node, char * str, int str_len)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == vtable),
                 "node->vtable Wrong virtual table");

    if ((node != NULL) && (node->vtable == vtable))
    {
        delete_buffer(&(node->data));

        if ((str != NULL) && (str_len > 0))
        {
            node->data = duplicate_buffer((void *) str, sizeof(char) * (str_len + 1));

            ASSERT_TRACE(node->data != NULL, "node->data != NULL");

            if (node->data != NULL)
            {
                ((char *) (node->data))[str_len] = '\0';
            }
        }
    }
}

char * vtable_data_get_string(const NodeVTable * vtable, Node * node)
{
    char * str = NULL;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == vtable),
                 "node->vtable Wrong virtual table");

    if ((node != NULL) && (node->vtable == vtable))
    {
        str = (char *) node->data;
    }

    return str;
}

void vtable_data_duplicate_data(const NodeVTable * vtable, Node * node, Node * src_node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == vtable),
                 "node->vtable Wrong virtual table");
    ASSERT_TRACE((src_node == NULL) || (src_node->vtable == vtable),
                 "src_node->vtable Wrong virtual table");

    if ((node != NULL)
        && (node->vtable == vtable)
        && (src_node != NULL)
        && (src_node->vtable == vtable)
        && (src_node->data != NULL))
    {
        char *  src_str = (char *) src_node->data;

        node->data = (NodeData) duplicate_buffer((void *) src_str, sizeof(char) * (strlen(src_str) + 1));

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");
        ASSERT_TRACE((node->data == NULL) || (strlen((char *) node->data) == strlen((char *) src_node->data)),
                     "strlen((char *) node->data) == strlen((char *) src_node->data)");
        ASSERT_TRACE((node->data == NULL) || (strcmp((char *) src_node->data, (char *) node->data) == 0),
                     "strcmp((char *) src_node->data, (char *) node->data) == 0");
    }
}

void vtable_data_destroy_data(const NodeVTable * vtable, Node * node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == vtable),
                 "node->vtable Wrong virtual table");

    if ((node != NULL) && (node->vtable == vtable))
    {
        delete_buffer(&(node->data));
    }
}

// End Function to handle NodeVTable data as char string

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node VTable Document

extern const NodeVTable node_vtable_document;

void document_set_string(Node * node, char * str, int str_len)
{
    vtable_data_set_string(&node_vtable_document, node, str, str_len);
}

char * document_get_string(Node * node)
{
    return vtable_data_get_string(&node_vtable_document, node);
}

void document_duplicate_data(Node * node, Node * src_node)
{
    vtable_data_duplicate_data(&node_vtable_document, node, src_node);
}

void document_destroy_data(Node * node)
{
    vtable_data_destroy_data(&node_vtable_document, node);
}

const NodeVTable node_vtable_document =
{
    document_duplicate_data,
    document_destroy_data
};

// Node VTable Document
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node VTable Linde

const NodeVTable node_vtable_line =
{
    NULL,
    NULL
};

// Node VTable Line
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node VTable Word

extern const NodeVTable node_vtable_word;

void word_set_string(Node * node, char * str, int str_len)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_word),
                 "node->vtable == &node_vtable_word");

    if ((node != NULL) && (node->vtable == &node_vtable_word))
    {
        delete_buffer(&(node->data));

        if ((str != NULL) && (str_len > 0))
        {
            node->data = duplicate_buffer((void *) str, sizeof(char) * (str_len + 1));

            ASSERT_TRACE(node->data != NULL, "node->data != NULL");

            if (node->data != NULL)
            {
                ((char *) (node->data))[str_len] = '\0';
            }
        }
    }
}

char * word_get_string(Node * node)
{
    char * str = NULL;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_word),
                 "node->vtable == &node_vtable_word");

    if ((node != NULL) && (node->vtable == &node_vtable_word))
    {
        str = (char *) node->data;
    }

    return str;
}

void word_duplicate_data(Node * node, Node * src_node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_word),
                 "node->vtable == &node_vtable_word");
    ASSERT_TRACE((src_node == NULL) || (src_node->vtable == &node_vtable_word),
                 "src_node->vtable == &node_vtable_word");

    if ((node != NULL)
        && (node->vtable == &node_vtable_word)
        && (src_node != NULL)
        && (src_node->vtable == &node_vtable_word)
        && (src_node->data != NULL))
    {
        char *  src_str = (char *) src_node->data;

        node->data = (NodeData) duplicate_buffer((void *) src_str, sizeof(char) * (strlen(src_str) + 1));

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");
        ASSERT_TRACE((node->data == NULL) || (strlen((char *) node->data) == strlen((char *) src_node->data)),
                     "strlen((char *) node->data) == strlen((char *) src_node->data)");
        ASSERT_TRACE((node->data == NULL) || (strcmp((char *) src_node->data, (char *) node->data) == 0),
                     "strcmp((char *) src_node->data, (char *) node->data) == 0");
    }
}

void word_destroy_data(Node * node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_word),
                 "node->vtable == &node_vtable_word");

    if ((node != NULL) && (node->vtable == &node_vtable_word))
    {
        delete_buffer(&(node->data));
    }
}

const NodeVTable node_vtable_word =
{
    word_duplicate_data,
    word_destroy_data
};

// Node VTable Word
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node VTable Token

extern const NodeVTable node_vtable_token;

TokenInfo * new_tokenInfo()
{
    return new_buffer(sizeof(TokenInfo));
}

void delete_tokenInfo(TokenInfo * token_info)
{
    if (token_info != NULL)
    {
        delete_buffer((void **) &(token_info->string));
        delete_buffer((void **) &token_info);
    }
}

TokenInfo * duplicate_tokenInfo(TokenInfo * token_info)
{
    TokenInfo * copy_token_info = NULL;

    if (token_info != NULL)
    {
        copy_token_info = new_tokenInfo();

        ASSERT_TRACE(copy_token_info != NULL, "copy_token_info != NULL");

        if (copy_token_info != NULL)
        {
            mem_cpy(copy_token_info, token_info, sizeof(TokenInfo));

            ASSERT_TRACE(memcmp(copy_token_info, token_info, sizeof(TokenInfo)) == 0,
                        "memcmp(copy_token_info, token_info, sizeof(TokenInfo)) == 0");

            if (token_info->string != NULL)
            {
                copy_token_info->string = duplicate_char_buffer(token_info->string);

                ASSERT_TRACE(memcmp(copy_token_info->string, token_info->string, sizeof(char) * strlen(token_info->string)) == 0,
                            "memcmp(copy_token_info->string, token_info->string, sizeof(char) * strlen(token_info->string)) == 0");
            }
        }
    }

    return copy_token_info;
}

void token_set_string(Node * node, char * token_str, int token_len)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token), "node->vtable == &node_vtable_token");
    ASSERT_TRACE(token_str != NULL, "token_str != NULL");
    ASSERT_TRACE(token_len > 0, "token_len > 0");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (token_str != NULL)
        && (token_len > 0))
    {
        if (node->data == NULL)
        {
            node->data = new_buffer(sizeof(TokenInfo));
        }

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");

        if (node->data != NULL)
        {
            TokenInfo * token_info = (TokenInfo *) node->data;

            delete_buffer((void **)&(token_info->string));

            token_info->string = duplicate_buffer((void *) token_str, sizeof(char) * (token_len + 1));

            ASSERT_TRACE(token_info->string != NULL, "token_info->string != NULL");

            if (token_info->string != NULL)
            {
                (token_info->string)[token_len] = '\0';

                ASSERT_TRACE((strlen(token_info->string) == token_len),
                             "strlen(token_info->string) == token_len");
                ASSERT_TRACE(strncmp(token_info->string, token_str, token_len) == 0,
                             "strncmp(token_info->string, token_str, token_len) == 0");
            }
        }
    }
}

char * token_get_string(Node * node)
{
    char * str = NULL;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (node->data != NULL))
    {
        str = (char *) ((TokenInfo *) node->data)->string;
    }

    return str;
}

void token_set_type(Node * node, Token type)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");
    ASSERT_TRACE((TOKEN_START < type) && (type < TOKEN_END),
                 "(TOKEN_START < type) && (type < TOKEN_END)");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && ((TOKEN_START < type) && (type < TOKEN_END)))
    {
        if (node->data == NULL)
        {
            node->data = new_buffer(sizeof(TokenInfo));
        }

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");

        if (node->data != NULL)
        {
            ((TokenInfo *) node->data)->token = type;
        }
    }
}

Token token_get_type(Node * node)
{
    Token type = TOKEN_START;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (node->data != NULL))
    {
        type = ((TokenInfo *) node->data)->token;
    }

    return type;
}

void token_set_process(Node * node, TokenProcess process)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");

    if ((node != NULL) && (node->vtable == &node_vtable_token))
    {
        if (node->data == NULL)
        {
            node->data = new_buffer(sizeof(TokenInfo));
        }

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");

        if (node->data != NULL)
        {
            ((TokenInfo *) node->data)->process = process;
        }
    }
}

TokenProcess token_get_process(Node * node)
{
    TokenProcess process = NULL;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (node->data != NULL))
    {
        process = ((TokenInfo *) node->data)->process;
    }

    return process;
}

void token_set_position(Node * node, unsigned int position)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");

    if ((node != NULL) && (node->vtable == &node_vtable_token))
    {
        if (node->data == NULL)
        {
            node->data = new_buffer(sizeof(TokenInfo));
        }

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");

        if (node->data != NULL)
        {
            ((TokenInfo *) node->data)->position = position;
        }
    }
}

unsigned int token_get_position(Node * node)
{
    unsigned int position = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (node->data != NULL))
    {
        position = ((TokenInfo *) node->data)->position;
    }

    return position;
}

void token_set_fieldSize(Node * node, unsigned int fieldSize)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");

    if ((node != NULL) && (node->vtable == &node_vtable_token))
    {
        if (node->data == NULL)
        {
            node->data = new_buffer(sizeof(TokenInfo));
        }

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");

        if (node->data != NULL)
        {
            ((TokenInfo *) node->data)->fieldSize = fieldSize;
        }
    }
}

unsigned int token_get_fieldSize(Node * node)
{
    unsigned int fieldSize = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (node->data != NULL))
    {
        fieldSize = ((TokenInfo *) node->data)->fieldSize;
    }

    return fieldSize;
}

void token_duplicate_data(Node * node, Node * src_node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");
    ASSERT_TRACE((src_node == NULL) || (src_node->vtable == &node_vtable_token),
                 "src_node->vtable == &node_vtable_token");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (src_node != NULL)
        && (src_node->vtable == &node_vtable_token)
        && (src_node->data != NULL))
    {
        node->data = duplicate_tokenInfo(src_node->data);
    }
}

void token_destroy_data(Node * node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == &node_vtable_token),
                 "node->vtable == &node_vtable_token");

    if ((node != NULL)
        && (node->vtable == &node_vtable_token)
        && (node->data != NULL))
    {
        delete_tokenInfo((TokenInfo *) node->data);
        node->data = NULL;
    }
}

const NodeVTable node_vtable_token =
{
    token_duplicate_data,
    token_destroy_data
};

// Node VTable Token
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ASSERT_TRACE_TOKEN_NODE_HIERARCHY(node)                                             \
        ASSERT_TRACE((node) != NULL,                                                        \
                    "(node) != NULL");                                                      \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->data != NULL),                                                \
                     "(node)->data != NULL");                                               \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->vtable == &node_vtable_token),                                \
                     "(node)->vtable == &node_vtable_token");                               \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent != NULL),                                              \
                     "(node)->parent != NULL");                                             \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->data != NULL),                                        \
                     "(node)->parent->data != NULL");                                       \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->vtable == &node_vtable_word),                         \
                     "(node)->parent->vtable == &node_vtable_word");                        \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->parent != NULL),                                      \
                     "(node)->parent->parent != NULL");                                     \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->parent == NULL)                                       \
                  || ((node)->parent->parent->data == NULL),                                \
                     "(node)->parent->parent->data == NULL");                               \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->parent == NULL)                                       \
                  || ((node)->parent->parent->vtable == &node_vtable_line),                 \
                     "(node)->parent->parent->vtable == &node_vtable_line");                \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->parent == NULL)                                       \
                  || ((node)->parent->parent->parent != NULL),                              \
                     "(node)->parent->parent->parent != NULL");                             \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->parent == NULL)                                       \
                  || ((node)->parent->parent->parent == NULL)                               \
                  || ((node)->parent->parent->parent->data != NULL),                        \
                     "(node)->parent->parent->parent->data != NULL");                       \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->parent == NULL)                                       \
                  || ((node)->parent->parent->parent == NULL)                               \
                  || ((node)->parent->parent->parent->vtable == &node_vtable_document),     \
                     "(node)->parent->parent->parent->vtable == &node_vtable_document");    \
        ASSERT_TRACE(((node) == NULL)                                                       \
                  || ((node)->parent == NULL)                                               \
                  || ((node)->parent->parent == NULL)                                       \
                  || ((node)->parent->parent->parent == NULL)                               \
                  || ((node)->parent->parent->parent->parent == NULL),                      \
                     "(node)->parent->parent->parent->parent == NULL")

#define IS_VALID_DOCUMENT_NODE(node)                            \
                     (((node) != NULL)                          \
                   && ((node)->vtable == &node_vtable_document) \
                   && ((node)->data != NULL)                    \
                   && (0 < strlen((char *)(node)->data)))

#define IS_VALID_LINE_NODE(node)                                \
                     (((node) != NULL)                          \
                  &&  ((node)->vtable == &node_vtable_line)     \
                  &&  ((node)->data == NULL))

#define IS_VALID_WORD_NODE(node)                                \
                     (((node) != NULL)                          \
                  &&  ((node)->vtable == &node_vtable_word)     \
                  &&  ((node)->data != NULL)                    \
                  &&  (0 < strlen((char *)(node)->data)))

#define IS_VALID_TOKEN_NODE(node)                               \
                     (((node) != NULL)                          \
                  &&  ((node)->vtable == &node_vtable_token)    \
                  &&  ((node)->data != NULL))

#define IS_VALID_TOKEN_NODE_HIERARCHY(node)                     \
                  (IS_VALID_TOKEN_NODE(node)                    \
                && IS_VALID_WORD_NODE((node)->parent)           \
                && IS_VALID_LINE_NODE((node)->parent->parent)   \
                && IS_VALID_DOCUMENT_NODE((node)->parent->parent->parent))


// Parser implementation

void * process_STRUCT_TEMPLATE(Node **, void *);
void * process_MAP_TEMPLATE(Node **, void *);

void * process_TEMPLATE_INSTRUCTION(Node ** pNode, void * context);
void * process_QUOT(Node ** pNode, void * context);
void * process_MAP(Node ** pNode, void * context);
void * process_STRING(Node ** pNode, void * context);
void * process_HEX_DUMP(Node ** pNode, void * context);
void * process_PAYLOAD_TRAILER(Node ** pNode, void * context);

void * process_struct_field(Node ** pNode, void * context);
void * process_union_field(Node ** pNode, void * context);
void * process_member_field(Node ** pNode, void * context);

TokenInfo token_info_group[] =
{// {token_type,                    process_func,                           lexeme,             absolute_position_in_document,  size_in_bytes_memory_field}
    {TOKEN_auto,                    NULL,                                   "auto",             UNSIGNED_INT_LIMIT,             0},
    {TOKEN_double,                  process_member_field,                   "double",           UNSIGNED_INT_LIMIT,             8},
    {TOKEN_int,                     process_member_field,                   "int",              UNSIGNED_INT_LIMIT,             4},
    {TOKEN_struct,                  process_struct_field,                   "struct",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_break,                   NULL,                                   "break",            UNSIGNED_INT_LIMIT,             0},
    {TOKEN_else,                    NULL,                                   "else",             UNSIGNED_INT_LIMIT,             0},
    {TOKEN_long,                    NULL,                                   "long",             UNSIGNED_INT_LIMIT,             0},
    {TOKEN_switch,                  NULL,                                   "switch",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_case,                    NULL,                                   "case",             UNSIGNED_INT_LIMIT,             0},
    {TOKEN_enum,                    NULL,                                   "enum",             UNSIGNED_INT_LIMIT,             0},
    {TOKEN_register,                NULL,                                   "register",         UNSIGNED_INT_LIMIT,             0},
    {TOKEN_typedef,                 NULL,                                   "typedef",          UNSIGNED_INT_LIMIT,             0},
    {TOKEN_char,                    process_member_field,                   "char",             UNSIGNED_INT_LIMIT,             1},
    {TOKEN_extern,                  NULL,                                   "extern",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_return,                  NULL,                                   "return",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_union,                   process_union_field,                    "union",            UNSIGNED_INT_LIMIT,             0},
    {TOKEN_const,                   NULL,                                   "const",            UNSIGNED_INT_LIMIT,             0},
    {TOKEN_float,                   process_member_field,                   "float",            UNSIGNED_INT_LIMIT,             4},
    {TOKEN_short,                   process_member_field,                   "short",            UNSIGNED_INT_LIMIT,             2},
    {TOKEN_unsigned,                process_member_field,                   "unsigned",         UNSIGNED_INT_LIMIT,             4},
    {TOKEN_continue,                NULL,                                   "continue",         UNSIGNED_INT_LIMIT,             0},
    {TOKEN_for,                     NULL,                                   "for",              UNSIGNED_INT_LIMIT,             0},
    {TOKEN_signed,                  process_member_field,                   "signed",           UNSIGNED_INT_LIMIT,             4},
    {TOKEN_void,                    process_member_field,                   "void",             UNSIGNED_INT_LIMIT,             4},
    {TOKEN_default,                 NULL,                                   "default",          UNSIGNED_INT_LIMIT,             0},
    {TOKEN_goto,                    NULL,                                   "goto",             UNSIGNED_INT_LIMIT,             0},
    {TOKEN_sizeof,                  NULL,                                   "sizeof",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_volatile,                NULL,                                   "volatile",         UNSIGNED_INT_LIMIT,             0},
    {TOKEN_do,                      NULL,                                   "do",               UNSIGNED_INT_LIMIT,             0},
    {TOKEN_if,                      NULL,                                   "if",               UNSIGNED_INT_LIMIT,             0},
    {TOKEN_static,                  NULL,                                   "static",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_while,                   NULL,                                   "while",            UNSIGNED_INT_LIMIT,             0},
    {TOKEN_NUMSIGN,                 NULL,                                   "#",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_define,                  NULL,                                   "define",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_QUOT,                    process_QUOT,                           "\"",               UNSIGNED_INT_LIMIT,             0},
    {TOKEN_REVERSE_SOLIDUS,         NULL,                                   "\\",               UNSIGNED_INT_LIMIT,             0},
    {TOKEN_STRUCT_TEMPLATE,         process_STRUCT_TEMPLATE,                "STRUCT_TEMPLATE_", UNSIGNED_INT_LIMIT,             0},
    {TOKEN_TEMPLATE_INSTRUCTION,    process_TEMPLATE_INSTRUCTION,           "//^",              UNSIGNED_INT_LIMIT,             0},
    {TOKEN_DEFAULT,                 NULL,                                   "DEFAULT",          UNSIGNED_INT_LIMIT,             0},
    {TOKEN_ERROR,                   NULL,                                   "ERROR",            UNSIGNED_INT_LIMIT,             0},
    {TOKEN_COLON,                   NULL,                                   ":",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_SEMICOLON,               NULL,                                   ";",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_END_TEMPLATE,            NULL,                                   "END_TEMPLATE",     UNSIGNED_INT_LIMIT,             0},
    {TOKEN_MAP_TEMPLATE,            process_MAP_TEMPLATE,                   "MAP_TEMPLATE",     UNSIGNED_INT_LIMIT,             0},
    {TOKEN_MAP,                     process_MAP,                            "MAP",              UNSIGNED_INT_LIMIT,             0},
    {TOKEN_END_MAP,                 NULL,                                   "END_MAP",          UNSIGNED_INT_LIMIT,             0},
    {TOKEN_LEFT_CURLY_BRACKET,      NULL,                                   "{",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_RIGHT_CURLY_BRACKET,     NULL,                                   "}",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_LEFT_SQUARE_BRACKET,     NULL,                                   "[",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_RIGHT_SQUARE_BRACKET,    NULL,                                   "]",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_LEFT_PARENTHESIS,        NULL,                                   "(",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_RIGHT_PARENTHESIS,       NULL,                                   ")",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_COMMENT,                 NULL,                                   "//",               UNSIGNED_INT_LIMIT,             0},
    {TOKEN_COMMENT_OPEN,            NULL,                                   "/*",               UNSIGNED_INT_LIMIT,             0},
    {TOKEN_COMMENT_CLOSE,           NULL,                                   "*/",               UNSIGNED_INT_LIMIT,             0},
    {TOKEN_COMMA,                   NULL,                                   ",",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_EQUALS_SIGN,             NULL,                                   "=",                UNSIGNED_INT_LIMIT,             0},
    {TOKEN_UINT32,                  process_member_field,                   "UINT32",           UNSIGNED_INT_LIMIT,             4},
    {TOKEN_UINT16,                  process_member_field,                   "UINT16",           UNSIGNED_INT_LIMIT,             2},
    {TOKEN_UINT8,                   process_member_field,                   "UINT8",            UNSIGNED_INT_LIMIT,             1},
    {TOKEN_INT32,                   process_member_field,                   "INT32",            UNSIGNED_INT_LIMIT,             4},
    {TOKEN_INT16,                   process_member_field,                   "INT16",            UNSIGNED_INT_LIMIT,             2},
    {TOKEN_INT8,                    process_member_field,                   "INT8",             UNSIGNED_INT_LIMIT,             1},
    {TOKEN_BOOLEAN,                 process_member_field,                   "BOOLEAN",          UNSIGNED_INT_LIMIT,             1},
    {TOKEN_Int,                     process_member_field,                   "Int",              UNSIGNED_INT_LIMIT,             4},
    {TOKEN_Short,                   process_member_field,                   "Short",            UNSIGNED_INT_LIMIT,             2},
    {TOKEN_Char,                    process_member_field,                   "Char",             UNSIGNED_INT_LIMIT,             1},
    {TOKEN_STRING,                  process_STRING,                         "STRING",           UNSIGNED_INT_LIMIT,             0},
    {TOKEN_HEX_DUMP,                process_HEX_DUMP,                       "HEX_DUMP",         UNSIGNED_INT_LIMIT,             0},
    {TOKEN_PAYLOAD_TRAILER,         process_PAYLOAD_TRAILER,                "PAYLOAD_TRAILER",  UNSIGNED_INT_LIMIT,             0},
    {TOKEN_UNKNOWN,                 NULL,                                   NULL,               UNSIGNED_INT_LIMIT,             0}
};

#define IS_FIELD_TOKEN_TYPE(token_type) \
    (  ((token_type) == TOKEN_double)   \
    || ((token_type) == TOKEN_int)      \
    || ((token_type) == TOKEN_struct)   \
    || ((token_type) == TOKEN_char)     \
    || ((token_type) == TOKEN_union)    \
    || ((token_type) == TOKEN_float)    \
    || ((token_type) == TOKEN_short)    \
    || ((token_type) == TOKEN_unsigned) \
    || ((token_type) == TOKEN_signed)   \
    || ((token_type) == TOKEN_void)     \
    || ((token_type) == TOKEN_UINT32)   \
    || ((token_type) == TOKEN_UINT16)   \
    || ((token_type) == TOKEN_UINT8)    \
    || ((token_type) == TOKEN_INT32)    \
    || ((token_type) == TOKEN_INT16)    \
    || ((token_type) == TOKEN_INT8)     \
    || ((token_type) == TOKEN_BOOLEAN)  \
    || ((token_type) == TOKEN_Int)      \
    || ((token_type) == TOKEN_Short)    \
    || ((token_type) == TOKEN_Char))

#define IS_VALID_FIELD_TOKEN_NODE(token_node) (IS_VALID_TOKEN_NODE_HIERARCHY(token_node) \
                                         && IS_FIELD_TOKEN_TYPE(((TokenInfo *)((token_node)->data))->token))

// TInstruction Definition


TInstruction * new_tinstruction(void)
{
    TInstruction * template_instruction;
    
    template_instruction = new_buffer(sizeof(TInstruction));

    ASSERT_TRACE(template_instruction != NULL,
                "template_instruction != NULL");

    if (template_instruction != NULL)
    {
        template_instruction->array_format_string = new_array(delete_char_buffer, duplicate_char_buffer);
        template_instruction->array_token_info = new_array(delete_tokenInfo, duplicate_tokenInfo);
    }

    return template_instruction;
}

void delete_tinstruction(TInstruction * template_instruction)
{
    if (template_instruction != NULL)
    {
        delete_array(template_instruction->array_format_string);

        delete_buffer((void **) &(template_instruction->map_name));

        delete_array(template_instruction->array_token_info);

        delete_node_path(template_instruction->data_path);

        delete_buffer((void **) &(template_instruction));
    }
}

TInstruction * duplicate_tinstruction(TInstruction * template_instruction)
{
    TInstruction * copy_template_instruction = NULL;

    if (template_instruction != NULL)
    {
        copy_template_instruction = new_buffer(sizeof(TInstruction));
        
        ASSERT_TRACE(copy_template_instruction != NULL,
                    "copy_template_instruction != NULL");

        if (copy_template_instruction != NULL)
        {
            copy_template_instruction->array_format_string =
                template_instruction->array_format_string->duplicate(template_instruction->array_format_string);

            copy_template_instruction->map_name = duplicate_char_buffer(template_instruction->map_name);

            copy_template_instruction->array_token_info =
                template_instruction->array_token_info->duplicate(template_instruction->array_token_info);

            copy_template_instruction->iterations = template_instruction->iterations;

            copy_template_instruction->data_path = duplicate_node_path(template_instruction->data_path);
        }
    }

    return copy_template_instruction;
}

// Struct Template Definition

Template * new_template(char *   id_name,
                        long int id_value,
                        Node *   struct_tree,
                        Array *  array_template_instructions)
{
    Template * struct_template;

    struct_template = new_buffer(sizeof(Template));

    ASSERT_TRACE(struct_template != NULL, "struct_template != NULL");

    if (struct_template != NULL)
    {
        struct_template->id_name = duplicate_char_buffer(id_name);
        struct_template->id_value = id_value;
        struct_template->struct_tree = struct_tree;

        if (array_template_instructions != NULL)
        {
            struct_template->array_template_instructions = array_template_instructions;
        }
        else
        {
            struct_template->array_template_instructions = new_array(delete_tinstruction, duplicate_tinstruction);
        }
    }

    return struct_template;
}

void delete_template(Template * struct_template)
{
    if (struct_template != NULL)
    {
        delete_buffer((void **) &(struct_template->id_name));
        destroy_tree(struct_template->struct_tree);

        delete_array(struct_template->array_template_instructions);
        delete_buffer((void **) &(struct_template));
    }
}

Template * duplicate_template(Template * struct_template)
{
    Template * copy_struct_template = NULL;

    if (struct_template != NULL)
    {
        copy_struct_template = new_buffer(sizeof(Template));

        ASSERT_TRACE(copy_struct_template != NULL,
                    "copy_struct_template != NULL");

        if (copy_struct_template != NULL)
        {
            copy_struct_template->id_name = duplicate_char_buffer(struct_template->id_name);
            copy_struct_template->id_value = struct_template->id_value;
            copy_struct_template->struct_tree = duplicate_tree(struct_template->struct_tree);
            copy_struct_template->array_template_instructions =
                struct_template->array_template_instructions->duplicate(struct_template->array_template_instructions);
        }
    }

    return copy_struct_template;
}

// MapElement Template Definition


MapElement * new_mapElement(void)
{
    return (MapElement *) new_buffer(sizeof(MapElement));
}

void delete_mapElement(MapElement * map_element)
{
    if (map_element != NULL)
    {
        delete_buffer((void **) &(map_element->name));
        delete_buffer((void **) &(map_element->format_string));
        delete_buffer((void **) &map_element);
    }
}

MapElement * duplicate_mapElement(MapElement * map_element)
{
    MapElement * copy_mapElement = NULL;

    if (map_element != NULL)
    {
        copy_mapElement = new_mapElement();

        ASSERT_TRACE(copy_mapElement != NULL, "copy_mapElement != NULL");

        if (copy_mapElement != NULL)
        {
            copy_mapElement->format_string = duplicate_char_buffer(map_element->format_string);

            copy_mapElement->name = duplicate_char_buffer(map_element->name);

            copy_mapElement->value = map_element->value;
        }
    }

    return copy_mapElement;
}
// Map Template Definition

Map * new_map(void)
{
    Map * map;

    map = (Map *) new_buffer(sizeof(Map));

    ASSERT_TRACE(map != NULL, "map != NULL");

    if (map != NULL)
    {
        map->array_map_elements = new_array(delete_mapElement, duplicate_mapElement);
    }

    return map;
}

void delete_map(Map * map)
{
    if (map != NULL)
    {
        delete_buffer((void **) &(map->id_name));

        delete_array(map->array_map_elements);

        delete_buffer((void **) &(map->default_string));

        delete_buffer((void **) &map);
    }
}

Map * duplicate_map(Map * map)
{
    Map * copy_map = NULL;

    if (map != NULL)
    {
        copy_map = new_buffer(sizeof(Map));

        ASSERT_TRACE(copy_map != NULL, "copy_map != NULL");

        if (copy_map != NULL)
        {
            copy_map->id_name = duplicate_char_buffer(map->id_name);
            copy_map->array_map_elements = map->array_map_elements->duplicate(map->array_map_elements);
            copy_map->default_string = duplicate_char_buffer(map->default_string);
        }
    }

    return copy_map;
}

// SymbolTable Definition

SymbolTable * new_symbolTable(void)
{
    SymbolTable * symbol_table;

    symbol_table = new_buffer(sizeof(SymbolTable));

    ASSERT_TRACE(symbol_table != NULL, "symbol_table != NULL");

    if (symbol_table != NULL)
    {
        symbol_table->array_template = new_array(delete_template, duplicate_template);
        symbol_table->array_map = new_array(delete_map, duplicate_map);
    }

    return symbol_table;
}

void delete_symbolTable(SymbolTable * symbolTable)
{
    if (symbolTable != NULL)
    {
        delete_array(symbolTable->array_template);
        delete_array(symbolTable->array_map);

        delete_buffer(&symbolTable);
    }
}

SymbolTable * duplicate_symbolTable(SymbolTable * symbol_table)
{
    SymbolTable * copy_symbol_table = NULL;

    if (symbol_table != NULL)
    {
        copy_symbol_table = new_buffer(sizeof(SymbolTable));

        ASSERT_TRACE(copy_symbol_table != NULL, "copy_symbol_table != NULL");

        if (copy_symbol_table != NULL)
        {
            copy_symbol_table->array_template = symbol_table->array_template->duplicate(symbol_table->array_template);

            copy_symbol_table->array_map = symbol_table->array_map->duplicate(symbol_table->array_map);
        }
    }

    return copy_symbol_table;
}

#define IS_VALID_SYMBOL_TABLE(symbol_table)         \
        (((symbol_table) != NULL)                   \
      && ((symbol_table)->array_template != NULL)   \
      && ((symbol_table)->array_map != NULL))

#define ASSERT_TRACE_SYMBOL_TABLE(symbol_table)                                             \
        ASSERT_TRACE((symbol_table) != NULL, "symbol_table != NULL");                       \
        ASSERT_TRACE(((symbol_table) == NULL) || ((symbol_table)->array_template != NULL),  \
                                               "symbol_table->array_template != NULL");     \
        ASSERT_TRACE(((symbol_table) == NULL) || ((symbol_table)->array_map != NULL),       \
                                               "symbol_table->array_map != NULL");

//_____________________ Struct Template hierarchy data _____________________

typedef unsigned int (*GetFieldSize)        (Node *);
typedef int          (*GetFieldChildOffset) (Node *, Node *);

typedef struct
{
    NODE_VTABLE_MEMBERS
    GetFieldSize        get_field_size;
    GetFieldChildOffset get_field_child_offset;
} NodeVTableField;

extern const NodeVTableField node_vtable_struct_field;
extern const NodeVTableField node_vtable_union_field;
extern const NodeVTableField node_vtable_member_field;
extern const NodeVTableField node_vtable_bit_field;

#define IS_FIELD_VTABLE(vtable) (  ((vtable) == &node_vtable_struct_field)  \
                                || ((vtable) == &node_vtable_union_field)   \
                                || ((vtable) == &node_vtable_member_field)  \
                                || ((vtable) == &node_vtable_bit_field)  )

#define IS_FIELD_NODE(node)         (((node) != NULL) && IS_FIELD_VTABLE((NodeVTableField *)(node)->vtable))
#define IS_VALID_FIELD_NODE(node)   (IS_FIELD_NODE(node) && ((node)->data != NULL))

FieldData * new_fieldData()
{
    FieldData * field_data = new_buffer(sizeof(FieldData));

    ASSERT_TRACE(field_data != NULL, "field_data != NULL");

    if (field_data != NULL)
    {
        field_data->array_token_info = new_array(delete_tokenInfo, duplicate_tokenInfo);
        ASSERT_TRACE(field_data->array_token_info != NULL, "field_data->array_token_info != NULL");
    }

    return field_data;
}

void delete_fieldData(FieldData * field_data)
{
    if (field_data != NULL)
    {
        delete_buffer((void **) &(field_data->id_name));
        delete_array(field_data->array_token_info);
        delete_buffer((void **) &field_data);
    }
}

FieldData * duplicate_fieldData(FieldData * field_data)
{
    FieldData * copy_field_data = NULL;
    if (field_data != NULL)
    {
        copy_field_data = duplicate_buffer(field_data, sizeof(FieldData));

        ASSERT_TRACE(copy_field_data != NULL, "copy_field_data != NULL");
        ASSERT_TRACE((copy_field_data == NULL) || (memcmp(copy_field_data, field_data, sizeof(FieldData)) == 0),
                                                  "memcmp(copy_field_data, field_data, sizeof(FieldData)) == 0");

        if (copy_field_data != NULL)
        {
            copy_field_data->id_name = duplicate_char_buffer(field_data->id_name);
            copy_field_data->array_token_info = field_data->array_token_info->duplicate(field_data->array_token_info);
        }
    }

    return copy_field_data;
}

void node_vtable_field_duplicate_data(Node * node, Node * src_node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || IS_FIELD_NODE(node), "IS_FIELD_NODE(node)");

    if ((node != NULL) && IS_FIELD_NODE(node))
    {
        ASSERT_TRACE((src_node == NULL) || IS_FIELD_NODE(src_node), "IS_FIELD_NODE(src_node)");

        if ((src_node != NULL)
            && IS_FIELD_NODE(src_node))
        {
            delete_fieldData((FieldData *) node->data);
            node->data = duplicate_fieldData((FieldData *)src_node->data);
        }
        else
        {
            node->data = new_fieldData();
        }

        ASSERT_TRACE(node->data != NULL, "node->data != NULL");
    }
}

void node_vtable_field_destroy_data(Node * node)
{
    
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || IS_FIELD_NODE(node), "IS_FIELD_NODE(node)");

    if ((node != NULL)
        && IS_FIELD_NODE(node)
        && (node->data != NULL))
    {
        delete_fieldData((FieldData *) node->data);
    }
}

unsigned int field_node_get_size(Node * node)
{
    unsigned int size = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || IS_FIELD_NODE(node), "IS_FIELD_NODE(node)");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");

    if (IS_VALID_FIELD_NODE(node))
    {
        FieldData * field_data = (FieldData *) node->data;

        size = field_data->field_size;

        if (size == 0)
        {
            size = ((NodeVTableField *)(node->vtable))->get_field_size(node);

            field_data->field_size = size;
        }
    }

    return size;
}

int field_node_get_offset(Node * node)
{
    int offset = 0;
    
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || IS_FIELD_NODE(node), "IS_FIELD_NODE(node)");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");

    if (IS_VALID_FIELD_NODE(node))
    {
        FieldData * field_data = (FieldData *) node->data;

        offset = field_data->field_offset;

        ASSERT_TRACE((node->parent == NULL) || IS_VALID_FIELD_NODE(node->parent),
                                              "IS_VALID_FIELD_NODE(node->parent)");

        if ((offset == 0) && IS_VALID_FIELD_NODE(node->parent))
        {
            offset = ((NodeVTableField *)(node->parent->vtable))->get_field_child_offset(node->parent, node);

            field_data->field_offset = offset;
        }
    }

    return offset;
}

// ______ node_vtable_struct_field ______ Virtual functions definition

unsigned int get_struct_size(Node * node)
{
    unsigned int size = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == (const NodeVTable *) &node_vtable_struct_field),
                                   "node->vtable == &node_vtable_struct_field");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");
    ASSERT_TRACE((node == NULL) || (node->first_child != NULL), "node->first_child != NULL");

    if ((node != NULL)
        && (node->vtable == (const NodeVTable *) &node_vtable_struct_field)
        && (node->data != NULL)
        && (node->first_child != NULL))
    {
        Node * child;

        for (child = node->first_child; child != NULL; child = child->next)
        {
            ASSERT_TRACE(IS_FIELD_NODE(child), "IS_FIELD_NODE(child)");

            if (IS_FIELD_NODE(child))
            {
                size += field_node_get_size(child);
            }
        }
    }

    return size;
}

int get_struct_child_offset(Node * node, Node * child)
{
    int offset = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == (const NodeVTable *) &node_vtable_struct_field),
                                   "node->vtable == &node_vtable_struct_field");
    ASSERT_TRACE((node == NULL) || (node == child->parent), "node == child->parent");

    if ((node != NULL)
        && (node->vtable == (const NodeVTable *) &node_vtable_struct_field)
        && (node == child->parent))
    {
        offset = field_node_get_offset(node);

        if (child->prev != NULL)
        {
            for (child = child->prev; child != NULL; child = child->prev)
            {
                ASSERT_TRACE(IS_FIELD_NODE(child), "IS_FIELD_NODE(child)");

                if (IS_FIELD_NODE(child))
                {
                    offset += field_node_get_size(child);
                }
            }
        }
    }

    return offset;
}

const NodeVTableField node_vtable_struct_field =
{
    node_vtable_field_duplicate_data,
    node_vtable_field_destroy_data,
    get_struct_size,
    get_struct_child_offset
};


// ______ node_vtable_union_field ______ Virtual functions definition

unsigned int get_union_size(Node * node)
{
    unsigned int size = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == (const NodeVTable *) &node_vtable_union_field),
                                   "node->vtable == &node_vtable_union_field");
    ASSERT_TRACE((node == NULL) || (node->data != NULL), "node->data != NULL");
    ASSERT_TRACE((node == NULL) || (node->first_child != NULL), "node->first_child != NULL");

    if ((node != NULL)
        && (node->vtable == (const NodeVTable *) &node_vtable_union_field)
        && (node->data != NULL)
        && (node->first_child != NULL))
    {
        Node *          child;
        unsigned int    child_size;

        for (child = node->first_child; child != NULL; child = child->next)
        {
            ASSERT_TRACE(IS_FIELD_NODE(child), "IS_FIELD_NODE(child)");

            if (IS_FIELD_NODE(child))
            {
                child_size = field_node_get_size(child);

                if (size < child_size)
                {
                    size = child_size;
                }
            }
        }
    }

    return size;
}

int get_union_child_offset(Node * node, Node * child)
{
    int offset = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == (const NodeVTable *) &node_vtable_union_field),
                                   "node->vtable == &node_vtable_union_field");
    ASSERT_TRACE((node == NULL) || (node == child->parent), "node == child->parent");

    if ((node != NULL)
        && (node->vtable == (const NodeVTable *) &node_vtable_union_field)
        && (node == child->parent))
    {
        offset = field_node_get_offset(node);
    }

    return offset;
}

const NodeVTableField node_vtable_union_field =
{
    node_vtable_field_duplicate_data,
    node_vtable_field_destroy_data,
    get_union_size,
    get_union_child_offset
};


// ______ node_vtable_member_field ______ Virtual functions definition

#define ASSERT_TRACE_MEMBER_FIELD_NODE(member)                                                                                  \
    ASSERT_TRACE((member) != NULL, "member != NULL");                                                                           \
    ASSERT_TRACE(((member) != NULL) || ((member)->vtable == (const NodeVTable *) &node_vtable_member_field),                                         \
                                        "member ->vtable == (const NodeVTable *) &node_vtable_member_field");                                        \
    ASSERT_TRACE(((member) == NULL) || ((member)->data != NULL),                                                                \
                                        "member ->data != NULL");                                                               \
    ASSERT_TRACE(((member) == NULL) || ((member)->data == NULL) || (((FieldData *)((member)->data))->array_token_info != NULL), \
                                                                   "((FieldData *)( member ->data))->array_token_info != NULL")

#define IS_VALID_MEMBER_FIELD_NODE(member)                                  \
    (  ((member) != NULL)                                                   \
    && ((member)->vtable == (const NodeVTable *) &node_vtable_member_field) \
    && ((member)->data != NULL)                                             \
    && (((FieldData *)((member)->data))->array_token_info != NULL))

unsigned int get_member_size(Node * node)
{
    unsigned int size = 0;

    ASSERT_TRACE(node !=  NULL, "node !=  NULL");
    ASSERT_TRACE((node ==  NULL) || (node->vtable == (const NodeVTable *) &node_vtable_member_field),
                                    "node->vtable == &node_vtable_member_field");
    ASSERT_TRACE((node ==  NULL) || (node->data != NULL), "node->data != NULL");

    if ((node != NULL)
        && (node->vtable == (const NodeVTable *) &node_vtable_member_field)
        && (node->data != NULL))
    {
        FieldData * field_data = node->data;
        Array *     array_token = field_data->array_token_info;

        ASSERT_TRACE(array_token != NULL, "array_token != NULL");

        if (array_token != NULL)
        {
            int         i;

            i = array_token->get_count(array_token);

            ASSERT_TRACE(i > 0, "There are no items in the token array");

            if (i > 0)
            {
                TokenInfo * token_info = (TokenInfo *) array_token->get_object(array_token, i - 1);

                ASSERT_TRACE(token_info != NULL, "token_info != NULL");

                if (token_info != NULL)
                {
                    size = token_info->fieldSize;
                }

                if (field_data->num_elements > 0)
                {
                    size = field_data->num_elements * size;
                }

                ASSERT_TRACE(size > 0, "The token info corresponding to the given token doesn't have a valid size");
            }
        }
    }

    return size;
}

const NodeVTableField node_vtable_member_field =
{
    node_vtable_field_duplicate_data,
    node_vtable_field_destroy_data,
    get_member_size,
    NULL
};

// ______ node_vtable_bit_field ______ Virtual functions definition

#define ASSERT_TRACE_BIT_FIELD_NODE(member)                                                                                  \
    ASSERT_TRACE((member) != NULL, "member != NULL");                                                                           \
    ASSERT_TRACE(((member) != NULL) || ((member)->vtable == (const NodeVTable *) &node_vtable_bit_field),                                         \
                                        "member ->vtable == (const NodeVTable *) &node_vtable_bit_field");                                        \
    ASSERT_TRACE(((member) == NULL) || ((member)->data != NULL),                                                                \
                                        "member ->data != NULL");                                                               \
    ASSERT_TRACE(((member) == NULL) || ((member)->data == NULL) || (((FieldData *)((member)->data))->array_token_info != NULL), \
                                                                   "((FieldData *)( member ->data))->array_token_info != NULL")

#define IS_VALID_BIT_FIELD_NODE(member)                                     \
    (  ((member) != NULL)                                                   \
    && ((member)->vtable == (const NodeVTable *) &node_vtable_bit_field)    \
    && ((member)->data != NULL)                                             \
    && (((FieldData *)((member)->data))->array_token_info != NULL))

unsigned int get_bit_field_size(Node * node)
{
    unsigned int size = 0;

    ASSERT_TRACE(node !=  NULL, "node !=  NULL");
    ASSERT_TRACE((node ==  NULL) || (node->vtable == (const NodeVTable *) &node_vtable_member_field),
                                    "node->vtable == &node_vtable_bit_field");
    ASSERT_TRACE((node ==  NULL) || (node->data != NULL), "node->data != NULL");

    if ((node != NULL)
        && (node->vtable == (const NodeVTable *) &node_vtable_bit_field)
        && (node->data != NULL)
        && (node->first_child != NULL))
    {
        FieldData * field_data = node->first_child->data;
        Array *     array_token = field_data->array_token_info;

        ASSERT_TRACE(array_token != NULL, "array_token != NULL");

        if (array_token != NULL)
        {
            int         i;

            i = array_token->get_count(array_token);

            ASSERT_TRACE(i > 0, "There are no items in the token array");

            if (i > 0)
            {
                TokenInfo * token_info = (TokenInfo *) array_token->get_object(array_token, i - 1);

                ASSERT_TRACE(token_info != NULL, "token_info != NULL");

                if (token_info != NULL)
                {
                    size = token_info->fieldSize;
                }

                ASSERT_TRACE(size > 0, "The token info corresponding to the given token doesn't have a valid size");
            }
        }
    }

    return size;
}

int get_bit_field_child_offset(Node * node, Node * child)
{
    int offset = 0;

    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE((node == NULL) || (node->vtable == (const NodeVTable *) &node_vtable_bit_field),
                                   "node->vtable == &node_vtable_bit_field");
    ASSERT_TRACE((node == NULL) || (node == child->parent), "node == child->parent");

    if ((node != NULL)
        && (node->vtable == (const NodeVTable *) &node_vtable_bit_field)
        && (node == child->parent))
    {
        offset = field_node_get_offset(node);
    }

    return offset;
}

const NodeVTableField node_vtable_bit_field =
{
    node_vtable_field_duplicate_data,
    node_vtable_field_destroy_data,
    get_bit_field_size,
    get_bit_field_child_offset
};

//_______________________________________________________________

// Parser definition

Parser * new_parser(void)
{
    return new_buffer(sizeof(Parser));
}

void delete_parser(Parser * parser)
{
    if (parser != NULL)
    {
        delete_buffer((void **) &(parser->string));
        delete_symbolTable(parser->symbolTable);

        destroy_tree(parser->token_tree);

        delete_buffer((void **) &parser);
    }
}

typedef struct
{
    Node *  parent;
    Array * array_ti;
} PFContext;

Node * get_next_token_node(Node * token)
{
    ASSERT_TRACE(token != NULL, "token != NULL");

    if (token != NULL)
    {
        token = navigate_tree_get_next_vtable(token, &node_vtable_token);

        while (IS_VALID_TOKEN_NODE_HIERARCHY(token)
            && ((token_get_type(token) == TOKEN_COMMENT)
             || (token_get_type(token) == TOKEN_COMMENT_OPEN)))
        {
            if (token_get_type(token) == TOKEN_COMMENT)
            {
                Node * line;

                for (line = token->parent->parent;
                    IS_VALID_TOKEN_NODE(token)
                    && (token->parent->parent == line);
                    token = navigate_tree_get_next_vtable(token, &node_vtable_token));
            }

            if (IS_VALID_TOKEN_NODE_HIERARCHY(token) && (token_get_type(token) == TOKEN_COMMENT_OPEN))
            {
                for (token = navigate_tree_get_next_vtable(token, &node_vtable_token);
                    IS_VALID_TOKEN_NODE_HIERARCHY(token)
                    && (token_get_type(token) != TOKEN_COMMENT_CLOSE);
                    token = navigate_tree_get_next_vtable(token, &node_vtable_token));

                if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                    && (token_get_type(token) == TOKEN_COMMENT_CLOSE))
                {
                    token = navigate_tree_get_next_vtable(token, &node_vtable_token);
                }
            }
        }
    }

    return token;
}

void * process_field(Node ** pNode, void * context, Token token_field, const NodeVTableField * node_vtable_field)
{
    Node * field = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");
    ASSERT_TRACE(context != NULL, "context != NULL");
    ASSERT_TRACE(IS_FIELD_TOKEN_TYPE(token_field), "IS_FIELD_TOKEN_TYPE(token_field)");
    ASSERT_TRACE(IS_FIELD_VTABLE(node_vtable_field), "IS_FIELD_VTABLE(node_vtable_field)");

    if ((pNode != NULL)
        && (context != NULL)
        && IS_FIELD_TOKEN_TYPE(token_field)
        && IS_FIELD_VTABLE(node_vtable_field))
    {
        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(*pNode);

        if (IS_VALID_TOKEN_NODE_HIERARCHY(*pNode) && (token_get_type(*pNode) == token_field))
        {
            Node *          node = NULL;
            Node *          token = *pNode;
            Node *          word = token->parent;
            Node *          line = word->parent;
            Node *          child_field = NULL;
            TInstruction *  instruction = NULL;

            for (token = get_next_token_node(token);
                IS_VALID_TOKEN_NODE(token)
                && (token_get_type(token) != TOKEN_LEFT_CURLY_BRACKET);
                token = get_next_token_node(token));

            if (IS_VALID_TOKEN_NODE(token) && (token_get_type(token) == TOKEN_LEFT_CURLY_BRACKET))
            {
                PFContext * pf_context = (PFContext *) context;
                
                field = new_node((const NodeVTable *) node_vtable_field);

                ASSERT_TRACE(field != NULL, "field != NULL");

                if (field != NULL)
                {
                    if (pf_context->parent != NULL)
                    {
                        append_child(pf_context->parent, field);
                    }

                    for (token = get_next_token_node(token);
                        IS_VALID_TOKEN_NODE(token)
                        && (token_get_type(token) != TOKEN_RIGHT_CURLY_BRACKET)
                        && (field != NULL);
                        token = get_next_token_node(token))
                    {
                        word = token->parent;
                        line = word->parent;

                        if (IS_VALID_FIELD_TOKEN_NODE(token) && (token_get_process(token) != NULL))
                        {
                            pf_context->parent = field;

                            child_field = token_get_process(token)(&token, context);

                            ASSERT_TRACE((child_field == NULL) || IS_VALID_FIELD_NODE(child_field), "IS_VALID_FIELD_NODE(child_field)");

                            if (IS_VALID_FIELD_NODE(child_field))
                            {
                                if (((FieldData *)(child_field->data))->bits > 0)
                                {
                                    for (node = field->first_child;
                                         (node != NULL) && (node->next != NULL);
                                         node = node->next);

                                    ASSERT_TRACE((field->first_child == NULL) || IS_VALID_FIELD_NODE(node), "IS_VALID_FIELD_NODE(node)");

                                    if ((field->first_child == NULL) || IS_VALID_FIELD_NODE(node))
                                    {
                                        if (!IS_VALID_BIT_FIELD_NODE(node)
                                            || ((((FieldData *)(node->data))->field_size * BITS_PER_BYTE) < (((FieldData *)node->data)->bit_field_size + ((FieldData *)child_field->data)->bits))
                                            || (field_node_get_size(child_field) != field_node_get_size(node)))
                                        {
                                            node = new_node((const NodeVTable *)&node_vtable_bit_field);

                                            if (IS_VALID_BIT_FIELD_NODE(node))
                                            {
                                                ((FieldData *)(node->data))->field_size = field_node_get_size(child_field);

                                                append_child(field, node);
                                            }
                                        }

                                        ASSERT_TRACE_BIT_FIELD_NODE(node);

                                        if (IS_VALID_BIT_FIELD_NODE(node))
                                        {
                                            unsigned int i;
                                            //((FieldData *)child_field->data)->bit_field_member_mask = (unsigned int)(pow(2, ((FieldData *)child_field->data)->bits) - 1) << ((FieldData*) node)->bit_field_size;
                                            for (i = 0; i < ((FieldData *)child_field->data)->bits; i++)
                                            {
                                                ((FieldData *)child_field->data)->bit_field_member_mask |= 1 << (((FieldData *)node->data)->bit_field_size + i);
                                            }
                                            ((FieldData *)node->data)->bit_field_size += ((FieldData *)child_field->data)->bits;
                                            append_child(node, child_field);
                                        }
                                    }
                                }
                                else if (child_field->parent == NULL)
                                {
                                    append_child(field, child_field);
                                }

                                *pNode = token;
                            }
                            else
                            {
                                destroy_tree(field);
                                field = NULL;
                            }
                        }

                        if ((token_get_type(token) == TOKEN_TEMPLATE_INSTRUCTION)
                            && (token_get_process(token) != NULL))
                        {
                            if (token->parent->prev == NULL)
                            {
                                child_field = NULL;
                            }

                            instruction = (TInstruction *) token_get_process(token)(&token, child_field);

                            if (instruction != NULL)
                            {
                                pf_context->array_ti->append_object(pf_context->array_ti, instruction);
                                *pNode = token;
                            }
                            else
                            {
                                destroy_tree(field);
                                field = NULL;
                            }
                        }
                    }

                    if (field != NULL)
                    {
                        if (IS_VALID_TOKEN_NODE(token) && (token_get_type(token) == TOKEN_RIGHT_CURLY_BRACKET))
                        {
                            for (token = get_next_token_node(token);
                                IS_VALID_TOKEN_NODE(token) && (token_get_type(token) != TOKEN_SEMICOLON);
                                token = get_next_token_node(token));

                            if (IS_VALID_TOKEN_NODE(token) && (token_get_type(token) == TOKEN_SEMICOLON))
                            {
                                *pNode = token;
                            }
                            else
                            {
                                destroy_tree(field);
                                field = NULL;

                                error(line, "ERROR: Missing ';', no valid declaration ");
                            }
                        }
                        else
                        {
                            destroy_tree(field);
                            field = NULL;

                            error(line, "ERROR: Missing '}', no valid declaration ");
                        }
                    }
                }
            }
            else
            {
                error(line, "ERROR: There's no valid definition");
            }
        }
    }

    return field;
}

void * process_struct_field(Node ** pNode, void * context)
{
    return process_field(pNode, context, TOKEN_struct, &node_vtable_struct_field);
}

void * process_union_field(Node ** pNode, void * context)
{
    return process_field(pNode, context, TOKEN_union, &node_vtable_union_field);
}

void * process_member_field(Node ** pNode, void * context)
{
    Node * member = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        Node * token = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);

        if (IS_VALID_TOKEN_NODE_HIERARCHY(token))
        {
            Node *      word = token->parent;
            Node *      line = word->parent;
            TokenInfo * token_info;

            member = new_node((const NodeVTable *) &node_vtable_member_field);

            ASSERT_TRACE_MEMBER_FIELD_NODE(member);

            if (IS_VALID_MEMBER_FIELD_NODE(member))
            {
                FieldData * field_data = (FieldData *) member->data;

                for (;IS_VALID_FIELD_TOKEN_NODE(token)
                    && (token->prev == NULL)
                    && (token->next == NULL);
                    token = get_next_token_node(token))
                {
                    word = token->parent;
                    line = word->parent;

                    ASSERT_TRACE(token->data != NULL, "token->data != NULL");

                    token_info = duplicate_tokenInfo((TokenInfo *)(token->data));

                    ASSERT_TRACE(token_info != NULL, "token_info != NULL");

                    field_data->array_token_info->append_object(field_data->array_token_info, token_info);
                }

                if ((0 < field_data->array_token_info->get_count(field_data->array_token_info))
                    && IS_VALID_TOKEN_NODE_HIERARCHY(token))
                {
                    char *  id_name;
                    int     id_name_len;

                    word = token->parent;
                    line = word->parent;
                    id_name = word_get_string(word);

                    for (id_name_len = 0;
                        (id_name != NULL)
                        && (id_name_len < (int)strlen(id_name))
                        && (id_name[id_name_len] != ';')
                        && (id_name[id_name_len] != ':')
                        && (id_name[id_name_len] != '[')
                        && (id_name[id_name_len] != '}')
                        && (id_name[id_name_len] != '\n');
                        id_name_len ++);
                        
                        if ((id_name != NULL)
                            && (0 < id_name_len)
                            && ((id_name[id_name_len] == ';')
                             || (id_name[id_name_len] == ':')
                             || (id_name[id_name_len] == '[')
                             || (id_name_len == (int)strlen(id_name))))
                        {
                            char * str_doc = document_get_string(line->parent);

                            word = token->parent;
                            line = word->parent;

                            char_string_copy(&(field_data->id_name), id_name, id_name_len);

                            ASSERT_TRACE(field_data->id_name != NULL, "field_data->id_name != NULL");

                            for (token = get_next_token_node(token);
                                IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                && (token_get_type(token) != TOKEN_SEMICOLON)
                                && (token_get_type(token) != TOKEN_COLON)
                                && (token_get_type(token) != TOKEN_LEFT_SQUARE_BRACKET)
                                && (token_get_type(token) != TOKEN_RIGHT_CURLY_BRACKET)
                                && (token_get_type(token) != TOKEN_TEMPLATE_INSTRUCTION)
                                && (token_get_type(token) != TOKEN_END_TEMPLATE);
                                token = get_next_token_node(token));

                            if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                && ((token_get_type(token) == TOKEN_SEMICOLON)
                                 || (token_get_type(token) == TOKEN_COLON)
                                 || (token_get_type(token) == TOKEN_LEFT_SQUARE_BRACKET))
                                && (str_doc != NULL)
                                && ((token_get_position(token) + strlen(token_get_string(token))) < strlen(str_doc)))
                            {
                                Token           token_type = token_get_type(token);
                                long int        value;
                                unsigned int    pos;

                                switch (token_type)
                                {
                                case TOKEN_COLON:
                                case TOKEN_LEFT_SQUARE_BRACKET:

                                    token = get_next_token_node(token);

                                    if (IS_VALID_TOKEN_NODE_HIERARCHY(token))
                                    {
                                        pos = token_get_position(token);

                                        for (token = get_next_token_node(token);
                                            IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                            && (token_get_type(token) != TOKEN_RIGHT_SQUARE_BRACKET)
                                            && (token_get_type(token) != TOKEN_SEMICOLON)
                                            && (token_get_type(token) != TOKEN_TEMPLATE_INSTRUCTION)
                                            && (token_get_type(token) != TOKEN_RIGHT_CURLY_BRACKET)
                                            && (token_get_type(token) != TOKEN_END_TEMPLATE);
                                            token = get_next_token_node(token));

                                        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                            && (((token_type == TOKEN_LEFT_SQUARE_BRACKET) && (token_get_type(token) == TOKEN_RIGHT_SQUARE_BRACKET))
                                             || ((token_type == TOKEN_COLON)               && ((token_get_type(token) == TOKEN_SEMICOLON))))
                                            && (pos < token_get_position(token)))
                                        {
                                            if ((SUCCESS == evaluate_expression(&str_doc[pos], token_get_position(token) - pos, &value))
                                                && (value > 0))
                                            {
                                                word = token->parent;
                                                line = word->parent;

                                                switch (token_type)
                                                {
                                                case TOKEN_LEFT_SQUARE_BRACKET:
                                                    field_data->num_elements = value;

                                                    token = get_next_token_node(token);

                                                    if (!(IS_VALID_TOKEN_NODE(token)
                                                        && (token_get_type(token) == TOKEN_SEMICOLON)))
                                                    {
                                                        destroy_tree(member);
                                                        member = NULL;
                                                        error(line, "ERROR: Ther's no valid member definition");
                                                    }
                                                    break;

                                                case TOKEN_COLON:
                                                    field_data->bits = value;
                                                    break;

                                                default : ASSERT_TRACE(0, "There's a mechanism error!");
                                                }
                                            }
                                            else
                                            {
                                                destroy_tree(member);
                                                member = NULL;
                                                error(line, "ERROR: The expression cannot be evaluated");
                                            }
                                        }
                                        else
                                        {
                                            destroy_tree(member);
                                            member = NULL;
                                            error(line, "ERROR: There's no valid member definition");
                                        }
                                    }
                                    else
                                    {
                                        destroy_tree(member);
                                        member = NULL;
                                        error(line, "ERROR: There's no valid member definition");
                                    }
                                    break;

                                case TOKEN_SEMICOLON:
                                    break;

                                default : ASSERT_TRACE(0, "There's a mechanism error!");
                                }

                                if (member != NULL)
                                {
                                    *pNode = token;
                                }
                            }
                            else
                            {
                                destroy_tree(member);
                                member = NULL;
                                error(line, "Error: There's no valid member definition");
                            }
                        }
                        else
                        {
                            destroy_tree(member);
                            member = NULL;
                            error(line, "ERROR: Missing member ID Name");
                        }
                }
                else
                {
                    destroy_tree(member);
                    member = NULL;
                    error(line, "ERROR: Missing valid member type definition");
                }
            }
        }
    }

    return member;
}

void * process_MAP_TEMPLATE(Node ** pNode, void * context)
{
    Map * map = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        Node * token = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);

        if (IS_VALID_TOKEN_NODE_HIERARCHY(token))
        {
            Node * prev_token = navigate_tree_get_prev_vtable(token, &node_vtable_token);

            if (IS_VALID_TOKEN_NODE_HIERARCHY(prev_token)
                && (token_get_type(prev_token) == TOKEN_TEMPLATE_INSTRUCTION)
                && (token->parent->parent == prev_token->parent->parent)
                && (token->next == NULL))
            {
                Node * word = navigate_tree_get_next_vtable(token, &node_vtable_word);

                if (IS_VALID_WORD_NODE(word)
                    && (word->parent == token->parent->parent)
                    && (word->next == NULL))
                {
                    Node * line = word->parent;
                    map = new_map();

                    ASSERT_TRACE(map != NULL, "map != NULL");

                    if (map != NULL)
                    {
                        map->id_name = duplicate_char_buffer((char *) word->data);

                        ASSERT_TRACE(map->id_name != NULL, "map->id_name != NULL");

                        for (token = get_next_token_node(token);
                            IS_VALID_TOKEN_NODE_HIERARCHY(token)
                            && (token_get_type(token) != TOKEN_enum)
                            && (token_get_type(token) != TOKEN_NUMSIGN)
                            && (token_get_type(token) != TOKEN_END_MAP);
                            token = get_next_token_node(token));

                        if (IS_VALID_TOKEN_NODE_HIERARCHY(token))
                        {
                            Node *              next_token;
                            MapElement *        element = NULL;
                            char *              str_doc = document_get_string(token->parent->parent->parent);
                            unsigned long int   format_string_pos = 0;

                            switch (token_get_type(token))
                            {
                            case TOKEN_enum:

                                word = token->parent;

                                for (token = get_next_token_node(token);
                                    IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                    && (token_get_type(token) != TOKEN_LEFT_CURLY_BRACKET)
                                    && (token_get_type(token) != TOKEN_RIGHT_CURLY_BRACKET)
                                    && (token_get_type(token) != TOKEN_COMMA);
                                    token = get_next_token_node(token));

                                    if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                        && (token_get_type(token) == TOKEN_LEFT_CURLY_BRACKET))
                                    {
                                        long int value = 0;

                                        for (token = get_next_token_node(token);
                                            IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                            && (map != NULL)
                                            && (token_get_type(token) != TOKEN_RIGHT_CURLY_BRACKET)
                                            && (token_get_type(token) != TOKEN_END_MAP);
                                            token = get_next_token_node(token))
                                        {
                                            word = token->parent;
                                            format_string_pos = token_get_position(token);

                                            ASSERT_TRACE(format_string_pos < strlen(str_doc),
                                                        "format_string_pos < strlen(str_doc)");

                                            for (token = get_next_token_node(token);
                                                IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                && (token_get_type(token) != TOKEN_COMMA)
                                                && (token_get_type(token) != TOKEN_EQUALS_SIGN)
                                                && (token_get_type(token) != TOKEN_RIGHT_CURLY_BRACKET);
                                                token = get_next_token_node(token));

                                            ASSERT_TRACE(format_string_pos < token_get_position(token),
                                                        "format_string_pos < token_get_position(token)");
                                            ASSERT_TRACE(token_get_position(token) < strlen(str_doc),
                                                        "token_get_position(token) < strlen(str_doc)");

                                            if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                && (format_string_pos < token_get_position(token))
                                                && (token_get_position(token) < strlen(str_doc))
                                                && ((token_get_type(token) == TOKEN_COMMA)
                                                 || (token_get_type(token) == TOKEN_EQUALS_SIGN)
                                                 || (token_get_type(token) == TOKEN_RIGHT_CURLY_BRACKET)))
                                            {
                                                element = new_mapElement();

                                                ASSERT_TRACE(element != NULL, "element != NULL");

                                                if (element != NULL)
                                                {
                                                    if (word == token->parent)
                                                    {
                                                        char_string_copy(&(element->name),
                                                            &str_doc[format_string_pos],
                                                            token_get_position(token) - format_string_pos);
                                                        ASSERT_TRACE(element->name != NULL, "element->name != NULL");
                                                        *pNode = token;
                                                    }
                                                    else
                                                    {
                                                        element->name = duplicate_char_buffer(word_get_string(word));
                                                        ASSERT_TRACE(element->name != NULL, "element->name != NULL");
                                                        *pNode = token;
                                                    }
                                                    
                                                    if (token_get_type(token) == TOKEN_RIGHT_CURLY_BRACKET)
                                                    {
                                                        token = navigate_tree_get_prev_vtable(token, &node_vtable_token);
                                                    }

                                                    if ((map != NULL)
                                                        && IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                        && (token_get_type(token) == TOKEN_EQUALS_SIGN))
                                                    {
                                                        word = token->parent;
                                                        format_string_pos = token_get_position(token) + 1;

                                                        for (token = get_next_token_node(token);
                                                            IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                            && (token_get_type(token) != TOKEN_COMMA);
                                                            token = get_next_token_node(token));

                                                        ASSERT_TRACE(format_string_pos < token_get_position(token),
                                                                    "format_string_pos < token_get_position(token)");
                                                        ASSERT_TRACE(token_get_position(token) < strlen(str_doc),
                                                                    "token_get_position(token) < strlen(str_doc)");

                                                        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                            && (token_get_type(token) == TOKEN_COMMA)
                                                            && (format_string_pos < token_get_position(token))
                                                            && (token_get_position(token) < strlen(str_doc)))
                                                        {
                                                            if (evaluate_expression(&str_doc[format_string_pos],
                                                                                    token_get_position(token) - format_string_pos,
                                                                                    &value) != SUCCESS)
                                                            {
                                                                delete_map(map);
                                                                map = NULL;
                                                                error(word->parent, "ERROR: Value cannot be evaluated");
                                                            }
                                                        }
                                                        else
                                                        {
                                                            delete_map(map);
                                                            map = NULL;
                                                            error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                                        }
                                                    }

                                                    if (IS_VALID_TOKEN_NODE_HIERARCHY(token) && (map != NULL))
                                                    {
                                                        next_token = get_next_token_node(token);

                                                        if (IS_VALID_TOKEN_NODE(next_token)
                                                            && (token_get_type(next_token) == TOKEN_TEMPLATE_INSTRUCTION))
                                                        {
                                                            for (token = get_next_token_node(token);
                                                                IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                && (token_get_type(token) != TOKEN_QUOT);
                                                                token = get_next_token_node(token));

                                                            if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                && (token_get_type(token) == TOKEN_QUOT))
                                                            {
                                                                format_string_pos = token_get_position(token) + 1;

                                                                for (token = get_next_token_node(token);
                                                                    IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                    && (token_get_type(token) != TOKEN_QUOT);
                                                                    token = get_next_token_node(token));

                                                                    ASSERT_TRACE(format_string_pos < token_get_position(token),
                                                                                "format_string_pos < token_get_position(token)");
                                                                    ASSERT_TRACE(token_get_position(token) < strlen(str_doc),
                                                                                "token_get_position(token) < strlen(str_doc)");

                                                                if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                    && (token_get_type(token) == TOKEN_QUOT)
                                                                    && (format_string_pos < token_get_position(token))
                                                                    && (token_get_position(token) < strlen(str_doc)))
                                                                {
                                                                    char_string_copy(&(element->format_string),
                                                                        &str_doc[format_string_pos],
                                                                        token_get_position(token) - format_string_pos);
                                                                    *pNode = token;
                                                                }
                                                                else
                                                                {
                                                                    delete_map(map);
                                                                    map = NULL;
                                                                    error(word->parent, "ERROR: There's no valid format string definition");
                                                                }
                                                            }
                                                            else
                                                            {
                                                                delete_map(map);
                                                                map = NULL;
                                                                error(word->parent, "ERROR: There's no valid format string definition");
                                                            }
                                                        }
                                                    }

                                                    if (map != NULL)
                                                    {
                                                        element->value = value;
                                                        map->array_map_elements->append_object(map->array_map_elements, element);
                                                        value ++;
                                                    }
                                                    else
                                                    {
                                                        delete_mapElement(element);
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                delete_map(map);
                                                map = NULL;
                                                error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                            }
                                        }

                                        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                            && (token_get_type(token) == TOKEN_RIGHT_CURLY_BRACKET))
                                        {
                                            word = token->parent;

                                            for (token = get_next_token_node(token);
                                                IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                && (token_get_type(token) != TOKEN_SEMICOLON);
                                                token = get_next_token_node(token));

                                            if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                && (token_get_type(token) == TOKEN_SEMICOLON))
                                            {
                                                token = get_next_token_node(token);

                                                if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                    && (token_get_type(token) == TOKEN_TEMPLATE_INSTRUCTION))
                                                {
                                                    token = get_next_token_node(token);

                                                    if (IS_VALID_TOKEN_NODE_HIERARCHY(token))
                                                    {
                                                        if (token_get_type(token) == TOKEN_DEFAULT)
                                                        {
                                                            token = get_next_token_node(token);

                                                            if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                && (token_get_type(token) == TOKEN_QUOT))
                                                            {
                                                                format_string_pos = token_get_position(token) + 1;
                                                                word = token->parent;

                                                                ASSERT_TRACE(format_string_pos < strlen(str_doc),
                                                                            "format_string_pos < strlen(str_doc)");

                                                                if (format_string_pos < strlen(str_doc))
                                                                {
                                                                    for (token = get_next_token_node(token);
                                                                        IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                        && (word->parent == token->parent->parent)
                                                                        && (token_get_type(token) != TOKEN_QUOT);
                                                                        token = get_next_token_node(token));

                                                                    ASSERT_TRACE(format_string_pos < token_get_position(token),
                                                                                "format_string_pos < token_get_position(token)");

                                                                    ASSERT_TRACE(token_get_position(token) < strlen(str_doc),
                                                                                "token_get_position(token) < strlen(str_doc)");

                                                                    if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                        && (word->parent == token->parent->parent)
                                                                        && (token_get_type(token) == TOKEN_QUOT)
                                                                        && (format_string_pos < token_get_position(token))
                                                                        && (token_get_position(token) < strlen(str_doc)))
                                                                    {
                                                                        char_string_copy(&(map->default_string),
                                                                                         &str_doc[format_string_pos],
                                                                                         token_get_position(token) - format_string_pos);

                                                                        *pNode = token;
                                                                    }
                                                                    else
                                                                    {
                                                                        delete_map(map);
                                                                        map = NULL;
                                                                        error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                delete_map(map);
                                                                map = NULL;
                                                                error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                                            }
                                                        }
                                                        else if (token_get_type(token) != TOKEN_END_MAP)
                                                        {
                                                            delete_map(map);
                                                            map = NULL;
                                                            error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                                        }
                                                    }
                                                    else
                                                    {
                                                        delete_map(map);
                                                        map = NULL;
                                                        error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                                    }
                                                }
                                                else
                                                {
                                                    delete_map(map);
                                                    map = NULL;
                                                    error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                                }
                                            }
                                            else
                                            {
                                                delete_map(map);
                                                map = NULL;
                                                error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                            }
                                        }
                                        else if (map != NULL)
                                        {
                                            delete_map(map);
                                            map = NULL;
                                            error(word->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                        }
                                    }
                                    else
                                    {
                                        delete_map(map);
                                        map = NULL;
                                        error(word->parent, "ERROR: There's no valid \"enum\" MAP_TEMPLATE definition");
                                    }
                                break;

                            case TOKEN_NUMSIGN:
                                for (; IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                    && (token_get_type(token) != TOKEN_END_MAP)
                                    && (map != NULL);
                                    token = get_next_token_node(token))
                                {
                                    word = token->parent;
                                    line = word->parent;
                                    element = NULL;

                                    for (next_token = token;
                                        IS_VALID_TOKEN_NODE_HIERARCHY(next_token)
                                        && (token_get_type(next_token) != TOKEN_define)
                                        && (word->parent == next_token->parent->parent);
                                        next_token = get_next_token_node(next_token));

                                    word = NULL;
                                    if (IS_VALID_TOKEN_NODE_HIERARCHY(next_token)
                                        && (token_get_type(next_token) == TOKEN_define))
                                    {
                                        word = navigate_tree_get_next_vtable(next_token, &node_vtable_word);
                                    }

                                    if ((token_get_type(token) == TOKEN_NUMSIGN)
                                        && IS_VALID_WORD_NODE(word))
                                    {
                                        element = new_mapElement();

                                        ASSERT_TRACE(element != NULL, "element != NULL");

                                        if (element != NULL)
                                        {
                                            element->name = duplicate_char_buffer(word_get_string(word));

                                            ASSERT_TRACE(element->name != NULL, "element->name != NULL");

                                            word = navigate_tree_get_next_vtable(word, &node_vtable_word);

                                            if (IS_VALID_WORD_NODE(word) && IS_VALID_TOKEN_NODE(word->first_child))
                                            {
                                                format_string_pos = token_get_position(word->first_child);

                                                for (token = word->first_child;
                                                    IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                    && (token_get_type(token) != TOKEN_COMMENT)
                                                    && (token_get_type(token) != TOKEN_COMMENT_OPEN)
                                                    && (token_get_type(token) != TOKEN_TEMPLATE_INSTRUCTION)
                                                    && (word->parent == token->parent->parent);
                                                    token = navigate_tree_get_next_vtable(token, &node_vtable_token));

                                                ASSERT_TRACE(format_string_pos < token_get_position(token),
                                                            "format_string_pos < token_get_position(token)");
                                                ASSERT_TRACE(token_get_position(token) < strlen(str_doc),
                                                            "token_get_position(token) < strlen(str_doc)");

                                                if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                    && (format_string_pos < token_get_position(token))
                                                    && (token_get_position(token) < strlen(str_doc))
                                                    && (evaluate_expression(&str_doc[format_string_pos], token_get_position(token) - format_string_pos, &(element->value)) == SUCCESS))
                                                {
                                                    for (next_token = token;
                                                        IS_VALID_TOKEN_NODE_HIERARCHY(next_token)
                                                        && (token_get_type(next_token) != TOKEN_TEMPLATE_INSTRUCTION)
                                                        && (next_token->parent->parent == word->parent);
                                                        next_token = get_next_token_node(next_token));

                                                    if (IS_VALID_TOKEN_NODE_HIERARCHY(next_token)
                                                        && (token_get_type(next_token) == TOKEN_TEMPLATE_INSTRUCTION)
                                                        && (next_token->parent->parent == word->parent))
                                                    {
                                                        token = next_token;
                                                    }

                                                    if (IS_VALID_TOKEN_NODE(token)
                                                        && (token_get_type(token) == TOKEN_TEMPLATE_INSTRUCTION)
                                                        && (token->parent->parent ==  word->parent))
                                                    {
                                                        token = get_next_token_node(token);

                                                        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                            && (token_get_type(token) == TOKEN_QUOT))
                                                        {
                                                            format_string_pos = token_get_position(token) + 1;

                                                            ASSERT_TRACE((format_string_pos + 1) < (int) strlen(str_doc), "(format_string_pos + 1) < strlen(str_doc)");

                                                            if (format_string_pos < (int) strlen(str_doc))
                                                            {
                                                                word = token->parent;

                                                                for (token = get_next_token_node(token);
                                                                    IS_VALID_TOKEN_NODE(token)
                                                                    && (token_get_type(token) != TOKEN_QUOT)
                                                                    && (word->parent == token->parent->parent);
                                                                    token = get_next_token_node(token));

                                                                if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                    && (token_get_type(token) == TOKEN_QUOT)
                                                                    && (word->parent == token->parent->parent))
                                                                {
                                                                    ASSERT_TRACE(format_string_pos < (int) token_get_position(token),
                                                                                "format_string_pos < token_get_position(token)");
                                                                    ASSERT_TRACE(token_get_position(token) < strlen(str_doc),
                                                                                "token_get_position(token) < strlen(str_doc)");

                                                                    char_string_copy(&(element->format_string),
                                                                                        &str_doc[format_string_pos],
                                                                                        token_get_position(token) - format_string_pos);
                                                                    
                                                                    map->array_map_elements->append_object(map->array_map_elements, element);
                                                                    *pNode = token;
                                                                }
                                                                else
                                                                {
                                                                    delete_map(map);
                                                                    map = NULL;
                                                                    delete_mapElement(element);
                                                                    element = NULL;
                                                                    error(word->parent, "ERROR: NO valid format string");
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            delete_map(map);
                                                            map = NULL;
                                                            delete_mapElement(element);
                                                            element = NULL;
                                                            error(word->parent, "ERROR: No valid format string");
                                                        }
                                                    }
                                                    else
                                                    {
                                                        map->array_map_elements->append_object(map->array_map_elements, element);

                                                        for (next_token = line->first_child->first_child;
                                                            IS_VALID_TOKEN_NODE_HIERARCHY(next_token)
                                                            && (next_token->parent->parent == line);
                                                            token = next_token, next_token = get_next_token_node(next_token));

                                                            *pNode = token;
                                                    }
                                                }
                                                else
                                                {
                                                    delete_map(map);
                                                    map = NULL;
                                                    delete_mapElement(element);
                                                    element = NULL;
                                                    error(word->parent, "ERROR: MAP value cannot be evaluated");
                                                }
                                            }
                                            else
                                            {
                                                delete_map(map);
                                                map = NULL;
                                                delete_mapElement(element);
                                                element = NULL;
                                                error(token->parent->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
                                            }
                                        }
                                    }
                                    else if (token_get_type(token) == TOKEN_TEMPLATE_INSTRUCTION)
                                    {
                                        word = token->parent;
                                        next_token = get_next_token_node(token);

                                        if (IS_VALID_TOKEN_NODE(next_token))
                                        {
                                            if (token_get_type(next_token) == TOKEN_DEFAULT)
                                            {
                                                token = get_next_token_node(next_token);

                                                if (IS_VALID_TOKEN_NODE_HIERARCHY(token) && (token_get_type(token) == TOKEN_QUOT))
                                                {
                                                    word = token->parent;
                                                    format_string_pos = token_get_position(token) + 1;

                                                    ASSERT_TRACE(format_string_pos < (int) strlen(str_doc), "format_string_pos < str_doc");

                                                    if (format_string_pos < (int) strlen(str_doc))
                                                    {
                                                        for (token = get_next_token_node(token);
                                                            IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                            && (token_get_type(token) != TOKEN_QUOT);
                                                            token = get_next_token_node(token));

                                                        ASSERT_TRACE(format_string_pos < token_get_position(token),
                                                                    "format_string_pos < token_get_position(token)");
                                                        ASSERT_TRACE(token_get_position(token) < strlen(str_doc),
                                                                    "token_get_position(token) < strlen(str_doc)");

                                                        if ((format_string_pos < token_get_position(token))
                                                            && (token_get_position(token) < strlen(str_doc)))
                                                        {
                                                            if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                                                                && (token_get_type(token) == TOKEN_QUOT))
                                                            {
                                                                char_string_copy(&(map->default_string),
                                                                                &str_doc[format_string_pos],
                                                                                token_get_position(token) - format_string_pos);
                                                                *pNode = token;
                                                            }
                                                            else
                                                            {
                                                                delete_map(map);
                                                                map = NULL;
                                                                delete_mapElement(element);
                                                                element = NULL;
                                                                error(word->parent, "ERROR: There's no valid default format string");
                                                            }
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    delete_map(map);
                                                    map = NULL;
                                                    delete_mapElement(element);
                                                    element = NULL;
                                                    error(word->parent, "ERROR: There's no valid Token in MAP_TEMPLATE declaration");
                                                }
                                            }
                                            else if (token_get_type(next_token) != TOKEN_END_MAP)
                                            {
                                                delete_map(map);
                                                map = NULL;
                                                delete_mapElement(element);
                                                element = NULL;
                                                error(word->parent, "ERROR: There's no valid Token in MAP_TEMPLATE declaration");
                                            }
                                        }
                                        else
                                        {
                                            delete_map(map);
                                            map = NULL;
                                            delete_mapElement(element);
                                            element = NULL;
                                            error(word->parent, "ERROR: No valid MAP_TEMPLATE declaration");
                                        }
                                    }
                                    else
                                    {
                                        delete_map(map);
                                        map = NULL;
                                        delete_mapElement(element);
                                        element = NULL;
                                        error(token->parent->parent, "ERROR: There's no valid MAP_TEPLATE declaration");
                                    }
                                }
                                break;
                            default:
                                delete_map(map);
                                error(token->parent->parent, "ERROR: There's no valid MAP_TEMPLATE declaration");
                            }
                        }
                        else
                        {
                            delete_map(map);
                            error(word->parent, "ERROR: No complete MAP_TEMPLATE definition");
                        }
                    }
                }
            }
            else
            {
                error(token->parent->parent, "ERROR: There's no valid MAP_TEMPLATE definition");
            }
        }
    }

    return map;
}

void * process_QUOT(Node ** pNode, void * context)
{
    TInstruction * instruction = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        Node * token = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);
        ASSERT_TRACE(!IS_VALID_TOKEN_NODE(token) || (token_get_type(token) == TOKEN_QUOT),
                                                    "token_get_type(token) == TOKEN_QUOT");
        ASSERT_TRACE(context == NULL || IS_VALID_FIELD_NODE((Node *)context),
                    "context == NULL || IS_VALID_FIELD_NODE((Node *)context)");

        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
            && (token_get_type(token) == TOKEN_QUOT)
            && (context == NULL || IS_VALID_FIELD_NODE((Node *)context)))
        {
            instruction = new_tinstruction();

            ASSERT_TRACE(instruction != NULL, "instruction != NULL");

            if (instruction != NULL)
            {
                Node *          member_field = (Node *) context;
                Node *          next_token = NULL;
                Node *          word = token->parent;
                Node *          line = word->parent;
                Array *         array_fs = instruction->array_format_string;
                TokenInfo *     token_info = NULL;
                char *          str_doc = document_get_string(line->parent);
                char *          format_string = NULL;
                unsigned int    pos = 0;

                if (IS_VALID_FIELD_NODE(member_field))
                {
                    NodePath * node_path = obtain_node_path(member_field);

                    ASSERT_TRACE(node_path != NULL, "node_path != NULL");

                    instruction->data_path = node_path;

                    instruction->iterations = ((FieldData *)(member_field->data))->num_elements;
                }

                for (next_token = token;
                    IS_VALID_TOKEN_NODE_HIERARCHY(next_token)
                    && (next_token->parent->parent == line)
                    && (instruction != NULL);
                    next_token = get_next_token_node(next_token))
                {
                    token = next_token;

                    switch (token_get_type(token))
                    {
                    case TOKEN_QUOT:
                        pos = token_get_position(token);

                        for (token = get_next_token_node(token);
                            IS_VALID_TOKEN_NODE_HIERARCHY(token)
                            && (token_get_type(token) != TOKEN_QUOT)
                            && (token->parent->parent == line);
                            token = get_next_token_node(token));

                        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                            && (token->parent->parent == line)
                            && (token_get_type(token) == TOKEN_QUOT)
                            && ((pos + 1) < token_get_position(token)))
                        {
                            char_string_copy(&format_string, &str_doc[pos + 1], token_get_position(token) - (pos + 1));

                            array_fs->append_object(array_fs, format_string);
                            format_string = NULL;
                        }
                        else
                        {
                            delete_tinstruction(instruction);
                            instruction = NULL;
                            error(line, "ERROR: There's no valid Template Instruction definition");
                        }
                        break;

                    case TOKEN_ERROR:
                        if (0 < array_fs->get_count(array_fs))
                        {
                            token_info = duplicate_tokenInfo((TokenInfo *)token->data);
                            ASSERT_TRACE(token_info != NULL, "token_info != NULL");
                            instruction->array_token_info->append_object(instruction->array_token_info, token_info);
                        }
                        else
                        {
                            delete_tinstruction(instruction);
                            instruction = NULL;
                            error(line, "ERROR: There's no valid Template Instruction definition");
                        }
                        break;

                    default:
                        delete_tinstruction(instruction);
                        instruction = NULL;
                        error(line, "ERROR: Unknown token in Template Instruction definition");
                    }

                    next_token = token;
                }

                if (instruction != NULL)
                {
                    if (IS_VALID_TOKEN_NODE_HIERARCHY(token))
                    {
                        *pNode = token;
                    }
                    else
                    {
                        delete_tinstruction(instruction);
                        instruction = NULL;
                        error(line, "ERROR: There's no valid Template Instruction definition");
                    }
                }
            }
        }
    }

    return instruction;
}

void * process_MAP(Node ** pNode, void * context)
{
    TInstruction * instruction = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        Node * member_field = (Node *) context;
        Node * token = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);
        ASSERT_TRACE((member_field == NULL) || IS_VALID_MEMBER_FIELD_NODE(member_field),
                    "(member_field == NULL) || IS_VALID_MEMBER_FIELD_NODE(member_field)");

        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
            && (token_get_type(token) == TOKEN_MAP))
        {
            if (IS_VALID_MEMBER_FIELD_NODE(member_field)
                && ((((FieldData *)(member_field->data)))->num_elements == 0))
            {
                if (IS_VALID_WORD_NODE(token->parent->next))
                {
                    char * map_name = word_get_string(token->parent->next);

                    instruction = new_tinstruction();

                    ASSERT_TRACE(instruction != NULL, "instruction != NULL");

                    if (instruction != NULL)
                    {
                        instruction->map_name = duplicate_char_buffer(map_name);

                        ASSERT_TRACE(instruction->map_name != NULL, "instruction->map_name != NULL");

                        instruction->data_path = obtain_node_path(member_field);

                        ASSERT_TRACE(instruction->data_path != NULL, "instruction->data_path != NULL");
                    }

                    for (token = token->parent->next->first_child;
                        (token != NULL) && (token->next != NULL);
                        token = token->next);

                    *pNode = token;
                }
                else
                {
                    error(token->parent->parent, "ERROR: There's no valid Template Instruction definition");
                }
            }
            else
            {
                error(token->parent->parent, "ERROR: There's no valid member to associate MAP");
            }
        }
    }

    return instruction;
}

void * process_STRING(Node ** pNode, void * context)
{
    TInstruction * instruction = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        Node * member_field = (Node *) context;
        Node * token = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);
        ASSERT_TRACE((member_field == NULL) || IS_VALID_MEMBER_FIELD_NODE(member_field),
                    "(member_field == NULL) || IS_VALID_MEMBER_FIELD_NODE(member_field)");

        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
            && (token_get_type(token) == TOKEN_STRING))
        {
            if (IS_VALID_MEMBER_FIELD_NODE(member_field))
            {
                Node * next_token = get_next_token_node(token);

                if (IS_VALID_TOKEN_NODE_HIERARCHY(next_token) 
                    && (token->parent->parent != next_token->parent->parent))
                {
                    instruction = new_tinstruction();

                    ASSERT_TRACE(instruction != NULL, "instruction != NULL");

                    if (instruction != NULL)
                    {
                        TokenInfo * token_info = duplicate_tokenInfo((TokenInfo *) token->data);

                        ASSERT_TRACE(token_info != NULL, "token_info != NULL");

                        instruction->array_token_info->append_object(instruction->array_token_info, token_info);

                        instruction->data_path = obtain_node_path(member_field);

                        ASSERT_TRACE(instruction->data_path != NULL, "instruction->data_path != NULL");

                        instruction->iterations = ((FieldData *)(member_field->data))->num_elements;

                        *pNode = token;
                    }
                }
                else
                {
                    error(token->parent->parent, "ERROR: There's no valid Template Instruction definition");
                }
            }
            else
            {
                error(token->parent->parent, "ERROR: Missing member to associate to STRING");
            }
        }
    }

    return instruction;
}

void * process_HEX_DUMP(Node ** pNode, void * context)
{
    TInstruction * instruction = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        Node * member_field = (Node *) context;
        Node * token = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);
        ASSERT_TRACE((member_field == NULL) || IS_VALID_MEMBER_FIELD_NODE(member_field),
                    "(member_field == NULL) || IS_VALID_MEMBER_FIELD_NODE(member_field)");

        if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
            && (token_get_type(token) == TOKEN_HEX_DUMP))
        {
            if (IS_VALID_MEMBER_FIELD_NODE(member_field))
            {
                if (token->parent->next == NULL)
                {
                    instruction = new_tinstruction();

                    ASSERT_TRACE(instruction != NULL, "instruction != NULL");

                    if (instruction != NULL)
                    {
                        TokenInfo * token_info = duplicate_tokenInfo((TokenInfo *) token->data);

                        ASSERT_TRACE(token_info != NULL, "token_info != NULL");

                        instruction->array_token_info->append_object(instruction->array_token_info, token_info);

                        instruction->data_path = obtain_node_path(member_field);

                        ASSERT_TRACE(instruction->data_path != NULL, "instruction->data_path != NULL");

                        instruction->iterations = ((FieldData *)(member_field->data))->num_elements;

                        *pNode = token;
                    }
                }
                else
                {
                    error(token->parent->parent, "ERROR: There's no valid Template Instruction definition");
                }
            }
            else
            {
                error(token->parent->parent, "ERROR: Missing member to associate to HEX_DUMP");
            }
        }
    }

    return instruction;
}

void * process_PAYLOAD_TRAILER(Node ** pNode, void * context)
{
    TInstruction * instruction = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        Node * token = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);

        if (IS_VALID_TOKEN_NODE_HIERARCHY(token))
        {
            if ((context == NULL) && (token_get_type(token) == TOKEN_PAYLOAD_TRAILER))
            {
                TokenInfo * token_info = (TokenInfo *) token->data;

                token = get_next_token_node(token);

                if (IS_VALID_TOKEN_NODE_HIERARCHY(token)
                    && (token->parent->next == NULL)
                    && (token->prev == NULL)
                    && (token->next == NULL)
                    && ((token_get_type(token) == TOKEN_HEX_DUMP)
                     || (token_get_type(token) == TOKEN_STRING)))
                {
                    instruction = new_tinstruction();

                    ASSERT_TRACE(instruction != NULL, "instruction != NULL");

                    if (instruction != NULL)
                    {
                        token_info = duplicate_tokenInfo(token_info);

                        ASSERT_TRACE(token_info != NULL, "token_info != NULL");

                        instruction->array_token_info->append_object(instruction->array_token_info, token_info);

                        token_info = duplicate_tokenInfo((TokenInfo *) token->data);

                        ASSERT_TRACE(token_info != NULL, "token_info != NULL");

                        instruction->array_token_info->append_object(instruction->array_token_info, token_info);
                    }
                }
                else
                {
                    error(token->parent->parent, "ERROR: There's no valid Template Instruction definition");
                }
            }
            else
            {
                error(token->parent->parent, "ERROR: PAYLOAD_TRAILER must not be associated to a member");
            }
        }
    }

    return instruction;
}

void * process_TEMPLATE_INSTRUCTION(Node ** pNode, void * context)
{
    void * instruction = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");

    if (pNode != NULL)
    {
        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(*pNode);

        if (IS_VALID_TOKEN_NODE_HIERARCHY(*pNode))
        {
            Node * token = get_next_token_node(*pNode);

            if (IS_VALID_TOKEN_NODE_HIERARCHY(token) && (token_get_process(token) != NULL))
            {
                instruction = token_get_process(token)(&token, context);

                if (instruction != NULL)
                {
                    *pNode = token;
                }
            }
            else
            {
                error((*pNode)->parent->parent, "ERROR: No process on Template Instruction");
            }
        }
    }

    return instruction;
}

void * process_STRUCT_TEMPLATE(Node ** pNode, void * context)
{
    Template * struct_template = NULL;

    ASSERT_TRACE(pNode != NULL, "pNode != NULL");
    ASSERT_TRACE((pNode == NULL) || (*pNode != NULL), "*pNode != NULL");

    if (pNode != NULL)
    {
        Node * current_node = *pNode;

        ASSERT_TRACE_TOKEN_NODE_HIERARCHY(current_node);
        ASSERT_TRACE(token_get_type(current_node) == TOKEN_STRUCT_TEMPLATE,
                    "token_get_type(current_node) == TOKEN_STRUCT_TEMPLATE");

        if (IS_VALID_TOKEN_NODE_HIERARCHY(current_node)
            && (token_get_type(current_node) == TOKEN_STRUCT_TEMPLATE))
        {
            Node * token = current_node;
            Node * word = token->parent;
            Node * line = word->parent;
            char * str_doc = document_get_string(line->parent);

            token = navigate_tree_get_prev_vtable(current_node, &node_vtable_token);

            if (IS_VALID_TOKEN_NODE(token) && (token_get_type(token) == TOKEN_define))
            {
                token = navigate_tree_get_prev_vtable(token, &node_vtable_token);

                if (IS_VALID_TOKEN_NODE(token) && (token_get_type(token) == TOKEN_NUMSIGN))
                {
                    char * word_str = word_get_string(word);
                    char * token_str = token_get_string(current_node);

                    if (strlen(token_str) < strlen(word_str))
                    {
                        char *          id_name = &word_str[strlen(token_str)];
                        char *          id_value_str = NULL;
                        unsigned int    id_value_str_len;
                        long int        id_value;


                        word = navigate_tree_get_next_vtable(current_node, &node_vtable_word);

                        if (IS_VALID_WORD_NODE(word) && IS_VALID_TOKEN_NODE(word->first_child)
                            && (token_get_position(word->first_child) < strlen(str_doc)))
                        {
                            token = word->first_child;
                            id_value_str = &str_doc[token_get_position(token)];

                            // TODO: Check if is better to use the token tree or something different
                            for (id_value_str_len = 0;
                                ((token_get_position(token) + id_value_str_len) < strlen(str_doc))
                                && ( (id_value_str[id_value_str_len] == ' ')                                  
                                  || (id_value_str[id_value_str_len] == '(')
                                  || (id_value_str[id_value_str_len] == ')')
                                  || (id_value_str[id_value_str_len] == 'x')
                                  || (id_value_str[id_value_str_len] == 'X')
                                  || (id_value_str[id_value_str_len] == '+')
                                  || (id_value_str[id_value_str_len] == '*')
                                  || (('0' <= id_value_str[id_value_str_len]) && (id_value_str[id_value_str_len] <= '9'))
                                  || (('a' <= id_value_str[id_value_str_len]) && (id_value_str[id_value_str_len] <= 'f'))
                                  || (('A' <= id_value_str[id_value_str_len]) && (id_value_str[id_value_str_len] <= 'F')));
                                id_value_str_len ++);

                                if (evaluate_expression(id_value_str, id_value_str_len, &id_value) == SUCCESS)
                                {
                                    for (token = get_next_token_node(token);
                                        IS_VALID_TOKEN_NODE(token)
                                        && (token_get_type(token) != TOKEN_struct)
                                        && (token_get_type(token) != TOKEN_TEMPLATE_INSTRUCTION)
                                        && (token_get_type(token) != TOKEN_END_TEMPLATE);
                                        token = get_next_token_node(token));

                                    if (IS_VALID_TOKEN_NODE(token) && (token_get_type(token) != TOKEN_END_TEMPLATE))
                                    {
                                        Node *          struct_field = NULL;
                                        Array *         array_ti = NULL;
                                        TInstruction *  instruction = NULL;

                                        array_ti = new_array(delete_tinstruction, duplicate_tinstruction);

                                        ASSERT_TRACE(array_ti != NULL, "array_ti != NULL");

                                        if ((array_ti != NULL) && (token_get_process(token) != NULL))
                                        {
                                            word = token->parent;
                                            line = word->parent;

                                            switch (token_get_type(token))
                                            {
                                            case TOKEN_struct:                                                
                                                {
                                                    PFContext  pf_context = {NULL, array_ti};

                                                    struct_field = token_get_process(token)(&token, &pf_context);

                                                    if (struct_field == NULL)
                                                    {
                                                        delete_array(array_ti);
                                                        array_ti = NULL;
                                                    }
                                                }

                                                break;

                                            case TOKEN_TEMPLATE_INSTRUCTION:
                                                {
                                                    Node * next_token;

                                                    for (next_token = get_next_token_node(token);
                                                        IS_VALID_TOKEN_NODE(next_token)
                                                        && (token_get_type(next_token) != TOKEN_END_TEMPLATE)
                                                        && (array_ti != NULL);
                                                        next_token = get_next_token_node(token))
                                                    {
                                                        word = token->parent;
                                                        line = word->parent;

                                                        if ((token_get_type(token) == TOKEN_TEMPLATE_INSTRUCTION)
                                                            && (token_get_type(next_token) != TOKEN_END_TEMPLATE)
                                                            && (token_get_process(next_token) != NULL))
                                                        {
                                                            instruction = token_get_process(next_token)(&next_token, NULL);

                                                            if (instruction != NULL)
                                                            {
                                                                array_ti->append_object(array_ti, instruction);
                                                                *pNode = next_token;
                                                            }
                                                            else
                                                            {
                                                                delete_array(array_ti);
                                                                array_ti = NULL;
                                                            }
                                                        }

                                                        token = next_token;
                                                    }

                                                    if (!IS_VALID_TOKEN_NODE(token))
                                                    {
                                                        delete_array(array_ti);
                                                        array_ti = NULL;
                                                        error(line, "ERROR: There's no valid Struct Template definition");
                                                    }
                                                }
                                                break;

                                            default:
                                                delete_array(array_ti);
                                                array_ti = NULL;
                                                ASSERT_TRACE(0 ,"There's a mechanism error");
                                            }

                                            for (; IS_VALID_TOKEN_NODE(token) && (token_get_type(token) != TOKEN_END_TEMPLATE);
                                                token = get_next_token_node(token));

                                            if (!IS_VALID_TOKEN_NODE(token) || (token_get_type(token) != TOKEN_END_TEMPLATE))
                                            {
                                                delete_array(array_ti);
                                                array_ti = NULL;

                                                destroy_tree(struct_field);
                                                struct_field = NULL;

                                                error(line, "ERROR: Is missing the END_TEMPLATE instruction in template definition");
                                            }

                                            if (array_ti != NULL)
                                            {
                                                struct_template = new_template(id_name,
                                                                               id_value,
                                                                               struct_field,
                                                                               array_ti);
                                                *pNode = token;

                                                ASSERT_TRACE(struct_template != NULL, "struct_template != NULL");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        error(line, "ERROR: Incomplete template definition");
                                    }
                                }
                                else
                                {
                                    error(line, "ERROR: The ID VALUE cannot be evaluated");
                                }
                        }
                        else
                        {
                            error(line, "ERROR: There is no template ID VALUE");
                        }
                    }
                    else
                    {
                        error(line, "ERROR: There is no template ID NAME");
                    }
                }
                else
                {
                    error(line, "ERROR: Is missing the \"#\" token");
                }
            }
            else if (!(IS_VALID_TOKEN_NODE(token) && (token_get_type(token) == TOKEN_RIGHT_CURLY_BRACKET)))
            {
                error(line, "ERROR: Is missing the \"define\" token");
            }
        }
    }

    return struct_template;
}

void parser_reset(Parser * parser)
{
    ASSERT_TRACE(parser != NULL, "parser != NULL");

    if (parser != NULL)
    {
        delete_buffer((void **) &(parser->string));

        destroy_tree(parser->token_tree);
        parser->token_tree = NULL;

        delete_symbolTable(parser->symbolTable);
        parser->symbolTable = NULL;
    }
}

void parser_set_string(Parser * parser, char * string)
{
    ASSERT_TRACE(parser != NULL, "parser != NULL");
    ASSERT_TRACE(string != NULL, "string != NULL");
    ASSERT_TRACE((string != NULL) && (strlen(string) > 0), "strlen(string) >0");

    if ((parser != NULL)
        && (string != NULL)
        && (strlen(string) > 0))
    {
        parser_reset(parser);

        parser->string = duplicate_buffer((void *) string, sizeof(char) * (strlen(string) + 1));

        ASSERT_TRACE(parser->string != NULL, "parser->string != NULL");
        ASSERT_TRACE((parser->string == NULL) || (strlen(parser->string) == strlen(string)),
                     "strlen(parser->string) == strlen(string)");
        ASSERT_TRACE((parser->string == NULL) || (strcmp(parser->string, string) == 0),
                     "strcmp(parser->string, string) == 0");
    }
}

Node * parser_create_token_tree(char * str, long int str_len)
{ // Lexical analysis,
    Node * token_tree = NULL;

    ASSERT_TRACE(str != NULL, "str != NULL");
    ASSERT_TRACE(str_len > 0, "str_len > 0");

    if ((str != NULL) && (str_len > 0))
    {
        Node *      document_node;
        Node *      line_node;
        Node *      word_node;
        Node *      token_node;
        long int    current_pos;
        long int    word_end_pos;
        char *      token_str;
        int         token_len;
        Token       token_found;
        int         j;
        int         i;

        document_node = new_node(&node_vtable_document);

        ASSERT_TRACE(document_node != NULL, "document_node != NULL");

        document_set_string(document_node, str, str_len);

        token_tree = document_node;

        line_node = new_node(&node_vtable_line);

        ASSERT_TRACE(line_node != NULL, "line_node != NULL");

        append_child(document_node, line_node);

        current_pos = 0;

        while ((document_node != NULL)
               && (line_node != NULL)
               && (current_pos < str_len))
        {
            while ((current_pos < str_len) && (isspace(str[current_pos]) != 0))
            {
                if (str[current_pos] == '\n')
                {
                    line_node = new_node(&node_vtable_line);

                    ASSERT_TRACE(line_node != NULL, "line_node != NULL");

                    append_child(document_node, line_node);
                }

                current_pos ++;
            }

            for (word_end_pos = current_pos;
                (word_end_pos < str_len) && (isspace(str[word_end_pos]) == 0);
                word_end_pos ++);

            if ((line_node != NULL) && (word_end_pos <= str_len) && (current_pos < word_end_pos))
            {
                word_node = new_node(&node_vtable_word);

                ASSERT_TRACE(word_node != NULL, "word_node != NULL");

                word_set_string(word_node, &str[current_pos], word_end_pos - current_pos);

                append_child(line_node, word_node);

                i = current_pos;

                while ((current_pos < word_end_pos)  && (word_node != NULL))
                {
                    token_found = TOKEN_NOTOKEN;

                    for (j = 0;
                        (j < NUM_ELEMENTS_TOKEN_INFO_GROUP)
                        && (token_found == TOKEN_NOTOKEN);
                        j ++)
                    {
                        token_str = (char *) token_info_group[j].string;

                        //ASSERT_TRACE(token_str != NULL, "token_str != NULL");

                        if (token_str != NULL)
                        {
                            token_len = (int) strlen(token_str);

                            ASSERT_TRACE(token_len > 0, "token_len > 0");

                            if ((token_len > 0)
                                && (token_len <= (word_end_pos - current_pos))
                                && (strncmp(&str[current_pos], token_str, token_len) == 0))
                            {
                                token_found = token_info_group[j].token;

                                ASSERT_TRACE((TOKEN_START < token_found) && (token_found < TOKEN_END),
                                            "(TOKEN_START < token_found) && (token_found < TOKEN_END)");

                                if ((TOKEN_START < token_found) && (token_found < TOKEN_END))
                                {   // A valid token has been found !!
                                    if (i < current_pos)
                                    {
                                        token_node = new_node(&node_vtable_token);
                                        token_set_string(token_node, &str[i], current_pos - i);
                                        token_set_position(token_node, i);
                                        token_set_type(token_node, TOKEN_UNKNOWN);
                                        append_child(word_node, token_node);
                                    }

                                    token_node = new_node(&node_vtable_token);
                                    token_set_string(token_node, token_str, token_len);
                                    token_set_position(token_node, current_pos);
                                    token_set_process(token_node, token_info_group[j].process);
                                    token_set_type(token_node, token_found);
                                    token_set_fieldSize(token_node, token_info_group[j].fieldSize);
                                    append_child(word_node, token_node);

                                    current_pos += token_len;
                                    i = current_pos;
                                }
                            }
                        }
                    }

                    ASSERT_TRACE((TOKEN_START < token_found) && (token_found < TOKEN_END),
                                "(TOKEN_START < token_found) && (token_found < TOKEN_END)");

                    if (token_found == TOKEN_NOTOKEN)
                    {
                        current_pos ++;
                    }
                }

                if (i < current_pos)
                {
                    token_node = new_node(&node_vtable_token);
                    token_set_string(token_node, &str[i], current_pos - i);
                    token_set_position(token_node, i);
                    token_set_type(token_node, TOKEN_UNKNOWN);
                    append_child(word_node, token_node);
                }
            }
        }
    }

    return token_tree;
}

SymbolTable * parser_create_symbol_table(Node * token_tree)
{
    SymbolTable * symbol_table = NULL;

    ASSERT_TRACE(IS_VALID_DOCUMENT_NODE(token_tree),
                "IS_VALID_DOCUMENT_NODE(token_tree)");

    if (IS_VALID_DOCUMENT_NODE(token_tree))
    {
        symbol_table = new_symbolTable();

        ASSERT_TRACE_SYMBOL_TABLE(symbol_table);

        if (IS_VALID_SYMBOL_TABLE(symbol_table))
        {
            Node *      token;
            Array *     array;
            Object      object;
            Token       token_type;

            for (token = get_next_token_node(token_tree);
                 token != NULL;
                 token = get_next_token_node(token))
            {
                ASSERT_TRACE_TOKEN_NODE_HIERARCHY(token);

                token_type = token_get_type(token);

                if (IS_VALID_TOKEN_NODE(token)
                    && (token_get_process(token) != NULL)
                    && ((token_type == TOKEN_STRUCT_TEMPLATE)
                     || (token_type == TOKEN_MAP_TEMPLATE)))
                {
                    object = token_get_process(token)(&token, NULL);

                    if (object != NULL)
                    {
                        switch (token_type)
                        {
                        case TOKEN_STRUCT_TEMPLATE:
                            array = symbol_table->array_template;
                            break;

                        case TOKEN_MAP_TEMPLATE:
                            array = symbol_table->array_map;
                            break;
                        default:
                            ASSERT_TRACE(0, "There is a mechanism error");
                        }

                        ASSERT_TRACE(array != NULL, "array != NULL");

                        if (array != NULL)
                        {
                            array->append_object(array, object);
                        }
                    }
                }
            }
        }
    }

    return symbol_table;
}

int parser_start_parsing(Parser * parser)
{
    int result = SUCCESS;

    ASSERT_TRACE(parser != NULL, "parser != NULL");
    ASSERT_TRACE((parser == NULL) || (parser->string != NULL),
                 "parser->string != NULL");
    ASSERT_TRACE((parser == NULL)
                 || (parser->string == NULL)
                 || (strlen(parser->string) > 0),
                 "strlen(parser->string) > 0");

    if ((parser != NULL) && (parser->string != NULL) && (strlen(parser->string) > 0))
    {
        // Lexical analysis
        parser->token_tree = parser_create_token_tree(parser->string, (long) strlen(parser->string));

        ASSERT_TRACE(parser->token_tree != NULL, "parser->token_tree != NULL");

        parser->symbolTable = parser_create_symbol_table(parser->token_tree);

        ASSERT_TRACE(parser->symbolTable != NULL, "parser->symbolTable != NULL");
    }
    return result;
}

int get_max_length_of_token(TokenInfo * token_info_array, int num)
{
    unsigned int max_length = 0;

    ASSERT_TRACE(token_info_array != NULL, "token_info_array != NULL");
    ASSERT_TRACE((num > 0) && (num != (unsigned int)(-1)), "(num > 0) && (num != (unsigned int)(-1))");

    if ((token_info_array != NULL) && (num > 0) && (num != (unsigned int)(-1)))
    {
        while ((--num) >= 0)
            if ((token_info_array[num].string != NULL)
                && (max_length < strlen(token_info_array[num].string)))
            {
                max_length = (unsigned int) strlen(token_info_array[num].string);
            }
    }

    return max_length;
}

#endif // _parser_engine_c_
