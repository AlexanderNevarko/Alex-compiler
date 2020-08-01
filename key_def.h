#if defined (KEYS)
    #define IFIF if (strcmp ("if", (*cur_token) -> data) == 0) return Take_If (cur_token)
    #define IFWHILE if (strcmp ("while", (*cur_token) -> data) == 0) return Take_While (cur_token)
    #define IFCALL if (strcmp ("call", (*cur_token) -> data) == 0) return Take_Call (cur_token)
    #define IFRET if (strcmp ("return", (*cur_token) -> data) == 0) return Take_Ret (cur_token)  
    #define IFDECL if (strcmp ("var", (*cur_token) -> data) == 0) return Take_Decl (cur_token)
    #define IFIN if (strcmp ("input", (*cur_token) -> data) == 0) return Take_In (cur_token)
    #define IFOUT if (strcmp ("output", (*cur_token) -> data) == 0) return Take_Out (cur_token)

#else
    #define IFIF
    #define IFWHILE
    #define IFCALL
    #define IFRET
    #define IFDECL
    #define IFIN
    #define IFOUT

#endif