#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define NODE
#include "tree_def.h"





enum types_t
{
    BAD_OP = -1,
    TEXT = 0,
    NUMBER = 1,
    VARIABLE = 2,
    CALL = 3,
    OPERATOR = 4,
    COMPARISON = 5,
    ASSIGN = 6,
    IF_T = 7,
    ELSE_T = 8,
    WHILE_T = 9,
    DECL_T = 10,
    FUNC_T = 11,
    SIN_T = 12,
    COS_T = 13,
    TAN_T = 14,
    LN_T = 15,
    SQRT_T = 16,
    DIVIDER = 17,
    START = 18,
    RETURN_T = 19,
    TERMINATOR = 20,
    INPUT_T = 21,
    OUTPUT_T = 22
};





struct Node_t
{
    Node_t* parent;
    char* data;
    int type;
    Node_t* left;
    Node_t* right;
};




Node_t* Node_Ctor (const char* info, int type);
bool Node_Dtor (Node_t* node);
bool Node_OK (Node_t* node);
bool Node_Dump (Node_t* node);

char* my_str_dup (const char* str, int size)
{
    char* new_str = (char*) calloc (size + 1, sizeof (*new_str));
    strcpy (new_str, str);
    return new_str;
}




Node_t* Node_Ctor (const char* info, int type)
{
    assert (info);

    Node_t* node = (Node_t*) calloc (1, sizeof (Node_t));
    
    PAR = NULL;
    
    DATA = my_str_dup (info, strlen (info));
    
    TYPE = type;

    LEFT = NULL;
    RIGHT = NULL;
    return node;
}




bool Node_Dtor (Node_t* node)
{
    assert (node);
    
    free (DATA);
    DATA = NULL;
    
    free (node);
    node = NULL;

    return true;
}




bool Node_OK (Node_t* node)
{
    return (node) ? DATA && (TYPE > BAD_OP) : NULL;
}




bool Node_Dump (Node_t* node)
{
    if (node == NULL)
    {
        printf ("Node Dump (ERROR!!!) NULL POINTER!\n");
        return false;
    }
    
    bool t = Node_OK (node);
    
    printf ("Node Dump (%s) [%0X]\n{\n"
            "\tparent [%0X]\n"
            "\tdata: %s\n"
            "\ttype = %d\n"
            "\tleft [%0X]\n"
            "\tright [%0X]\n"
            "}\n", t ? "OK" : "ERROR!!!", node, PAR, DATA, TYPE, LEFT, RIGHT);
    
    return t ? true : false;
}


#undef NODE
