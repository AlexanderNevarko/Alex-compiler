// CPU  (c)AlexN  v.2.0
const int OP_VERSION = 2;



#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "cpu_ver.h" 


const int NO_MARK = -1488;
long Filesize (FILE* f);


void CPU_Add (CPU_t* This);     // 65
void CPU_Sub (CPU_t* This);     // 66
void CPU_Mul (CPU_t* This);     // 67
void CPU_Div (CPU_t* This);     // 68
void CPU_In (CPU_t* This);      // 69  
void CPU_Out (CPU_t* This);     // 70
void CPU_Sin (CPU_t* This);     // 71
void CPU_Cos (CPU_t* This);     // 72
void CPU_Tan (CPU_t* This);     // 73
void CPU_Sqrt (CPU_t* This);    // 74

bool CPU_Push (CPU_t* This, double val);    // 75
double CPU_Pop (CPU_t* This);     // 76
bool CPU_Pushr (CPU_t* This, char nreg);    // 77
void CPU_Popr (CPU_t* This, char nreg);     // 78

void CPU_Mark (CPU_t* This);       // 79 puts name of mark in marks array
bool CPU_Jmp (CPU_t* This, int mark);       // 80 }
bool CPU_Je (CPU_t* This, int mark);        // 81 |
bool CPU_Jne (CPU_t* This, int mark);       // 82 |
bool CPU_Ja (CPU_t* This, int mark);        // 83 }  puts address of mark in cpu.pos
bool CPU_Jb (CPU_t* This, int mark);        // 84 |
bool CPU_Jae (CPU_t* This, int mark);       // 85 |
bool CPU_Jbe (CPU_t* This, int mark);       // 86 }

bool CPU_Call (CPU_t* This, int addr); // 90 // define this functions!!!
bool CPU_Ret (CPU_t* This); // 91


// JOKE FUNCTIONS
void CPU_Cwah () // 100
{
    printf ("\n");
    FILE* girl = fopen ("girl.txt", "r");
    char line[100] = {};
    char c = fgetc (girl);
    int linenum = 0;
    while (c != EOF)
    {
        if (c == '\n') linenum++;
        c = fgetc (girl);
    }
    rewind (girl);
    for (int i = 0; i < linenum; i++)
    {
        fgets (line, 100, girl);
        printf ("%s", line);
    }
    printf ("\n"); 
}

bool CPU_Jm (CPU_t* This, int mark) // 87
{
    STRUCT_CHECK (CPU);
    srand (time (NULL));
    int maybe = rand() % 2;
    printf ("maybe: %d\n", maybe);
    if (maybe)
        This -> pos = mark;
    else
        This -> pos += sizeof (int) + sizeof (char);
    STRUCT_CHECK (CPU);
    return (mark == NO_MARK);
}
/////////////




void CPU_Add (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double sum = Stack_Pop (&(This -> stk)) + Stack_Pop (&(This -> stk));
    Stack_Push (&(This -> stk), sum);
    STRUCT_CHECK (CPU);
}




void CPU_Mul (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double multiplication = Stack_Pop (&(This -> stk)) * Stack_Pop (&(This -> stk));
    Stack_Push (&(This -> stk), multiplication);
    STRUCT_CHECK (CPU);
}




void CPU_Sub (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    Stack_Push (&(This -> stk), (val2 - val1));
    STRUCT_CHECK (CPU);
}




void CPU_Div (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    assert ((val1) && "Unallowed value of divisor! Divisor == 0!");
    Stack_Push (&(This -> stk), (val2 / val1));
    STRUCT_CHECK (CPU);
}




void CPU_Sin (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double arg = Stack_Pop (&(This -> stk));
    Stack_Push (&(This -> stk), sin (arg));
    STRUCT_CHECK (CPU);
}




void CPU_Cos (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double arg = Stack_Pop (&(This -> stk));
    Stack_Push (&(This -> stk), cos (arg));
    STRUCT_CHECK (CPU);
}




void CPU_Tan (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double arg = Stack_Pop (&(This -> stk));
    Stack_Push (&(This -> stk), tan (arg));
    STRUCT_CHECK (CPU);
}




