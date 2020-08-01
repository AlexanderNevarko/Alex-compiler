#include "tree_func.h"

#define TREE_DSL
#include "DSL.h"

#define SUPER_DUMP
#include "DEBUG.h"



const char* PI = "3.14159";
const char* EXP = "2.71828";




enum TREE_ERRORS_t
{
    WRONG_END = 0,
    WRONG_LIST = 1,
    WRONG_VAR = 2,
    WRONG_NUM = 3,
    NO_CL_BR = 4,
    WRONG_PRIME = 5,
    WRONG_ASSIGN = 6,
    WRONG_LANGUAGE = 7,
    WRONG_ARG = 8,
    WRONG_PROG_BG = 9,
    NO_FINISH = 10,
    NO_DIV = 11,
    WRONG_COMPARISON_SYMBOL = 12,
    WRONG_COMP = 13,
    WRONG_DECL = 14,
    WRONG_FN_NAME = 15,
    NO_FUNC_END = 16,
    WRONG_V_NAME = 17,
    WRONG_CALL = 18,
    WRONG_FUNC = 19,
    WRONG_EXPR = 20,
    WRONG_TERM = 21,
    NO_ARG = 22,
    WRONG_RET = 23,
    WRONG_INPUT = 24,
    WRONG_INPUT_ARG = 25,
    WRONG_OUTPUT = 26,
    WRONG_OUTPUT_ARG = 27
};


Node_t* Tree_Error (TREE_ERRORS_t errnum, List_Elem* cur_token)
{
    printf ("\nSyntax error (tree) %d\n", errnum);
    Elem_Dump (cur_token);
    assert (0);
    return NULL;
}




Node_t* Tree_maker      (List* tokens_list);
Node_t* Take_Func       (List_Elem** cur_token);
Node_t* Take_Body       (List_Elem** cur_token);
Node_t* Take_Op         (List_Elem** cur_token);
//_______________________________________________________________________________________________________________________
Node_t* Take_Assign     (List_Elem** cur_token);
Node_t* Take_Var        (List_Elem** cur_token);
Node_t* Take_Expr       (List_Elem** cur_token);
Node_t* Take_Term       (List_Elem** cur_token);
Node_t* Take_Primary    (List_Elem** cur_token);
Node_t* Take_Num        (List_Elem** cur_token);
//_______________________________________________________________________________________________________________________
Node_t* Take_Sin        (List_Elem** cur_token);
Node_t* Take_Cos        (List_Elem** cur_token);
Node_t* Take_Tan        (List_Elem** cur_token);
Node_t* Take_Ln         (List_Elem** cur_token);
Node_t* Take_Sqrt       (List_Elem** cur_token);
Node_t* Take_In         (List_Elem** cur_token);
Node_t* Take_Out        (List_Elem** cur_token);
//_______________________________________________________________________________________________________________________
Node_t* Take_If         (List_Elem** cur_token);
Node_t* Take_While      (List_Elem** cur_token);
Node_t* Take_Call       (List_Elem** cur_token);
Node_t* Take_Ret        (List_Elem** cur_token);
Node_t* Take_Decl       (List_Elem** cur_token);
Node_t* Take_Compare    (List_Elem** cur_token);




Node_t* Tree_maker (List* token_list)
{
    PRINT (Tree_maker)
    List_Elem* cur_token = token_list -> N_elem -> next;

    if (strcmp (cur_token -> data, "start") != 0) return Tree_Error (WRONG_PROG_BG, cur_token);

    Node_t* token_tree = NULL;
    Node_t* cur_func = NULL;
    token_tree = Node_Ctor (cur_token -> data, START);
    cur_token = cur_token -> next;
    cur_func = token_tree;

    while (strcmp (cur_token -> data, "finish") != 0 && cur_token != token_list -> N_elem)
    {
        Add_Right (cur_func, Take_Func (&cur_token));
        cur_func = cur_func -> right;
    }

    if (strcmp (cur_token -> data, "finish") == 0)
    {
        Add_Right (cur_func, Node_Ctor ("finish", TERMINATOR));
        return token_tree;
    }
    else if (cur_token == token_list -> N_elem)
    {
        Tree_Error (NO_FINISH, cur_token);
        return token_tree;
    }
    else return Tree_Error (WRONG_LIST, cur_token);
}




