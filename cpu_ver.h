#define MEGADEBUG

#include <stdio.h>
#include <assert.h>
#include "stack.h"


const int REG_POISON = -100500;
const int REGNUM = 8;
const int RAMSIZE = 10 * 1024;
const int STACKSIZE = 100;
const int MARKNUM = 100;
const int RETNUM = 100;

enum REGISTERS 
{
    ax = 0,
    bx = 1,
    cx = 2,
    dx = 3,
    ex = 4,
    fx = 5,
    gx = 6,
    hx = 7,
};




struct CPU_t
{
    Stack_t stk; // stack
    Stack_t retstk; // return stack
    int pos; // position in program array
    double reg[REGNUM]; // array of registers
    char ram[RAMSIZE]; // CPU's RAM
};




bool CPU_Constructor (CPU_t* This);
bool CPU_Distructor (CPU_t* This);
bool CPU_OK (CPU_t* This);
bool CPU_Dump (CPU_t* This);




bool CPU_Constructor (CPU_t* This)
{
    assert (This);
    
    Stack_Constructor (&(This -> stk), STACKSIZE);
    
    Stack_Constructor (&(This -> retstk), RETNUM);

    for (int i = 0; i < REGNUM; i++)
        This -> reg[i] = NULL;
    
    for (int i = 0; i < RAMSIZE; i++)
        This -> ram[i] = NULL;
    
    This -> pos = 0;
    
    return (CPU_OK (This));
}




bool CPU_Distructor (CPU_t* This)
{
    STRUCT_CHECK (CPU)
    else
    {
        Stack_Distructor (&(This -> stk));

        for (int i = 0; i < REGNUM; i++)
            This -> reg[i] = REG_POISON;
        
        for (int i = 0; i < RAMSIZE; i++)
            This -> ram[i] = 0;
        
        This -> pos = BAD_POINTER;
        This = NULL;
        
        return (!CPU_OK (This));
    }
}




bool CPU_OK (CPU_t* This)
{
    return This &&
           Stack_OK (&(This -> stk)) &&
           Stack_OK (&(This -> retstk)) &&
           This -> reg &&
           This -> ram &&
           (This -> pos >= 0);
}




bool CPU_Dump (CPU_t* This)
{
    FILE* f = fopen ("Report.txt", "a");
    fprintf (f, "CPU_t %s %#x ", STRUCT_NAME (cpu), This);
    
    if (CPU_OK (This)) fprintf (f, "[OK]\n{\n");
    else fprintf (f, "[Error!!!]\n{\n");
    
    fprintf (f, "\tregisters:\n");
    for (int i = 0; i < REGNUM; i++)
        fprintf (f, "\t%cx = %lg\n", ('a' + i), This -> reg[i]);
    
    fprintf (f, "\n\n\tRAM: %s\n", This -> ram);

    fprintf (f, "\n\tpos = %d\n", This -> pos);
    
    Stack_Dump (&(This -> stk));
    
    fclose (f);
    
    return f;
}
