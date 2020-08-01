#include "compiler_inc.h"




enum COMP_ERRORS_t
{
	example
};




int Comp_Error (COMP_ERRORS_t errnum)
{
    printf ("\nCompiler error (compiler) %d\n", errnum);
    assert (0);
    return 0;
}






int main()
{
    int prog = open ("my_program.nev.txt", O_RDONLY, 0);

    List* token_list = Lex_reader (prog);

    List_Dump (token_list);

    Node_t* token_tree = Tree_maker (token_list);

    Optimize (token_tree);

    Tree_Light_Dump (token_tree);

    List_Dtor (token_list);
    Tree_Dtor (token_tree);
    return 0;
}