Node_t* Take_Func (List_Elem** cur_token)
{
    PRINT (Take_Func)
    CHECK_END;

    Node_t* func = NULL;
    Node_t* name = NULL;
    Node_t* arg = NULL;
    Node_t* body = NULL;

    EAT_NEXT (func, DIVIDER)
    if (TYPE_NCHECK (ID)) RET_ERR (WRONG_FN_NAME);
    EAT_NEXT (name, FUNC_T)
    Add_Left (func, name);

    if (TYPE_NCHECK (OPEN_BRACK)) RET_ERR (NO_ARG);
    MOVE;
    if (TYPE_NCHECK (CLOSE_BRACK))
    {
        arg = Take_Var (cur_token);
        Add_Right (name, arg);
    }
    while (TYPE_NCHECK (CLOSE_BRACK))
    {
        if (TYPE_NCHECK (PUNCT)) RET_ERR (WRONG_ARG);
        MOVE;
        Add_Right (arg, Take_Var (cur_token));
        arg = arg -> right;
    }
    MOVE;

    if (TYPE_CHECK (OPEN_BLOCK))
    {
        MOVE;
        body = Take_Body (cur_token);
        if (TYPE_CHECK (CLOSE_BLOCK)) MOVE;
        else RET_ERR (NO_FUNC_END);
    }
    else RET_ERR (WRONG_FUNC);
    Add_Left (name, body);

    return func;
}





Node_t* Take_Body (List_Elem** cur_token)
{
    PRINT (Take_Body)
    CHECK_END;

    Node_t* cur_op = NULL;
    Node_t* body = NULL;

    cur_op = Take_Op (cur_token);
    body = cur_op;
    while (TYPE_NCHECK (CLOSE_BLOCK))
    {
        Add_Right (cur_op, Take_Op (cur_token));
        cur_op = cur_op -> right;
    }

    return body;
}




Node_t* Take_Op (List_Elem** cur_token)
{
    PRINT (Take_Op)
    CHECK_END;

    if (TYPE_CHECK (ID))
    {
        #define KEYS
        #include "key_def.h"

        IFIF;
        else IFWHILE;
        else IFDECL;
        else IFCALL;
        else IFRET;
        else IFIN;
        else IFOUT;
        else return Take_Assign (cur_token);

        #undef KEYS
    }
    else RET_ERR (WRONG_LANGUAGE);
}


//_______________________________________________________________________________________________________________________


Node_t* Take_If (List_Elem** cur_token)
{
    PRINT (Take_If)
    CHECK_END;

    MAKE_DIV;
    Node_t* _if = NULL;
    Node_t* compare = NULL;
    Node_t* if_else = NULL;
    Node_t* if_true = NULL;
    Node_t* if_false = NULL;

    EAT_NEXT (_if, IF_T)
    Add_Left (divider, _if);
    if_else = Node_Ctor ("if_else", ELSE_T);

    if (TYPE_CHECK (OPEN_BRACK))
    {
        MOVE;
        compare = Take_Compare (cur_token);

        if (TYPE_CHECK (CLOSE_BRACK)) MOVE;
        else RET_ERR (NO_CL_BR);
    }
    else RET_ERR (WRONG_COMP);

    Add_Left (_if, compare);

    if (TYPE_CHECK (OPEN_BLOCK))
    {
        MOVE;
        if_true = Take_Op (cur_token);
        Add_Left (if_else, if_true);
        while (TYPE_NCHECK (CLOSE_BLOCK))
        {
            Add_Right (if_true, Take_Op (cur_token));
            if_true = if_true -> right;
        }
        MOVE;
    }
    else
    {
        if_true = Take_Op (cur_token);
        Add_Left (if_else, if_true);
    }

    if (DATA_CHECK (else))
    {
        MOVE;
        if (TYPE_CHECK (OPEN_BLOCK))
        {
            MOVE;
            if_false = Take_Op (cur_token);
            Add_Right (if_else, if_false);

            while (TYPE_NCHECK (CLOSE_BLOCK))
            {
                Add_Right (if_false, Take_Op (cur_token));
                if_false = if_false -> right;
            }
            MOVE;
        }
        else
        {
            if_false = Take_Op (cur_token);
            Add_Right (if_else, if_false);
        }
    }

    Add_Right (_if, if_else);

    return divider;
}




Node_t* Take_While (List_Elem** cur_token)
{
    PRINT (Take_While)
    CHECK_END;

    MAKE_DIV;
    Node_t* _while = NULL;
    Node_t* compare = NULL;
    Node_t* cur_op = NULL;

    EAT_NEXT (_while, WHILE_T)
    Add_Left (divider, _while);

    if (TYPE_CHECK (OPEN_BRACK))
    {
        MOVE;
        compare = Take_Compare (cur_token);

        if (TYPE_CHECK (CLOSE_BRACK)) MOVE;
        else RET_ERR (NO_CL_BR);
    }
    Add_Left (_while, compare);

    if (TYPE_CHECK (OPEN_BLOCK))
    {
        MOVE;
        cur_op = Take_Op (cur_token);
        Add_Right (_while, cur_op);
        while (TYPE_NCHECK (CLOSE_BLOCK))
        {
            Add_Right (cur_op, Take_Op (cur_token));
            cur_op = cur_op -> right;
        }
        MOVE;
    }
    else
    {
        cur_op = Take_Op (cur_token);
        Add_Right (_while, cur_op);
    }

    return divider;
}




