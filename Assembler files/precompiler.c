#include "common.h"

void moveBlanks(char s[])
{
    int i;
    int k = 0;

    for(i = 0 ; s[i] ; i++ )
    {
     	s[i] = s[i + k];
     	if(s[i] == '\v' || s[i] == '\f' || s[i] == '\r')
     	{
		  ++k;
		  --i;
	    }
    }
}

int countMacros (FILE *ptr) /*this func counts how many macros there are in the original file*/
{
    int macrocounter = 0;
    char line[MAXLENGTH], s[] = " \t", macro[] = "macro", * token, * fixed;

    while (!feof(ptr)) /*reading line after line from the text*/
    {
        fgets(line, MAXLENGTH, ptr); /*reading the next line*/
        fixed = moveTabAndSpace(line);
        token = strtok (fixed, s); /*token = first word in the current line*/

        if (! strcmp (token, macro) ) /*searching for the word "macro"*/
        ++macrocounter;
    }

    rewind(ptr); /*Moving file pointer to the first char*/
    return macrocounter;
}

void getMacro(Macro * mptr[], FILE * aspointer, int * macroamount)/*This func copying the macro after detected*/
{
    int i = 0;
    char line[MAXLENGTH];
    char endm[] = "endm\n", * fixed;

    while(1)
    {
        fgets(line, MAXLENGTH, aspointer);
        moveBlanks(line);
        fixed = moveTabAndSpace(line);
        printf("\r");
        
        if ( ! strcmp(fixed, endm)) /*Searching for the word "endm" - mark end of the current macro*/
            break;

        strcpy( (*mptr)[*(macroamount)-1].text[i], fixed); /*As long as the word endm is not received, the macro contents are saved*/
        ++i;
    }
}

int macroList(Macro * mptr[], FILE *ampointer, char token[], int macroamount) /*This func search for possible macro in the macro list*/
{
    int i = 0, j = 0;

    for (i = 0 ; i < macroamount ; i++) 
    {
        if(! strcmp (token, (*mptr)[i].name )) /*Running through the macro list, compering to the current line*/
        {
            while ((*mptr)[i].text[j][0] != '\0') /*Macro found - printing the macro from the macro data*/
            {
                fprintf (ampointer,"%s", (*mptr)[i].text[j]); /*Printing line after line*/
                ++j;
            }
            return 1;
        }
    }
    return 0;
}

int lineAnalysis (char line[], Macro * ptr[], FILE *aspointer, FILE *ampointer, int * macroacounter, int intmacro) /*This function analyzes the received line*/
{
    char linecpy[MAXLENGTH],  macro[] = "macro",  * token,  space[] = " \t", * fixed;
    
    strcpy(linecpy, line);
    fixed = moveTabAndSpace(linecpy);
    token = strtok (fixed, space);

    if(line[0] == '\0')
        return 0;

    if (! strcmp (token, macro) ) /*If the first word is a macro - move to the macro input function*/
    {
        strcpy((*ptr)[*(macroacounter)-1].name, strtok (NULL,space)); /*saving the name of the new macro*/
        getMacro(ptr, aspointer, macroacounter); /*copying the macro data into an array*/
        --*(macroacounter);
        return 0;
    }
    else if (macroList(ptr, ampointer, token, intmacro)) /*searching for possible macro through the macro list*/
        return 0;
    else
        fprintf(ampointer, "%s", line); /*regular line - print as is*/
    
    return 0;
}

void openPreCompileFiles(FILE ** asptr, FILE **amptr, char filename[])
{
    char as[] = ".as", am[] = ".am", * token, dot[] = ".", temp[MAXLENGTH];

    strcpy(temp, filename);

    if ( ! (*asptr = fopen(strcat(temp, as) , READ)) ) /*opening the file to convert*/
    {
        fprintf(stderr, "Error: cannot open input file(.as)\n");
        exit(0);
    }

    token = strtok (temp, dot);

    if ( ! (*amptr = fopen(strcat(token, am), WRITE)) ) /*opening the file to write into*/
    {
        fprintf(stderr, "Error: cannot open writing file(.am)\n");
        exit(0);
    }
}


void precompile(char filename[])
{
    FILE * asptr, *amptr;
    Macro * ptr;
    int * macrocounter;
    int statNumOfMacros = 0, numofmacros = 0;
    char buff[MAXLENGTH];

    openPreCompileFiles(&asptr, &amptr, filename);

    numofmacros = countMacros(asptr); /*counting how many macros are in the file*/

    statNumOfMacros = numofmacros;
    macrocounter = &numofmacros;

    ptr = (Macro*)malloc((numofmacros)*sizeof(Macro)); /*Creates a new array that will store all the data*/

    while (!feof(asptr)) /*running through the entire file, analyzing line by line*/
    {
        fgets(buff, MAXLENGTH, asptr);
        moveBlanks(buff);
        lineAnalysis (buff, &ptr, asptr, amptr, macrocounter, statNumOfMacros);
        buff[0] = '\0';
    }

    free (ptr); /*releasing dynamic mem*/
    fclose(asptr); /*closing writing file*/
    fclose(amptr); /*closing the file*/
}
