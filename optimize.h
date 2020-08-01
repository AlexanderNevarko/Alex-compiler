#include <math.h>


const int MAX_DOUBLE = 30;


enum DIFF_ERRORS_t
{
    WRONG_DIFF_EXPR = 0,
    WRONG_DIFF_OPERATOR = 1,
    ERROR_IN_SWITCH = 2,
    WRONG_TEX_OPERATOR = 3,
    TEX_DEFAULT_ERROR = 4,
    DIV_ZERO = 5
};




Node_t* Diff_Error (DIFF_ERRORS_t errnum)
{
    printf ("\nDiff error (diff) %d\n", errnum);
    assert (0);
    return NULL;
}


Node_t* Optimize            (Node_t* expr);
Node_t* Const_optimize      (Node_t* expr, int* count);
Node_t* One_Zero_optimize   (Node_t* expr, int* count);




Node_t* Optimize (Node_t* expr)
{   
    int count = 0;
    int count_prev = count;

    //printf ("[%0X]\ncount = %d\n", expr, count);

    printf ("here6\n");
    Const_optimize (expr, &count);
    printf ("first const optimization\n");
    One_Zero_optimize (expr, &count);
    printf ("second one_zero optimization\n");

    while (count_prev != count)
    {
        count_prev = count;
        Const_optimize (expr, &count);
        One_Zero_optimize (expr, &count);
    }
    
    return expr;
}




Node_t* Const_optimize (Node_t* expr, int* count)
{
    printf ("[%0X]C\ncount = %d\n", expr, *count);
    
    if (expr == NULL) return expr;

    if (expr -> right == NULL) return Const_optimize (expr -> left, count);
    if (expr -> left == NULL) return Const_optimize (expr -> right, count);
    
    if (expr -> type == OPERATOR)
    {
        if (expr -> left -> type == NUMBER && expr -> right -> type == NUMBER)
        {
            if (strcmp (expr -> data, "+") == 0)
            {
                (*count)++;
                double res = atof (expr -> left -> data) + atof (expr -> right -> data);

                free (expr -> data);
                expr -> data = (char*) calloc (MAX_DOUBLE, sizeof (*(expr -> data)));
                expr -> type = NUMBER;
                Delete_Node (expr, LT);
                Delete_Node (expr, RT);
                
                sprintf (expr -> data, "%lg", res);
                return expr;
            }
            else if (strcmp (expr -> data, "-") == 0)
            {
                (*count)++;
                double res = atof (expr -> left -> data) - atof (expr -> right -> data);

                free (expr -> data);
                expr -> data = (char*) calloc (MAX_DOUBLE, sizeof (*(expr -> data)));
                expr -> type = NUMBER;
                Delete_Node (expr, LT);
                Delete_Node (expr, RT);
                
                sprintf (expr -> data, "%lg", res);
                return expr;
            }
            else if (strcmp (expr -> data, "*") == 0)
            {
                (*count)++;
                double res = atof (expr -> left -> data) * atof (expr -> right -> data);

                free (expr -> data);
                expr -> data = (char*) calloc (MAX_DOUBLE, sizeof (*(expr -> data)));
                expr -> type = NUMBER;
                Delete_Node (expr, LT);
                Delete_Node (expr, RT);
                
                sprintf (expr -> data, "%lg", res);
                return expr;
            }
            else if (strcmp (expr -> data, "/") == 0)
            {
                (*count)++;
                double res = atof (expr -> left -> data) / atof (expr -> right -> data);

                free (expr -> data);
                expr -> data = (char*) calloc (MAX_DOUBLE, sizeof (*(expr -> data)));
                expr -> type = NUMBER;
                Delete_Node (expr, LT);
                Delete_Node (expr, RT);
                
                sprintf (expr -> data, "%lg", res);
                return expr;
            }
            else if (strcmp (expr -> data, "^") == 0)
            {
                (*count)++;
                double res = pow (atof (expr -> left -> data), atof (expr -> right -> data));

                free (expr -> data);
                expr -> data = (char*) calloc (MAX_DOUBLE, sizeof (*(expr -> data)));
                expr -> type = NUMBER;
                Delete_Node (expr, LT);
                Delete_Node (expr, RT);
                
                sprintf (expr -> data, "%lg", res);
                return expr;
            }
            else
            {
                Const_optimize (expr -> left, count);
                Const_optimize (expr -> right, count);
            }
        }
        else
        {
            Const_optimize (expr -> left, count);
            Const_optimize (expr -> right, count);
        }
    }
    else
    {
        Const_optimize (expr -> left, count);
        Const_optimize (expr -> right, count);
        return expr;
    }
}




