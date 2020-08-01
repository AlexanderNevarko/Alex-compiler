// solve the problem with the conflict of struct types
// problem solved
#define MEGADEBUG

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "super_struct_check.h"


const char* NELEM_DATA = "ZERO_ELEMENT";


enum lex_types_t
{
    ID,
    NUM,
    OPEN_BRACK,
    CLOSE_BRACK,
    OP,
    PUNCT,
    OPEN_BLOCK,
    CLOSE_BLOCK,
    DIV,
    END
};


struct List_Elem
{
    List_Elem* next;
    char* data;
    lex_types_t type;
    List_Elem* prev;
};


List_Elem* Elem_Ctor (const char* info, int sizeof_info, lex_types_t tp);
bool Elem_Dtor (List_Elem* This);
bool Elem_OK (List_Elem* This);
bool Elem_Dump (List_Elem* This);




List_Elem* Elem_Ctor (const char* info, int sizeof_info, lex_types_t tp)
{
    List_Elem* This = (List_Elem*) calloc (1, sizeof(*This));
	This -> next = This;
    This -> prev = This;
    
    char* str = (char*) calloc ((sizeof_info + 1), sizeof (str[0]));
    strcpy (str, info);
    This -> data = str;

    This -> type = tp;
    
	STRUCT_CHECK (Elem);
    return This;
}




bool Elem_Dtor (List_Elem* This)
{
    STRUCT_CHECK (Elem);

    //free (This -> data);
    This -> data = NULL;
    free (This);
    This = NULL;
    
    return true;
}




bool Elem_OK (List_Elem* This)
{
    return This &&
           (This) ? 
           (This -> next &&
           This -> prev &&
           This -> data) : true;
}




bool Elem_Dump (List_Elem* This)
{
    printf ("Elem_Dump\n{\n");
    if (This == 0) 
    {
        printf ("ERROR!!!\tNULL POINTER\n}\n");
        return false;
    }
    printf ("\tElem: (%s) [%0X]\n"
            "\t\tnext: %0X\n"
            "\t\tdata: %s\n"
            "\t\ttype: %d\n"
            "\t\tprev: %0X\n}\n", (Elem_OK (This) ? "OK" : "ERROR!!!"), This, This -> next, This -> data, This -> type, This -> prev);
    return true;
}







struct List
{
    List_Elem* N_elem;
    const char* name;
    int length;
};


List* List_Ctor (List*, const char*, int sizeof_name);
bool List_Dtor (List*);
bool List_OK (List*);
bool List_Dump (List*);




List* List_Ctor (const char* input_name, int sizeof_name)
{
    assert (input_name);
    List* This = (List*) calloc (1, sizeof (*This));
    This -> N_elem = Elem_Ctor (NELEM_DATA, sizeof (NELEM_DATA), END);
    
    char* str = (char*) calloc ((sizeof_name + 1), sizeof (str[0]));
    strcpy (str, input_name);
    This -> name = str;
    This -> length = 0;
    
    STRUCT_CHECK (List);
    return This;
}




bool List_Dtor (List* This)
{
    STRUCT_CHECK (List);

    List_Elem* ptr = This -> N_elem;
    for (int i = 0; i < This -> length; i++)
    {
        ptr = ptr -> next;
        Elem_Dtor (ptr -> prev);
    }
    Elem_Dtor (ptr);

    //free (This -> name);
    This -> name = NULL;
    free (This);
    This = NULL;

    return true;
}




bool List_OK (List* This)
{
    if (This == NULL || !Elem_OK (This -> N_elem) || This -> name == NULL || (This -> length < 0)) return false;
    
    List_Elem* ptr = This -> N_elem;
    for (int i = 0; i <= This -> length; i++)
    {
        if (Elem_OK (ptr) == false) return false;
        ptr = ptr -> next;
    }
    
    return true;
}




bool List_Dump (List* This)
{
    printf ("List dump:\n");
    
    if (This == 0)
    {
        printf ("List (ERROR!!!) [NULL]\n");
        return false;
    }

    printf ("List (%s) [%0X]\n", (List_OK (This) ? "OK" : "ERROR!!!"), This);

    FILE* f = fopen ("list_dump.gv", "w");
    fprintf (f, "digraph list\n{\n"
                "\t\"[%0X]\\n%s\\nlength = %d\" -> \"%s\\n%d\\n[%0X]\"\n", This, This -> name, This -> length,
                                                                           This -> N_elem -> data, This -> N_elem -> type, This -> N_elem);
    List_Elem* ptr = This -> N_elem -> next;
    
	for (int i = 0; i <= (This -> length); i++)
    {
        fprintf (f, "\t\"%s\\n%d\\n[%0X]\" -> \"%s\\n%d\\n[%0X]\"\n", ptr -> data, ptr -> type, ptr,
                                                                      ptr -> next -> data, ptr -> next -> type, ptr -> next);
        ptr = ptr -> next;
    }
    
    fprintf (f, "}\n");
    
    fclose (f);

    return List_OK (This);
}




int Put_in_tail (List* list, List_Elem* elem);
int Put_in_head (List* list, List_Elem* elem);
int Put_before (List* list, List_Elem* elem, List_Elem* list_elem);
int Put_after (List* list, List_Elem* elem, List_Elem* list_elem);
int Delete_Elem (List* list, List_Elem* elem);

List_Elem* SLOW_Find_addr_n (List* list, int num);



int Put_in_tail (List* list, List_Elem* elem)
{
    assert (list);
    
    if (elem)
    {
        elem -> next = list -> N_elem;
        elem -> prev = list -> N_elem -> prev;
    
        list -> N_elem -> prev -> next = elem;
        list -> N_elem -> prev = elem;
        list -> length++;
    }

    assert (list);
    return list -> length;
}





int Put_in_head (List* list, List_Elem* elem)
{
    assert (list);
    
    if (elem)
    {
        elem -> prev = list -> N_elem;
        elem -> next = list -> N_elem -> next;
    
        list -> N_elem -> next -> prev = elem;
        list -> N_elem -> next = elem;
        list -> length++;
    }

    assert (list);
    return list -> length;
}




int Put_before (List* list, List_Elem* input_elem, List_Elem* before_elem)
{
    assert (list && input_elem && before_elem);
    
    input_elem -> next = before_elem;
    input_elem -> prev = before_elem -> prev;
    
    before_elem -> prev -> next = input_elem;
    before_elem -> prev = input_elem;
    
    list -> length++;
    assert (list && input_elem && before_elem);
    return list -> length;
}




int Put_after (List* list, List_Elem* input_elem, List_Elem* before_elem)
{
    assert (list && input_elem && before_elem);
    
    input_elem -> prev = before_elem;
    input_elem -> next = before_elem -> next;
    
    before_elem -> next -> prev = input_elem;
    before_elem -> next = input_elem;
    
    list -> length++;
    assert (list && input_elem && before_elem);
    return list -> length;
}




int Delete_Elem (List* list, List_Elem* elem)
{
    assert (list && elem);

    elem -> prev -> next = elem -> next;
    elem -> next -> prev = elem -> prev;
    
    Elem_Dtor (elem);
    list -> length--;
    
    assert (list);
    return list -> length;
}




List_Elem* SLOW_Find_addr_n (List* list, int num)
{
    assert (list);
    List_Elem* ptr = list -> N_elem;
    for (int i = 0; i < num; i++)
    {
        ptr = ptr -> next;
    }
    assert (ptr);
    return ptr;
}