Node_t* Take_Call (List_Elem** cur_token)
{
    PRINT (Take_Call)
    CHECK_END;

    MAKE_DIV;
    Node_t* name = NULL;
    Node_t* arg = NULL;

    if (DATA_NCHECK (call)) RET_ERR (WRONG_CALL);
    MOVE;
    EAT_NEXT (name, CALL)
    Add_Left (divider, name);
    if (TYPE_NCHECK (OPEN_BRACK)) RET_ERR (WRONG_ARG);
    MOVE;
    if (TYPE_NCHECK (CLOSE_BRACK))
    {
        arg = Take_Var (cur_token);
        Add_Right (name, arg);
    }
    while (TYPE_NCHECK (CLOSE_BRACK))
    {
        if (TYPE_NCHECK (PUNCT)) RET_ERR (WRONG_ARG);
        MOVE;
        Add_Right (arg, Take_Var (cur_token));
        arg = arg -> right;
    }
    MOVE;
    if (TYPE_NCHECK (DIV)) RET_ERR (NO_DIV);
    MOVE;

    return divider;
}




Node_t* Take_Ret (List_Elem** cur_token)
{
    PRINT (Take_Ret)
    CHECK_END;

    MAKE_DIV;
    Node_t* ret = NULL;
    Node_t* arg = NULL;

    if (DATA_NCHECK (return)) RET_ERR (WRONG_RET);
    EAT_NEXT (ret, RETURN_T);
    arg = Take_Expr (cur_token);
    Add_Left (divider, ret);
    Add_Right (ret, arg);
    if (TYPE_NCHECK (DIV)) RET_ERR (NO_DIV);
    MOVE;

    return divider;
}




Node_t* Take_Decl (List_Elem** cur_token)
{
    PRINT (Take_Decl)
    CHECK_END;

    MAKE_DIV;
    Node_t* decl = NULL;
    Node_t* assign = NULL;
    Node_t* var = NULL;
    Node_t* expr = NULL;

    EAT_NEXT (decl, DECL_T)
    Node_Dump (decl);
    if (TYPE_NCHECK (ID)) RET_ERR (WRONG_V_NAME);
    var = Take_Var (cur_token);

    Node_Dump (decl);
    Elem_Dump (*cur_token);
    if (DATA_NCHECK (=)) RET_ERR (WRONG_DECL);
	EAT_NEXT (assign, ASSIGN)
	Add_Left (assign, var);
	Add_Left (decl, assign);

    expr = Take_Expr (cur_token);
    if (TYPE_NCHECK (DIV)) RET_ERR (NO_DIV);
    MOVE;
    Add_Right (assign, expr);
    Add_Left (divider, decl);

    return divider;
}




Node_t* Take_Compare (List_Elem** cur_token)
{
    PRINT (Take_Compare)
    CHECK_END;

    Node_t* expr_left = NULL;
    Node_t* comp = NULL;
    Node_t* expr_right = NULL;

    expr_left = Take_Expr (cur_token);
    if (DATA_CHECK (>) || DATA_CHECK (<) || DATA_CHECK (==) ||
        DATA_CHECK (>=) || DATA_CHECK (<=) || DATA_CHECK (!=))
    {
        EAT_NEXT (comp, COMPARISON)
    }
    else RET_ERR (WRONG_COMPARISON_SYMBOL);
    expr_right = Take_Expr (cur_token);

    Add_Left (comp, expr_left);
    Add_Right (comp, expr_right);

    return comp;
}


//_______________________________________________________________________________________________________________________


Node_t* Take_Assign (List_Elem** cur_token)
{
    PRINT (Take_Assign)
    CHECK_END;

    Node_t* var = NULL;
    Node_t* assign = NULL;
    Node_t* expr = NULL;
    Node_t* divider = NULL;

    var = Take_Var (cur_token);
    if (DATA_CHECK (=) || DATA_CHECK (+=) || DATA_CHECK (-=) || DATA_CHECK (*=) || DATA_CHECK (/=) || DATA_CHECK (^=))
    {
        EAT_NEXT (assign, ASSIGN)
        expr = Take_Expr (cur_token);
        Add_Left (assign, var);
        Add_Right (assign, expr);
    }
    else RET_ERR (WRONG_ASSIGN);

    if (TYPE_CHECK (DIV))
    {
        MAKE_DIV;
        MOVE;
        Add_Left (divider, assign);
        return divider;
    }
    else RET_ERR (NO_DIV);

}




