#if defined SUPER_DUMP
    #define PRINT(what) printf ("\n"); printf (#what); printf ("\n");
#else 
    #define PRINT(what)
#endif