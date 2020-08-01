#include "node.h"

#define NODE
#include "tree_def.h"




Node_t* Tree_Ctor (const char* info, int type);
bool Tree_Dtor (Node_t* head);
bool Tree_OK (Node_t* head);
bool Tree_Dump (Node_t* head);
bool Child_Dump (FILE* dump, Node_t* node);

bool Tree_Light_Dump (Node_t* root);
bool Child_Light_Dump (FILE* dump, Node_t* node);




Node_t* Tree_Ctor (const char* info, int type)
{
    return Node_Ctor (info, type);
}




bool Tree_Dtor (Node_t* head)
{
    if (head == NULL) return true;

    Tree_Dtor (head -> right);
    Tree_Dtor (head -> left);

    Node_Dtor (head);

    return true;
}




bool Tree_OK (Node_t* head)
{
    if (head == NULL) return true;

    return Node_OK (head) &&
           Tree_OK (head -> right) &&
           Tree_OK (head -> left);
}




bool Tree_Dump (Node_t* head)
{
    FILE* dump = fopen ("tree_dump.gv", "w");
    fprintf (dump, "digraph tree\n{\n");

    Child_Dump (dump, head);

    fprintf (dump, "}\n");

    fclose (dump);

    return head ? Tree_OK (head) : true;
}




bool Child_Dump (FILE* dump, Node_t* node)
{
    if (node == NULL) return true;
    
    if (LEFT)
        fprintf (dump, "\t\"[%0X]\\nparent [%0X]\\ntype = %d\\ndata: %s\\nleft [%0X] right [%0X]\" -> "
                       "\"[%0X]\\nparent [%0X]\\ntype = %d\\ndata: %s\\nleft [%0X] right [%0X]\" [label = \"left\"]\n", 
                       node, PAR, TYPE, DATA, LEFT, RIGHT,
                       LEFT, LEFT -> parent, LEFT -> type, LEFT -> data, LEFT -> left, LEFT -> right);
    
    if (RIGHT)
        fprintf (dump, "\t\"[%0X]\\nparent [%0X]\\ntype = %d\\ndata: %s\\nleft [%0X] right [%0X]\" -> "
                       "\"[%0X]\\nparent [%0X]\\ntype = %d\\ndata: %s\\nleft [%0X] right [%0X]\" [label = \"right\"]\n",
                       node, PAR, TYPE, DATA, LEFT, RIGHT,
                       RIGHT, RIGHT -> parent, RIGHT -> type, RIGHT -> data, RIGHT -> left, RIGHT -> right);
    
    Child_Dump (dump, LEFT);
    Child_Dump (dump, RIGHT);
    
    return Tree_OK (node);
}




bool Tree_Light_Dump (Node_t* root)
{
    FILE* dump = fopen ("tree_light_dump.gv", "w");
    fprintf (dump, "digraph tree\n{\n");

    Child_Light_Dump (dump, root);

    fprintf (dump, "}\n");

    fclose (dump);

    return root ? Tree_OK (root) : true;
}




bool Child_Light_Dump (FILE* dump, Node_t* node)
{
    if (node == NULL) return true;
    
    if (LEFT)
        fprintf (dump, "\t\"%s\\n%d\\n[%0X]\" -> \"%s\\n%d\\n[%0X]\" [label = \"left\"]\n", 
                       DATA, TYPE, node, LEFT -> data, LEFT -> type, LEFT);
    
    if (RIGHT)
        fprintf (dump, "\t\"%s\\n%d\\n[%0X]\" -> \"%s\\n%d\\n[%0X]\" [label = \"right\"]\n", 
                       DATA, TYPE, node, RIGHT -> data, RIGHT -> type, RIGHT);
    
    Child_Light_Dump (dump, LEFT);
    Child_Light_Dump (dump, RIGHT);
    
    return Tree_OK (node);
}




#undef NODE