Node_t* Take_Expr (List_Elem** cur_token)
{
    PRINT (Take_Expr)
    CHECK_END;

    Node_t* operand_left = NULL;
    Node_t* op_tor = NULL;
    Node_t* operand_right = NULL;

    operand_left = Take_Term (cur_token);
    if (DATA_CHECK (+) || DATA_CHECK (-))
    {
        EAT_NEXT (op_tor, OPERATOR)
        Add_Left (op_tor, operand_left);
        operand_right = Take_Expr (cur_token);
        Add_Right (op_tor, operand_right);
    }

    return op_tor ? (operand_right ? op_tor : Tree_Error (WRONG_EXPR, *cur_token)) : operand_left;
}




Node_t* Take_Term (List_Elem** cur_token)
{
    PRINT (Take_Term)
    CHECK_END;

    Node_t* operand_left = NULL;
    Node_t* op_tor = NULL;
    Node_t* operand_right = NULL;

    operand_left = Take_Primary (cur_token);
    if (DATA_CHECK(*) || DATA_CHECK(/) || DATA_CHECK(^))
    {
        EAT_NEXT (op_tor, OPERATOR)
        Add_Left (op_tor, operand_left);
        operand_right = Take_Term (cur_token);
        Add_Right (op_tor, operand_right);
    }

    return op_tor ? (operand_right ? op_tor : Tree_Error (WRONG_TERM, *cur_token)) : operand_left;
}




Node_t* Take_Primary (List_Elem** cur_token)
{
    PRINT (Take_Primary)
    CHECK_END;

    Node_t* node = NULL;

    if (TYPE_CHECK (OPEN_BRACK))
    {
        MOVE;
        node = Take_Expr (cur_token);
        if (TYPE_CHECK (CLOSE_BRACK))
        {
            MOVE;
            return node;
        }
        else RET_ERR (NO_CL_BR);
    }
    else if (TYPE_CHECK (NUM))
    {
        node = Take_Num (cur_token);
        return node;
    }
    else if (TYPE_CHECK (ID))
    {

        if (DATA_CHECK (SIN))
        {
            node = Take_Sin (cur_token);
            return node;
        }

        else if (DATA_CHECK(COS))
        {
            node = Take_Cos (cur_token);
            return node;
        }
        else if (DATA_CHECK(TAN))
        {
            node = Take_Tan (cur_token);
            return node;
        }
        else if (DATA_CHECK(LN))
        {
            node = Take_Ln (cur_token);
            return node;
        }
        else if (DATA_CHECK(SQRT))
        {
            node = Take_Sqrt (cur_token);
            return node;
        }
        else
        {
            node = Take_Var (cur_token);
            return node;
        }

    }

    else RET_ERR (WRONG_PRIME);
}




Node_t* Take_Var (List_Elem** cur_token)
{
    PRINT (Take_Var)
    CHECK_END;

    Node_t* node = NULL;

    if (TYPE_CHECK (ID))
    {
        if (DATA_CHECK (EXP))
        {
            node = Node_Ctor (EXP, NUMBER);
            MOVE;
        }
        else if (DATA_CHECK (PI))
        {
            node = Node_Ctor (PI, NUMBER);
            MOVE;
        }
        else
        {
            EAT_NEXT (node, VARIABLE)
        }
    }
    else RET_ERR (WRONG_VAR);

    return node;
}




Node_t* Take_Num (List_Elem** cur_token)
{
    PRINT (Take_Num )
    CHECK_END;

    Node_t* node = NULL;

    if (TYPE_CHECK (NUM))
    {
        EAT_NEXT (node, NUMBER)
    }
    else RET_ERR (WRONG_NUM);

    return node;
}


//_______________________________________________________________________________________________________________________


Node_t* Take_Sin (List_Elem** cur_token)
{
    PRINT (Take_Sin)
    CHECK_END;

    Node_t* func = NULL;
    Node_t* arg = NULL;

    if (TYPE_CHECK (ID))
    {
        EAT_NEXT (func, SIN_T)
        if (TYPE_CHECK (OPEN_BRACK))
        {
            MOVE;
            arg = Take_Expr (cur_token);
        }
        else RET_ERR (WRONG_ARG);

        if (TYPE_CHECK (CLOSE_BRACK))
        {
            MOVE;
            Add_Right (func, arg);
            return func;
        }
        else RET_ERR (NO_CL_BR);
    }
}





