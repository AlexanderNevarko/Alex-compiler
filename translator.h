#include "assm.h"




enum Trans_Errors_t
{

};




struct Err_info
{
    int type;
    Node_t* node;
};




struct Code_tb
{
    List* var_tb;
    List* func_tb;
};




Code_tb* Code_tb_Ctor (int var_tb_size, int func_tb_size);
Code_tb* Code_tb_Dtor (Code_tb* code_tb);
Code_tb* Code_tb_Ok   (Code_tb* code_tb);
Code_tb* Code_tb_Dump (Code_tb* code_tb);




FILE* Translator (Node_t* prog_tree);
Code_tb Checker  (Node_t* prog_tree);
FILE* To_assm    (Node_t* prog_tree, Code_tb* code_tb);




FILE* Translator (Node_t* prog_tree)
{
    Code_tb* code_tb = Checker (prog_tree);
    FILLE* assmfile = To_assm (prog_tree, code_tb);
    return assmfile;
}