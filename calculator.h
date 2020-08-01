// There may be some bugs maaaan!
#include <stdio.h>

char* prog = NULL;
int pos = 0;


enum ERRORS
{
    SYNTAX = 1,
    BRACKETS = 2,
    EMP_NUM = 3,
};


int Error (int errnum)
{
    return printf ("\nSyntax error %d\n", errnum);
}


int GetStarted (char* buf);
int GetExpression ();
int GetTrouble (); //Term!!!
int GetPrimary_exp ();
int GetNum ();


int GetStarted (char* buf)
{
    //printf ("\nI am G0 func\n");
    prog = buf;
    int result = GetExpression();
    if (prog[pos] != '\0') Error (SYNTAX);
    return result;
}


int GetExpression ()
{
    //printf ("\nI am GetExpression func\n");
    int operand1 = GetTrouble ();
    //printf ("\nI am GetExpression func and I have %d as an operand1\n", operand1);
    while (prog [pos] == '+' || prog[pos] == '-')
    {
        char oper = prog[pos];
        //printf ("\nI am GetExpression func and I have %c as an operator\n", oper);
        pos++;
        int operand2 = GetTrouble();
        //printf ("\nI am GetExpression func and I have %d as an operand2\n", operand2);
        if (oper == '+') operand1 += operand2;
        if (oper == '-') operand1 -= operand2;
    }
    //printf ("\nI am GetExpression func and I have %d as a result\n", operand1);
    return operand1;
}


int GetTrouble ()
{
    //printf ("\nI am GetTrouble func\n");
    int operand1 = GetPrimary_exp ();
    //printf ("\nI am GetTrouble func and I have %d as an operand1\n", operand1);
    while (prog [pos] == '*' || prog[pos] == '/')
    {
        char oper = prog[pos];
        //printf ("\nI am GetTrouble func and I have %c as an operator\n", oper);
        pos++;
        int operand2 = GetPrimary_exp();
        //printf ("\nI am GetTrouble func and I have %d as an operand2\n", operand2);
        if (oper == '*') operand1 *= operand2;
        if (oper == '/') operand1 = operand1 / operand2;
    }
    //printf ("\nI am GetExpression func and I have %d as a result\n", operand1);
    return operand1;
}


int GetPrimary_exp ()
{
    //printf ("\nI am GetPrimary_exp func\n");
    int res = 0;
    if (prog[pos] == '(')
    {
        //printf ("\nI am GetPrimary_exp func and I have a bracket expression\n");
        pos++;
        res = GetExpression ();
        //printf ("\nI am GetPrimary_exp func and I have %d as a result\n", res);
        if (prog[pos] != ')') Error (BRACKETS);
        pos++;
        return res;
    }
    else
    {
        res = GetNum ();
        //printf ("\nI am GetPrimary_exp func and I have %d as a result\n", res);
        return res;
    }
}


int GetNum ()
{
    //printf ("\nI am GetNum func\n");
    int number = 0;
    if (prog[pos] == '\0') Error (EMP_NUM);
    while (prog[pos] >= '0' && prog[pos] <= '9')
    {
        number = number * 10 + prog[pos] - '0';
        pos++;
    }
    //printf ("\nI am GetNum func and I read %d\n", number);
    return number;
}
