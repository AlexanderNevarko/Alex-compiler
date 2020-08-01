// CPU  (c)AlexN  v.2.0
const int ASSM_VERSION = 2;
#define MEGADEBUG


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>


const int MARKNUM = 100;
const int NO_MARK = -1488;
const int MAXNAME = 10;
const int MAXLENGTH = 10;
int ASSM_COUNTER = 0;

//______________________________________________________________
//    ENUM                                                      |
#define DEF_CMD(name, num, length, func) CMD_##name = num,

enum CMDs
{
    #include "my_cmd.h"
    CMD_nope
};

#undef DEF_CMD
//______________________________________________________________|


//array of marks struct_________________________________________
//    ALL ABOUT MARKS                                           |
struct mark_t
{
    int markname;
    int address; 
};


void marks_Ctor (mark_t* marks, int number)
{
    for (int i = 0; i < number; i++)
    {
        marks[i].markname = NULL;
        marks[i].address = -1;
    }
}


void marks_Dump (mark_t* marks, int number)
{
    FILE* f = fopen ("assembler_dump_file.txt", "a");
    fprintf (f, "\ntime: %d\n"
                "marks[%d]:\n"
                "mark names:    ", time (NULL), number);
    
	for (int i = 0; i < number; i++)
    {
        fprintf (f, "%-15d", marks[i].markname);
    }

	fprintf (f, "\nmark addresses:");
    
	for (int j = 0; j < number; j++)
    {
        fprintf (f, "%-15d", marks[j].address);
    }

	fprintf (f, "\n");
}


int Find_addr (mark_t* marks, int mark) // searches in assm marks array for a mark called "mark" and returns its' address
{
    printf ("\nFind_addr called, markname: %d\n", mark);
    int i = 0;
    while (i < MARKNUM)
    {
        if (marks[i].markname == mark)
        {
            printf ("markaddr: %d\n", marks[i].address);
            return marks[i].address;
        }
        else i++;
    }
    if (i >= MARKNUM) 
    {
         printf ("no mark found\n");
        return NO_MARK;
    }
}
//______________________________________________________________|



bool Assembler (FILE* program, FILE* codefile, mark_t* marks);
void What_to_do (FILE* program, const char* buf, char** ptr, char num, int cmdlength, mark_t* marks);
//FILE* Disassembler (char* prog);
long Filesize (FILE* f);
int hashname (char* name);




int main()
{
    FILE* codefile = fopen ("codefile.txt", "wb");
    FILE* program = fopen ("program.txt", "r");
    
    struct mark_t marks [MARKNUM] = {};
    marks_Ctor (marks, MARKNUM);
    
    Assembler (program, codefile, marks);
    Assembler (program, codefile, marks);
    
    fclose (codefile);
    
    system ("pause");
    return 0;
}




static int markcount = 0;

bool Assembler (FILE* program, FILE* codefile, mark_t* marks)
{
    ASSM_COUNTER++;
    printf ("\nProgram started assembling %d time\n\n", ASSM_COUNTER);

    long fsize = Filesize (program);
    
    char c = 0;
    int line = 0;
    int word = 0;
    c = fgetc (program);
    
    while (c != EOF)
    {
        if (isspace (c)) word++;
        if (c == '\n') line++;
        c = fgetc (program);
    }
    rewind (program);

	int bufsize = fsize * sizeof (char);
	char* buf = (char*) calloc (bufsize, sizeof (char));
    char* ptr = buf;
    printf ("%x\t%x\n", buf, ptr);
    char cmd[MAXLENGTH] = "";

    #define DEF_CMD(name, num, cmdlength, func) \
            if ((strcmp (cmd, #name) == 0)) \
            { \
                printf ("compares cmd = %s with %s\n", cmd, #name); \
                *((char*) ptr) = num; \
                ptr += sizeof (char); \
                What_to_do (program, buf, &ptr, num, cmdlength, marks);\
            } \
            else 
    
    c = fgetc (program);
    while (c != EOF)
    {
        ungetc (c, program);
        fscanf (program, "%s", cmd);
        printf ("%s\n", cmd);
        #include "my_cmd.h"
        {
            assert (!"Invalid comand!");
        }
        c = fgetc (program);
    }
    
    rewind (program);

    #undef DEF_CMD
    
    if (ASSM_COUNTER > 1) fwrite (buf, sizeof (char), bufsize, codefile);

    printf ("\nProgram finished assembling. Program file charachteristics:\n"
            "words: %d\n"
            "lines: %d\n"
            "marks: %d\n"
            "file size: %i\n", ++word, ++line, markcount, fsize);

    marks_Dump (marks, markcount);

    free (buf);
    markcount = 0;

    return buf;
}




void What_to_do (FILE* program, const char* buf, char** ptr, char num, int cmdlength, mark_t* marks)
{
    char cmd[MAXLENGTH] = "";
    char funcname[MAXNAME] = "";
    char regname[2] = "";
    double val = 0;
    int mk = 0;

    if (cmdlength == (sizeof(char) + sizeof(double))) /* pushes */
        {
            fscanf (program, "%lg", &val);
            *((double*) *ptr) = val;
            printf ("in buf: %lg\n", *((double*) *ptr));
            *ptr += sizeof (double);
        }
    else if (cmdlength == (sizeof(char) + sizeof(char))) /* pushrs and poprs */
        {
            fscanf (program, "%s", regname);
            *((char*) *ptr) = (regname[0] - 'a');
            printf ("in reg#: %d\n", *((char*) *ptr));
            *ptr += sizeof (char);
        }
    else if (num == CMD_mark) /* " : " */
        {
            fscanf (program, "%d", &(marks[markcount].markname));
            *((int*) *ptr) = marks[markcount].markname;
            *ptr += sizeof (int);
            marks[markcount].address = (int)(*ptr - buf);
            printf ("mark: %d, address: %d, markcount: %d\n", marks[markcount].markname, marks[markcount].address, markcount);
            markcount++;
        }
    else if (num >= CMD_jmp && num <= CMD_jm) /* jumps */
        {
            fscanf (program, "%d", &mk);
            *((int*) *ptr) = Find_addr (marks, mk);
            *ptr += sizeof (int);
        }
    else if (num == CMD_call) /* call */
        {
            fscanf (program, "%s", funcname);
            int hash = hashname (funcname);
            printf ("name: %s, hashname = %d\n", funcname, hash);
            *((int*) *ptr) = Find_addr (marks, hash);
            *ptr += sizeof (int);
        }
    else if (num == CMD_func) /* define of function */
        {
            fscanf (program, "%s", funcname);
            marks[markcount].markname = hashname (funcname);
            printf ("name: %s, hashname = %d\n", funcname, marks[markcount].markname);
            *((int*) *ptr) = marks[markcount].markname;
            *ptr += sizeof (int);
            marks[markcount].address = (int)(*ptr - buf);
            printf ("func: %d, address: %d, markcount: %d\n", marks[markcount].markname, marks[markcount].address, markcount);
            markcount++;
        }
    else ;
}




long Filesize (FILE* f)
{
    long sz = -1;
    fseek (f, 0, SEEK_END);
    sz = ftell (f);
    rewind (f);
    assert (!(sz == 0));
    assert (!(sz == -1));
    //printf ("size of file = %d\n", sz);
    return sz;
}




int hashname (char* name)
{
    unsigned int hash = 28657;
    for (int i = 1; i < MAXNAME; i++)
    {
        hash = hash * 37 + name[i];
    }
    return hash;
}