Node_t* Take_Cos (List_Elem** cur_token)
{
    PRINT (Take_Cos)
    CHECK_END;

    Node_t* func = NULL;
    Node_t* arg = NULL;

    if (TYPE_CHECK (ID))
    {
        EAT_NEXT (func, COS_T)
        if (TYPE_CHECK (OPEN_BRACK))
        {
            MOVE;
            arg = Take_Expr (cur_token);
        }
        else RET_ERR (WRONG_ARG);

        if (TYPE_CHECK (CLOSE_BRACK))
        {
            MOVE;
            Add_Right (func, arg);
            return func;
        }
        else RET_ERR (NO_CL_BR);
    }
}




Node_t* Take_Tan (List_Elem** cur_token)
{
    PRINT (Take_Tan)
    CHECK_END;

    Node_t* func = NULL;
    Node_t* arg = NULL;

    if (TYPE_CHECK (ID))
    {
        EAT_NEXT (func, TAN_T)
        if (TYPE_CHECK (OPEN_BRACK))
        {
            MOVE;
            arg = Take_Expr (cur_token);
        }
        else RET_ERR (WRONG_ARG);

        if (TYPE_CHECK (CLOSE_BRACK))
        {
            MOVE;
            Add_Right (func, arg);
            return func;
        }
        else RET_ERR (NO_CL_BR);
    }
}




Node_t* Take_Ln (List_Elem** cur_token)
{
    PRINT (Take_Ln)
    CHECK_END;

    Node_t* func = NULL;
    Node_t* arg = NULL;

    if (TYPE_CHECK (ID))
    {
        EAT_NEXT (func, LN_T)
        if (TYPE_CHECK (OPEN_BRACK))
        {
            MOVE;
            arg = Take_Expr (cur_token);
        }
        else RET_ERR (WRONG_ARG);

        if (TYPE_CHECK (CLOSE_BRACK))
        {
            MOVE;
            Add_Right (func, arg);
            return func;
        }
        else RET_ERR (NO_CL_BR);
    }
}




Node_t* Take_Sqrt (List_Elem** cur_token)
{
    PRINT (Take_Sqrt)
	CHECK_END;

    Node_t* func = NULL;
    Node_t* arg = NULL;

    if (TYPE_CHECK (ID))
    {
        EAT_NEXT (func, SQRT_T)
        if (TYPE_CHECK (OPEN_BRACK))
        {
            MOVE;
            arg = Take_Expr (cur_token);
        }
        else RET_ERR (WRONG_ARG);

        if (TYPE_CHECK (CLOSE_BRACK))
        {
            MOVE;
            Add_Right (func, arg);
            return func;
        }
        else RET_ERR (NO_CL_BR);
    }
}




Node_t* Take_In (List_Elem** cur_token)
{
    CHECK_END;
    if (DATA_NCHECK (input)) RET_ERR (WRONG_INPUT);   

    MAKE_DIV;
    Node_t* input = NULL;
    Node_t* arg = NULL;

    EAT_NEXT (input, INPUT_T)
    if (TYPE_NCHECK (OPEN_BRACK)) RET_ERR (WRONG_INPUT_ARG);  
    MOVE;
    EAT_NEXT (arg, VARIABLE);
    if (TYPE_NCHECK (CLOSE_BRACK)) RET_ERR (WRONG_INPUT_ARG);
    MOVE;
    if (TYPE_NCHECK (DIV)) RET_ERR (NO_DIV);
    MOVE;
    Add_Left (divider, input);
    Add_Right (input, arg);

    return divider;
}




Node_t* Take_Out (List_Elem** cur_token)
{
    CHECK_END;
    if (DATA_NCHECK (output)) RET_ERR (WRONG_OUTPUT);   

    MAKE_DIV;
    Node_t* output = NULL;
    Node_t* arg = NULL;

    EAT_NEXT (output, OUTPUT_T)
    if (TYPE_NCHECK (OPEN_BRACK)) RET_ERR (WRONG_OUTPUT_ARG);  
    MOVE;
    EAT_NEXT (arg, VARIABLE);
    if (TYPE_NCHECK (CLOSE_BRACK)) RET_ERR (WRONG_OUTPUT_ARG);
    MOVE;
    if (TYPE_NCHECK (DIV)) RET_ERR (NO_DIV);
    MOVE;
    Add_Left (divider, output);
    Add_Right (output, arg);

    return divider;
}




#undef SUPER_DUMP
#undef TREE_DSL