Node_t* One_Zero_optimize (Node_t* expr, int* count)
{
    printf ("[%0X]OZ\ncount = %d\n", expr, *count);
    
    if (expr == NULL) return expr;

    if (expr -> right == NULL) return One_Zero_optimize (expr -> left, count);
    if (expr -> left == NULL) return One_Zero_optimize (expr -> right, count);

    if (expr -> type == OPERATOR)
    {
        if (expr -> left -> type == NUMBER)
        {
            if (strcmp (expr -> left -> data, "1") == 0)
            {
                if (strcmp (expr -> data, "*") == 0)
                {
                    (*count)++;

                    Delete_Node (expr, LT);
                    
                    expr -> data = my_str_dup (expr -> right -> data, strlen (expr -> right -> data));
                    expr -> type = expr -> right -> type;
                    
                    Node_t* node = expr -> right;
                    Add_Left (expr, expr -> right -> left);
                    Add_Right (expr, expr -> right -> right);
                    Node_Dtor (node);
                    return One_Zero_optimize (expr, count);
                }
                else if (strcmp (expr -> data, "^") == 0)
                {
                    (*count)++;

                    Delete_Node (expr, LT);
                    Delete_Node (expr, RT);

                    expr -> data = my_str_dup ("1", sizeof ("1"));
                    expr -> type = NUMBER;
                    return expr;
                }
                else return One_Zero_optimize (expr -> right, count);
            }
            else if (strcmp (expr -> left -> data, "0") == 0)
            {
                if (strcmp (expr -> data, "+") == 0)
                {
                    (*count)++;
                    printf ("deleting left node\n");
                    Delete_Node (expr, LT);
                    printf ("left node deleted\n");
                    strcpy (expr -> data, expr -> right -> data);
                    printf ("data copied\n");
                    expr -> type = expr -> right -> type;
                    printf ("type copied\n");
                    Node_t* node = expr -> right;
                    Add_Left (expr, expr -> right -> left);
                    printf ("add left\n");
                    Add_Right (expr, expr -> right -> right);
                    printf ("add right\n");
                    Node_Dtor (node);
                    printf ("node destructed\n");
                    if (strcmp (expr -> parent -> data, "-") == 0)
                    {
                        free (expr -> parent -> data);
                        expr -> parent -> data = my_str_dup ("+", 2);
                    }
                    return One_Zero_optimize (expr, count);
                }
                else if (strcmp (expr -> data, "*") == 0 || strcmp (expr -> data, "/") == 0 || strcmp (expr -> data, "^") == 0)
                {
                    (*count)++;

                    Delete_Node (expr, LT);
                    Delete_Node (expr, RT);

                    expr -> data = my_str_dup ("0", sizeof ("0"));
                    expr -> type = NUMBER;
                    return expr;
                }
                else if (strcmp (expr -> data, "-") == 0)
                {
                    (*count)++;

                    if (strcmp (expr -> parent -> data, "-") == 0)
                    {
                        Node_t* par = expr -> parent;
                        Node_t* node = expr -> right;
                        Delete_Node (expr, LT);
                        Node_Dtor (expr);
                        Add_Right (par, node);
                    }
                    else
                    {
                        expr -> left -> data = my_str_dup ("-1", sizeof ("-1"));
                        expr -> data = my_str_dup ("*", sizeof ("*"));
                    }

                    return One_Zero_optimize (expr -> right, count);
                }
                else return expr;
            }
            else
            {
                Const_optimize (expr -> left, count);
                One_Zero_optimize (expr -> right, count);
                return expr;
            }
        }
        else if (expr -> right -> type == NUMBER)
        {
            if (strcmp (expr -> right -> data, "1") == 0)
            {
                if (strcmp (expr -> data, "*") == 0 || strcmp (expr -> data, "/") == 0 || strcmp (expr -> data, "^") == 0)
                {
                    (*count)++;
                    
                    Delete_Node (expr, RT);
                    
                    expr -> data = my_str_dup (expr -> left -> data, strlen (expr -> left -> data));
                    expr -> type = expr -> left -> type;
                    
                    Node_t* node = expr -> left;
                    Add_Right (expr, expr -> left -> right);
                    Add_Left (expr, expr -> left -> left);
                    Node_Dtor (node);
                    
                    return One_Zero_optimize (expr, count);
                }
                else return One_Zero_optimize (expr -> left, count);
            }
            else if (strcmp (expr -> right -> data, "0") == 0)
            {
                if (strcmp (expr -> data, "*") == 0)
                {
                    (*count)++;

                    Delete_Node (expr, LT);
                    Delete_Node (expr, RT);

                    expr -> data = my_str_dup ("0", sizeof ("0"));
                    expr -> type = NUMBER;

                    return expr;
                }
                else if (strcmp (expr -> data, "^") == 0)
                {
                    (*count)++;

                    Delete_Node (expr, LT);
                    Delete_Node (expr, RT);

                    expr -> data = my_str_dup ("1", sizeof ("1"));
                    expr -> type = NUMBER;

                    return expr;
                }
                else if (strcmp (expr -> data, "+") == 0 || strcmp (expr -> data, "-") == 0)
                {
                    (*count)++;
                    
                    Delete_Node (expr, RT);
                    
                    strcpy (expr -> data, expr -> left -> data);
                    expr -> type = expr -> left -> type;
                    
                    Node_t* node = expr -> left;
                    Add_Right (expr, expr -> left -> right);
                    Add_Left (expr, expr -> left -> left);
                    Node_Dtor (node);
                    
                    return One_Zero_optimize (expr, count);
                }
                else return Diff_Error (DIV_ZERO);
            }
            else
            {
                Const_optimize (expr -> right, count);
                One_Zero_optimize (expr -> left, count);
                return expr;
            }
        }
        else
        {
            One_Zero_optimize (expr -> right, count);
            One_Zero_optimize (expr -> left, count);
            return expr;
        }
    }
    else
    {
        One_Zero_optimize (expr -> right, count);
        One_Zero_optimize (expr -> left, count);
        return expr;
    }
}
