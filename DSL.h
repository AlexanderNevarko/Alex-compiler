#if defined TREE_DSL
    #define EAT_NEXT(where, type) where = Node_Ctor ((*cur_token) -> data, type); \
                                  *cur_token = (*cur_token) -> next;

    #define MOVE *cur_token = (*cur_token) -> next

    #define CHECK_END if ((*cur_token) -> type == END) return Tree_Error (WRONG_END, *cur_token)

    #define MAKE_DIV Node_t* divider = Node_Ctor (";", DIVIDER);

    #define DATA_CHECK(word) strcmp ((*cur_token) -> data, #word) == 0

    #define DATA_NCHECK(word) strcmp ((*cur_token) -> data, #word) != 0

    #define TYPE_CHECK(what) (*cur_token) -> type == what

    #define TYPE_NCHECK(what) (*cur_token) -> type != what

    #define RET_ERR(which) return Tree_Error (which, *cur_token)

#else
    #define EAT_NEXT
    #define MOVE
    #define CHECK_END
    #define MAKE_DIV
    #define DATA_CHECK
    #define DATA_NCHECK
    #define TYPE_CHECK
    #define TYPE_NCHECK
    #define RET_ERR

#endif
