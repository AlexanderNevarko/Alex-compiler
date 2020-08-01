#if defined (NODE)
    #define PAR node -> parent
    #define LEFT node -> left
    #define RIGHT node -> right
    #define DATA node -> data
    #define TYPE node -> type
    #define PL parent -> left
    #define PR parent -> right

#else
    #define PAR
    #define LEFT
    #define RIGHT
    #define DATA
    #define TYPE
    #define PL
    #define PR

#endif

#if defined (LEXIS)
    #define THE_END 100498
    #define EMPTY 100499
    #define OPEN_BRACKET 100500
    #define CLOSE_BRACKET 100501
    #define QUOTE 100502
    #define ELSE 100504

#else
    #define OPEN_BRACKET
    #define CLOSE_BRACKET
    #define QUOTE
    #define STRING

#endif


