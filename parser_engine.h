// 0000 file created by Yarib Nevarez @ Gdl IBM
#ifndef _parser_engine_h_
#define _parser_engine_h_

typedef enum
{
    TOKEN_START,
    TOKEN_auto,
    TOKEN_double,
    TOKEN_int,
    TOKEN_struct,
    TOKEN_break,
    TOKEN_else,
    TOKEN_long,
    TOKEN_switch,
    TOKEN_case,
    TOKEN_enum,
    TOKEN_register,
    TOKEN_typedef,
    TOKEN_char,
    TOKEN_extern,
    TOKEN_return,
    TOKEN_union,
    TOKEN_const,
    TOKEN_float,
    TOKEN_short,
    TOKEN_unsigned,
    TOKEN_continue,
    TOKEN_for,
    TOKEN_signed,
    TOKEN_void,
    TOKEN_default,
    TOKEN_goto,
    TOKEN_sizeof,
    TOKEN_volatile,
    TOKEN_do,
    TOKEN_if,
    TOKEN_static,
    TOKEN_while,
    TOKEN_NUMSIGN,
    TOKEN_define,
    TOKEN_QUOT,
    TOKEN_REVERSE_SOLIDUS,
    TOKEN_STRUCT_TEMPLATE,
    TOKEN_TEMPLATE_INSTRUCTION,
    TOKEN_DEFAULT,
    TOKEN_ERROR,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_END_TEMPLATE,
    TOKEN_MAP_TEMPLATE,
    TOKEN_MAP,
    TOKEN_END_MAP,
    TOKEN_LEFT_CURLY_BRACKET,
    TOKEN_RIGHT_CURLY_BRACKET,
    TOKEN_LEFT_SQUARE_BRACKET,
    TOKEN_RIGHT_SQUARE_BRACKET,
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,
    TOKEN_COMMENT,
    TOKEN_COMMENT_OPEN,
    TOKEN_COMMENT_CLOSE,
    TOKEN_COMMA,
    TOKEN_EQUALS_SIGN,
    TOKEN_ID,
    TOKEN_UINT32,
    TOKEN_UINT16,
    TOKEN_UINT8,
    TOKEN_INT32,
    TOKEN_INT16,
    TOKEN_INT8,
    TOKEN_BOOLEAN,
    TOKEN_Int,
    TOKEN_Short,
    TOKEN_Char,
    TOKEN_STRING,
    TOKEN_HEX_DUMP,
    TOKEN_PAYLOAD_TRAILER,
    TOKEN_STRUCT_TAG_ID,
    TOKEN_UNKNOWN,
    TOKEN_NOTOKEN,
    TOKEN_END
} Token;

typedef void *      (*TokenProcess)(Node **, void *);

unsigned int        field_node_get_size(Node * node);
int                 field_node_get_offset(Node * node);

typedef struct
{
    char *          id_name;
    Array *         array_token_info;           // TokenInfo *
    unsigned int    num_elements;
    unsigned int    bits;
    unsigned int    bit_field_member_mask;
    unsigned int    bit_field_size;
    unsigned int    field_size;
    unsigned int    field_offset;
} FieldData;

typedef struct
{
    Token           token;
    TokenProcess    process;
    char *          string;
    unsigned int    position;
    unsigned int    fieldSize;
} TokenInfo;

typedef struct
{
    Array *         array_format_string;        // char *
    char *          map_name;
    Array *         array_token_info;           // TokenInfo *
    int             iterations;
    NodePath *      data_path;
} TInstruction;

typedef struct
{
    char *          id_name;
    long int        id_value;
    Node *          struct_tree;
    Array *         array_template_instructions; // TInstruction *
} Template;


typedef struct
{
    long int        value;
    char *          name;
    char *          format_string;
} MapElement;

typedef struct
{
    char *          id_name;
    Array *         array_map_elements;         // MapElement *
    char *          default_string;
} Map;

typedef struct
{
    Array *         array_template;             // Template *
    Array *         array_map;                  // Map *
} SymbolTable;

typedef struct
{
    char *          string;
    Node *          token_tree;
    SymbolTable *   symbolTable;
} Parser;


    // Parser engine functions.

    Parser *        new_parser(void);
    void            parser_set_string(Parser * parser, char * string);
    int             parser_start_parsing(Parser * parser);
    void            delete_parser(Parser * parser);

#endif // _parser_engine_h_
