#include "utils.h"

int symboleTableSearch(char operand[], Symbol ** symboletable, int * arraysymbolelast)
{
    int j;

    for (j = 0; j < *(arraysymbolelast)+1  ; j++)
    {
        if(!strcmp(operand, (*symboletable)[j].name)) /*seraching for symbole in the symbole list*/
            return j;
    } 
    return -1; /*if symbole is not in the symbol table: return -1*/
}

int calcHeist(int mod)
{
    int x = CALCBASE, i = 0; /*CALCBASE == 16*/
    while (mod > x)
    {
        mod = mod - x; /*finding the modulo 16 of the operand mod*/
        ++i;
    }
    return mod;
}

int registerList(char regname[], int lim)
{
    int i;
    char registerlist[][MAXREGISTERLENGTH] = REGISTERLIST;

    if (regname[0] == REGISTERSTART) 
        for ( i = lim ; i < REGISTERAMMOUNT ; i++) /*searching through the register list r0....r15*/
        {   
            if ( !strcmp(regname, registerlist[i])) /*compering the givin string (regname) to the values in the register list*/
               return i+1; /*returning the number of the register*/
        }

    return 0;
}

char * moveTabAndSpace(char txt[])
{
    int i = 0 , j;

    if (txt)
    {
        while (txt[i] == '\t' || txt[i] == ' ') /*ignoring tabs and spaces*/
            ++i;

        j = strlen(txt) -1;

        while (txt[j] == '\t' || txt[j] == ' ') /*ignoring tabs and spaces*/
            --j;
        txt[j + 1] = '\0';
    }

    return &txt[i]; /*returning new pointer to string without tabs and spaces*/
}

int ignoreLabel(char line[])
{
    char space[] = " \r\v\f\t\n", * token, entry[] = ".entry", externi[] = ".extern";
    
    token = strtok(line, space);
    if(token[strlen(token)-1] == ':')
    {
        token = strtok(NULL, space);
        if((!strcmp(token, entry)) || (!strcmp(token, externi)))
            return 1;
    }
    return 0;
}
