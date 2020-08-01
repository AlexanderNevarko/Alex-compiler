#define MEGADEBUG

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "super_struct_check.h"

#define STACK_ERROR(err) {printf ("\nError # %d\n", err); assert (!"Error in stack!");}

#define STRUCT_NAME(name) #name

enum ERRORS
{
    STACK_OVERFLOW = 1,
    STACK_UNDERFLOW = 2,
    STACK_SIZE = 3
};

const int BAD_POINTER = -1;
const int BAD_LENGTH = -1;
const int POISON = -777;




struct Stack_t
{
    double* data;
    int pointer;
    int length;
};




bool Stack_Constructor (Stack_t* This, int size);
bool Stack_Distructor (Stack_t* This);
bool Stack_OK (Stack_t* This);
bool Stack_Dump (Stack_t* This);

bool Stack_Push (Stack_t* This, double val);
double Stack_Pop (Stack_t* This);
bool Stack_Push_And_Build (Stack_t* This, double val);
bool Stack_Pop_And_Cut (Stack_t* This, int cutsize);
bool Stack_Resize (Stack_t* This, int newsize);




bool Stack_Constructor (Stack_t* This, int size)
{
    assert (This);
    This -> data = (double*) calloc (size, sizeof (This -> data[0]));
    This -> pointer = 0;
    This -> length = size;
    return Stack_OK (This);
}




bool Stack_Distructor (Stack_t* This)
{
    STRUCT_CHECK (Stack)
    else
    {
        This -> pointer = BAD_POINTER;
        free (This -> data);
        This -> data = NULL;
        This -> length = BAD_LENGTH;
    }
    return (!Stack_OK (This));
}




bool Stack_OK (Stack_t* This)
{
    return This &&
           This -> data &&
          (This -> pointer) >= 0 &&
          (This -> length) >= 0;
}




/*Puts val in stack, but can not make it wider*/
bool Stack_Push (Stack_t* This, double val)
{
    STRUCT_CHECK (Stack);
    if (This -> pointer > This -> length)
    {
        STACK_ERROR (STACK_OVERFLOW);
        return false;
    }
    
    if (This -> pointer < 0)
    {
        STACK_ERROR (STACK_UNDERFLOW);
        return false;
    }
    
    This -> data [This -> pointer++] = val;
    
    STRUCT_CHECK (Stack);
    return true;
}




double Stack_Pop (Stack_t* This)
{
    STRUCT_CHECK (Stack);
	if (This -> pointer == 0) return 0;	
	This -> pointer--;
    double value = This -> data[This -> pointer];
    STRUCT_CHECK (Stack);
    return value;
}




/*Puts val in stack, and makes it wider, if it is needed*/
bool Stack_Push_And_Build (Stack_t* This, double val)
{
    STRUCT_CHECK (Stack);
    if (This -> pointer >= This -> length)
    {
        Stack_Resize (This, (This -> length + 1));
        This -> data [This -> pointer] = val;
        This -> pointer += 1;
        return (This -> data);
    };
    This -> data [This -> pointer] = val;
    This -> pointer++;
    STRUCT_CHECK (Stack);
    return true;
}




bool Stack_Pop_And_Cut (Stack_t* This, int cutsize)
{
    STRUCT_CHECK (Stack);
    Stack_Resize (This, ((This -> length) - cutsize));
    
    if (This -> pointer == 0) return 0;	
	
    This -> pointer--;
    double value = This -> data[This -> pointer];
    STRUCT_CHECK (Stack);
    return value;
}




bool Stack_Resize (Stack_t* This, int newsize)
{
    STRUCT_CHECK (Stack);
    if (newsize <= 0) 
    {
        STACK_ERROR (STACK_SIZE);
        return false;
    }

    This -> length = newsize;
    This -> data = (double*) realloc (This -> data, sizeof (This -> data[0]) * (This -> length));
    return This -> data;
}




bool Stack_Dump (Stack_t* This)
{
    FILE* f = fopen ("Report.txt", "a");
    fprintf (f, "Stack_t %s %#x ", STRUCT_NAME (stk), This);
  
    if (Stack_OK (This)) fprintf (f, "[OK]\n{\n\t");
    else fprintf (f, "[Error!!!]\n{\n\t");
  
    fprintf (f, "data = %p\n\tpointer = %d\n\tdata[%d]:\n\t{\n", This -> data, This -> pointer, This -> length);
  
    for (int i = 0; i < This -> length; i++)
        {
            if (i < This -> pointer) fprintf (f, "\t\t*data[%d] = %lg\n", i, This -> data[i]);
            else fprintf (f, "\t\tdata[%d] = %lg\n", i, This -> data[i]);
        }
  
    fprintf (f, "\t}\n}\n");
    fclose (f);
    return f;
}



