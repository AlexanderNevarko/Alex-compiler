// CPU  (c)AlexN  v.2.0
//const int COM_VERSION = 2;

//DEF_CMD (name, num, cmdsize (sizeof char + sizeof arguments), func)

DEF_CMD (add,  65, 1, CPU_Add (cpu))
DEF_CMD (sub,  66, 1, CPU_Sub (cpu))
DEF_CMD (mul,  67, 1, CPU_Mul (cpu))
DEF_CMD (div,  68, 1, CPU_Div (cpu))
DEF_CMD (in,   69, 1, CPU_In  (cpu))
DEF_CMD (out,  70, 1, CPU_Out (cpu))
DEF_CMD (sin,  71, 1, CPU_Sin (cpu))
DEF_CMD (cos,  72, 1, CPU_Cos (cpu))
DEF_CMD (tan,  73, 1, CPU_Tan (cpu))
DEF_CMD (sqrt, 74, 1, CPU_Sqrt (cpu))

DEF_CMD (push,  75, (sizeof(char) + sizeof(double)), CPU_Push(cpu, *(double*)((prog + cpu -> pos) + sizeof (char))))
DEF_CMD (pop,   76, 1, CPU_Pop(cpu))
DEF_CMD (pushr, 77, (sizeof(char) + sizeof(char)), CPU_Pushr (cpu, *((prog + cpu -> pos) + sizeof (char))))
DEF_CMD (popr,  78, (sizeof(char) + sizeof(char)), CPU_Popr  (cpu, *((prog + cpu -> pos) + sizeof (char))))

DEF_CMD (mark,   79, (sizeof (char) + sizeof (int)) , CPU_Mark (cpu))
DEF_CMD (jmp, 80, (sizeof (char) + sizeof (int)), CPU_Jmp (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (je,  81, (sizeof (char) + sizeof (int)), CPU_Je  (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (jne, 82, (sizeof (char) + sizeof (int)), CPU_Jne (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (ja,  83, (sizeof (char) + sizeof (int)), CPU_Ja  (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (jb,  84, (sizeof (char) + sizeof (int)), CPU_Jb  (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (jae, 85, (sizeof (char) + sizeof (int)), CPU_Jae (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (jbe, 86, (sizeof (char) + sizeof (int)), CPU_Jbe (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (jm,  87, (sizeof (char) + sizeof (int)), CPU_Jm  (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))

DEF_CMD (call, 90, (sizeof (char) + sizeof (int)), CPU_Call (cpu, *(int*)(prog + cpu -> pos + sizeof (char))))
DEF_CMD (ret, 91, 1, CPU_Ret (cpu))
DEF_CMD (func, 92, (sizeof (char) + sizeof (int)), {;})

DEF_CMD (cwah, 100, 1, CPU_Cwah ())

DEF_CMD (end, -1, 1, {;})
