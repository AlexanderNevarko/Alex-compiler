#include <stdlib.h>
#include <ctype.h>
#include <string.h>




const int TABLE_SIZE = 13;
const int MAXLENGTH = 30;
const int HASH_CONST = 228;



// Table functions
List* Table_Ctor (int size, unsigned int (*hash_func) (char*));
List* Insert_in_table (List* table, int size, char* word, unsigned int (*hash_func) (char*));
bool Table_Dtor (List* table, int size);
int Table_Dump (List* table, int size);
int Table_Spectre_Graphic (List* table, int size);

// Additonary help-functions
List_Elem* Find_word (List* list, const char* word);
int FWord_reader (FILE* f, char* word, int word_size);

// Hash-function
unsigned int Gnu_Hash (char* word);




List* Table_Ctor (int size, unsigned int (*func) (char*))
{
    List* table = (List*) calloc (size, sizeof (table[0]));
    for (int i = 0; i < size; i++)
    {
        List_Ctor (&(table[i]), "hash", i);
    }

    char word[MAXLENGTH] = "";
    int isword = 0;
    while ((isword = FWord_reader (file, word, MAXLENGTH)) != EOF)
    {

        if (isword)
        {
            long long int hash = (*func) (word) % size;
            List_Elem* ptr = NULL;
            ptr = Find_word (&(table[hash]), word);
            
            if (ptr)
            {
                ptr -> count++;
            }
            else
            {
                List_Elem* elem = Elem_Ctor (word, sizeof (word));
                Put_in_tail (&(table[hash]), elem);
            }
        }
    }
    printf ("table constructed\n");
    
	return table;
}




List* Insert_in_table (List* table, int size, char* word, unsigned int (*hash_func) (char*))
{
    assert (word);
    
    List_Elem* elem = NULL;
    unsigned int hash = (*hash_func) (word) % size;

}




bool Table_Dtor (List* table, int size)
{
    assert (table);
    for (int i = 0; i < size; i++)
    {
        List_Dtor (table + i);
    }

    free (table);
    
    printf ("table destructed\n");
    return true;
}




int number_of_dump_file = 0;
int Table_Dump (List* table, int size)
{
    number_of_dump_file++;
    char* Table_file_name = (char*) calloc (strlen ("Table_dump_.gv") + 2, sizeof (*Table_file_name));
    sprintf (Table_file_name, "Table_dump_%02d.gv", number_of_dump_file);
    FILE* dump = fopen (Table_file_name, "w");
    
    fprintf (dump, "digraph hash_%d\n{\n", number_of_dump_file);

    for (int i = 0; i < size; i++)
    {
        if (table[i].length > 0)
        {
            List_Elem* ptr = table[i].N_elem -> next;
            fprintf (dump, "\t\"hash = %d\" -> \"%s\\n%d\"\n", i, ptr -> data, ptr -> count);

            for (int j = 0; j < (table[i].length - 1); j++)
            {
                fprintf (dump, "\t\"%s\\n%d\" -> \"%s\\n%d\"\n", ptr -> data, ptr -> count, ptr -> next -> data, ptr -> next -> count);
                ptr = ptr -> next;
            }
        }

        fprintf (dump, "\t\"hash = %d\" -> \"hash = %d\"\n", i, i + 1);
        
    }
    
    fprintf (dump, "}\n");

    fclose (dump);
    free (Table_file_name);
    
    return size;
}




int number_of_spectre_file = 0;
int Table_Spectre_Graphic (List* table, int size)
{
    number_of_spectre_file++;
    char* Table_graphic_file_name = (char*) calloc (strlen ("Spectre_test_.csv") + 2, sizeof (*Table_graphic_file_name));
    sprintf (Table_graphic_file_name, "Spectre_test_%02d.csv", number_of_spectre_file);
    FILE* graphic = fopen (Table_graphic_file_name, "w");

    int i = 0;
    for (i = 0; i < size; i++)
    {
        fprintf (graphic, "%d; %d\n", i, table[i].length);
    }

    fclose (graphic);

    free (Table_graphic_file_name);
    return i;
}




List_Elem* Find_word (List* list, const char* word)
{
    List_Elem* ptr = list -> N_elem -> next;
    for (int i = 0; i < (list -> length); i++)
    {
        if (strcmp (ptr -> data, word) == 0) 
		{
			return ptr;
		}
		else ptr = ptr -> next;
    }
	return NULL;
}




int FWord_reader (FILE* f, char* word, int word_size)
{
    int c = 0;
    int i = 0;
    c = fgetc (f);
    if (c == '\0') return 0; 

    memset (word, 0, word_size);

    for (i = 0; i < word_size - 1;)
    {
        if (c == '\0')
        {
            break;
        }
        
        if (c == EOF)
        {
            word [i] = '\0';
            if (i > 0) return 1;
            else return EOF;
        }
        
        if (!isalpha (c)) break;
        
        if (isalpha (c)) word[i++] = c;
		
		c = fgetc (f);
    }

    word [i] = '\0';
    return i;
}




unsigned int Gnu_Hash (char* word)
{
    unsigned int hash = 6043;

    while (*word)
        hash = hash * 37 + *word++;

    return hash;
}