void CPU_Sqrt (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double arg = Stack_Pop (&(This -> stk));
    Stack_Push (&(This -> stk), sqrt (arg));
    STRUCT_CHECK (CPU);
}




void CPU_In (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double val = POISON;
    scanf ("%lg", &val);
    Stack_Push (&(This -> stk), val);
    STRUCT_CHECK (CPU);
}




void CPU_Out (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    double output = Stack_Pop (&(This -> stk));
    printf ("out: %lg\n", output);
    STRUCT_CHECK (CPU);
}




bool CPU_Push (CPU_t* This, double val)
{
    STRUCT_CHECK (CPU);
    return Stack_Push (&(This -> stk), val);
}




double CPU_Pop (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    return (Stack_Pop (&(This -> stk)));
    STRUCT_CHECK (CPU);
}




bool CPU_Pushr (CPU_t* This, char nreg)
{
    STRUCT_CHECK (CPU);
    return Stack_Push (&(This -> stk), This -> reg[nreg]);
}




void CPU_Popr (CPU_t* This, char nreg)
{
    STRUCT_CHECK (CPU);
    This -> reg[nreg] = Stack_Pop (&(This -> stk));
    STRUCT_CHECK (CPU);
}




void CPU_Mark (CPU_t* This)
{
    STRUCT_CHECK (CPU);
}




bool CPU_Jmp (CPU_t* This, int mark)
{
    STRUCT_CHECK (CPU);
    This -> pos = mark;
    STRUCT_CHECK (CPU);
    return (mark != NO_MARK);
}




bool CPU_Je (CPU_t* This, int mark)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    if (val1 == val2)
    {
        This -> pos = mark;
    }
    else
    {
        This -> pos += sizeof (int) + sizeof (char);
    }

    STRUCT_CHECK (CPU);
    return (mark == NO_MARK);
}




bool CPU_Jne (CPU_t* This, int mark)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    if (val1 != val2)
    {
        This -> pos = mark;
    }
    else
    {
        This -> pos += sizeof (int) + sizeof (char);
    }

    STRUCT_CHECK (CPU);
    return (mark == NO_MARK);
}




bool CPU_Ja (CPU_t* This, int mark)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    if (val1 < val2)
    {
        This -> pos = mark;
    }
    else
    {
        This -> pos += sizeof (int) + sizeof (char);
    }

    STRUCT_CHECK (CPU);
    return (mark == NO_MARK);
}




bool CPU_Jb (CPU_t* This, int mark)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    if (val1 > val2)
    {
        This -> pos = mark;
    }
    else
    {
        This -> pos += sizeof (int) + sizeof (char);
    }

    STRUCT_CHECK (CPU);
    return (mark == NO_MARK);
}




bool CPU_Jae (CPU_t* This, int mark)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    if (val1 <= val2)
    {
        This -> pos = mark;
    }
    else
    {
        This -> pos += sizeof (int) + sizeof (char);
    }

    STRUCT_CHECK (CPU);
    return (mark == NO_MARK);
}




bool CPU_Jbe (CPU_t* This, int mark)
{
    STRUCT_CHECK (CPU);
    double val1 = Stack_Pop (&(This -> stk));
    double val2 = Stack_Pop (&(This -> stk));
    if (val1 >= val2)
    {
        This -> pos = mark;
    }
    else
    {
        This -> pos += sizeof (int) + sizeof (char);
    }
    
    STRUCT_CHECK (CPU);
    return (mark == NO_MARK);
}




bool CPU_Call (CPU_t* This, int addr)
{
    STRUCT_CHECK (CPU);
    bool t = Stack_Push (&(This -> retstk), (This -> pos + sizeof (char) + sizeof (int)));
    This -> pos = addr;
    STRUCT_CHECK (CPU);
    return t;
}




bool CPU_Ret (CPU_t* This)
{
    STRUCT_CHECK (CPU);
    This -> pos = Stack_Pop (&(This -> retstk));
    STRUCT_CHECK (CPU);
    return This -> pos;
}




long Filesize (FILE* f)
{
    long sz = -1;
    fseek (f, 0, SEEK_END);
    sz = ftell (f);
    rewind (f);
    assert (!(sz == 0));
    assert (!(sz == -1));
    //printf ("size of file = %d\n", sz);
    return sz;
}

