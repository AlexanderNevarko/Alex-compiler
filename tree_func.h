#include <ctype.h>
#include "tree_ver.h"

#define NODE
#include "tree_def.h"



const int LT = 228;
const int RT = 1488;
const int MAXLENGTH = 100;





bool Add_Left (Node_t* parent, Node_t* node);
bool Add_Right (Node_t* parent, Node_t* node);
bool Delete_Node (Node_t* parent, int side);
Node_t* Node_copy (Node_t* node);
Node_t* Tree_copy (Node_t* root);

Node_t* Tree_Save (FILE* save, Node_t* root);
Node_t* Tree_Load (FILE* file);
int Lex_read (FILE* file, char* word, int length);
char Skip_spaces (FILE* file);



bool Add_Left (Node_t* parent, Node_t* node)
{
    PL = node;

    if (node)
        PAR = parent;

    return Node_OK (parent) && Node_OK (node);
}




bool Add_Right (Node_t* parent, Node_t* node)
{
    PR = node;

    if (node)
        PAR = parent;

    return Node_OK (parent) && Node_OK (node);
}




bool Delete_Node (Node_t* parent, int side)
{
    assert (parent);
    
    if (side == LT)
    {
        Node_Dtor (PL);
        PL = NULL;
        return Node_OK (parent);
    }

    if (side == RT)
    {
        Node_Dtor (PR);
        PR = NULL;
        return Node_OK (parent);
    }

    else return NULL;
}




Node_t* Node_copy (Node_t* node)
{
    if (node == NULL) return NULL;

    Node_t* copy = Node_Ctor (DATA, TYPE);
    
    copy -> left = LEFT;
    copy -> right = RIGHT;
    copy -> parent = PAR;

    return copy;
}




Node_t* Tree_copy (Node_t* root)
{
    Node_t* root_cpy = Node_copy (root);

    if (root -> left)
    {
        Add_Left (root_cpy, Tree_copy (root -> left));
    }
    if (root -> right)
    {
        Add_Right (root_cpy, Tree_copy (root -> right));
    }
    
    return root_cpy;
}




Node_t* Tree_Save (FILE* save, Node_t* node)
{   
    if (node == NULL)
    {
        fprintf (save, "nil ");
        return node;
    }
    
    fprintf (save, "( ");
    
    Tree_Save (save, LEFT);
    fprintf (save, "\"%s\" ", DATA);
    Tree_Save (save, RIGHT);

    fprintf (save, ") ");

    return node;
}




Node_t* Tree_Load (FILE* file)
{
    #define LEXIS
    #include "tree_def.h"

    Node_t* root = NULL;
    Node_t* left = NULL;
    Node_t* right = NULL;

    char buf[MAXLENGTH] = "";
    
    int lex = Lex_read (file, buf, MAXLENGTH);

    if (strcmp (buf, "nil") == 0) return NULL;

    if (lex == OPEN_BRACKET)
    {   
        left = Tree_Load (file);
        
        lex = Lex_read (file, buf, MAXLENGTH);
        if (lex == TEXT || lex == OPERATOR || lex == NUMBER)
        {
            root = Tree_Ctor (buf, lex);
        }

        right = Tree_Load (file);

        lex = Lex_read (file, buf, MAXLENGTH);
        assert (lex == CLOSE_BRACKET && "ERROR in load file: no closing bracket for node!!!");
    }

    Add_Left (root, left);
    Add_Right (root, right);

    #undef LEXIS
    return root;
}




int Lex_read (FILE* file, char* word, int length)
{
	#define LEXIS
    #include "tree_def.h"
    
    memset (word, '\0', length);
    char* word_ptr = word;

    char c = Skip_spaces (file);

    if (c == '\0')
    {
        return EMPTY;
    }
    else if (c == EOF)
    {
        return THE_END;
    }
    else if (c == '(')
    {
        *word = c;
        return OPEN_BRACKET;
    }
    else if (c == ')')
    {
        *word = c;
        return CLOSE_BRACKET;
    }
    else if (c == '"')
    {
        int i = 0; // numbers counter
        int c = 0; // chars counter
        int op = 0; // operarors counter
        int sp = 0; // spaces counter
        int e = 0; // counter for everything else

        *word_ptr = fgetc (file);
        while (*word_ptr != '"')
        {
            if (isalpha (*word_ptr)) c++;
            else if (isdigit (*word_ptr)) i++;
            else if (*word_ptr == '+' || *word_ptr == '-' || *word_ptr == '*' || *word_ptr == '/' || *word_ptr == '^') op++;
            else if (isspace (*word_ptr)) sp++;
            else e++;

            word_ptr++;
            *word_ptr = fgetc (file);
        }
        *word_ptr = '\0';
        return (c || sp || e) ? TEXT : ((op && (i == 0)) ? OPERATOR : ((i && (op == 0)) ? NUMBER : TEXT));
    }
    else
    {
        *word_ptr = c;
        while (not isspace (*word_ptr))
        {
            word_ptr++;
            *word_ptr = fgetc (file);
        }
        *word_ptr = '\0';
        return ELSE;
    }

	#undef LEXIS
}




char Skip_spaces (FILE* file)
{
    char c = fgetc (file);
    while (isspace (c))
        c = fgetc (file);

    return c;
}

#undef NODE
