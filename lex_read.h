#define LEX_READ
#include <fcntl.h>
#include <ctype.h>
#include "list_spec.h"



const int MAX_NUM_LENGTH = 30;
const int MAX_ID_LENGTH = 50;
const int MAX_OP = 3;




enum LIST_ERRORS_t
{
    SYNTAX = 1,
    BRACKETS = 2,
    EMP_STR = 3,
    WRONG_DOUBLE = 4
};


List_Elem* Error (LIST_ERRORS_t errnum)
{
    printf ("\nLexem error (list) %d\n", errnum);
    assert (0);
    return NULL;
}




List* Lex_reader      (int file_descriptor);
List* GetG0           (char** str);
List_Elem* GetOp      (char** str);
List_Elem* GetBrack   (char** str);
List_Elem* GetNum     (char** str);
List_Elem* GetId      (char** str);
List_Elem* GetPunct   (char** str);

char* Skip_spaces (char** str)
{
    while (isspace (**str)) *str += 1;
    return *str;
}




List* Lex_reader (int fd)
{
    long f_size = lseek (fd, 0L, SEEK_END);
    printf ("fsize = %d\n", f_size);
    char* buf = (char*) calloc (f_size, sizeof (*buf));
    lseek (fd, 0L, SEEK_SET);
    long f_length = read (fd, buf, f_size);
    buf[f_length] = '\0';
    char* buf_cpy = buf;
    
    puts (buf);
    
    return GetG0 (&buf);
}




List* GetG0 (char** str)
{
    assert (str);
    Skip_spaces (str);
    if (**str == '\0') return NULL;

    List* token_list = List_Ctor ("Tokens", sizeof ("Tokens"));

    while (**str != '\0')
    {
        Skip_spaces (str);
        
        if (**str == '(' || **str == ')' || **str == '{' || **str == '}')
        {
            Put_in_tail (token_list, GetBrack (str));
            Skip_spaces (str);
        }
        else if (isdigit (**str))
        {
            Put_in_tail (token_list, GetNum (str));
            Skip_spaces (str);
        }
        else if (isalpha (**str) || **str == '_')
        {
            Put_in_tail (token_list, GetId (str));
            Skip_spaces (str);
        }
        else if (**str == '+' || **str == '-' || **str == '*' || **str == '/' || **str == '^' ||
                 **str == '=' || **str == '>' || **str == '<' || **str == '!')
        {
            Put_in_tail (token_list, GetOp (str));
            Skip_spaces (str);
        }
        else if (**str == ';' || **str == ',')
        {
            Put_in_tail (token_list, GetPunct (str));
            Skip_spaces (str);
        }
        else
        {
            Error (SYNTAX);
            return NULL;
        }
    }

    return token_list;
}




List_Elem* GetBrack (char** str)
{
    Skip_spaces (str);
    if (**str == '\0') return Error (EMP_STR);

    if (**str == '(')
    {
        *str += 1;
        return Elem_Ctor ("(", 2, OPEN_BRACK);
    }
    else if (**str == ')')
    {
        *str += 1;
        return Elem_Ctor (")", 2, CLOSE_BRACK);
    }
    else if (**str == '{')
    {
        *str += 1;
        return Elem_Ctor ("{", 2, OPEN_BLOCK);
    }
    else if (**str == '}')
    {
        *str += 1;
        return Elem_Ctor ("}", 2, CLOSE_BLOCK);
    }
    else return Error (BRACKETS);
}




List_Elem* GetOp (char** str)
{
    Skip_spaces (str);
    if (**str == '\0') return Error (EMP_STR);

    char op_str[MAX_OP] = "";
    int count = 0;
    
    while (**str == '+' || **str == '-' || **str == '*' || **str == '/' || **str == '^' ||
           **str == '=' || **str == '>' || **str == '<' || **str == '!')
    {
        op_str[count] = **str;
        count++;
        *str += 1;
        if (count >= MAX_OP) return Error (SYNTAX);
    }
    op_str[count] = '\0';
    
    return Elem_Ctor (op_str, (count + 1), OP);
}




List_Elem* GetNum (char** str)
{
    char number[MAX_NUM_LENGTH] = "";
    int ptr = 0;
    int points = 0;
   
    Skip_spaces (str);
    if (**str == '\0') return Error (EMP_STR);
    
    while (isdigit (**str) || **str == '.')
    {
        points += (**str == '.' ? 1 : 0);
        if (points > 1) return Error (WRONG_DOUBLE);
        
        number[ptr] = **str;
        *str += 1;
        ptr++;
    }
    number[ptr] = '\0';

    return Elem_Ctor (number, (ptr + 1), NUM);
}




List_Elem* GetId (char** str)
{
    Skip_spaces (str);
    if (**str == '\0') return Error (EMP_STR);

    char id[MAX_ID_LENGTH] = "";
    int ptr = 0;
    
    if (isalpha(**str) || **str == '_')
    {
        id[ptr] = **str;
        ptr++;
        *str += 1;
        while (isalnum (**str) || **str == '_')
        {
            id[ptr] = **str;
            ptr++;
            *str += 1;
        }
        id[ptr] = '\0';
    }
    else return Error (SYNTAX);

    return Elem_Ctor (id, (ptr + 1), ID);
}




List_Elem* GetPunct (char** str)
{
    Skip_spaces (str);
    if (**str == '\0') return Error (EMP_STR);

    if (**str == ';')
    {
        *str += 1;
        return Elem_Ctor (";", 2, DIV);
    }
    else if (**str == ',')
    {
        *str += 1;
        return Elem_Ctor (",", 2, PUNCT);
    }
    else return Error (SYNTAX);
}
