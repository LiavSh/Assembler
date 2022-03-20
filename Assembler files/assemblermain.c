#include "utils.h"
#include "precompiler.h"
#include "transition1.h"
#include "transition2.h"

int main(int argc, char *argv[])
{
    int errorcounter, IC, DC, arraysymbolesize, arraysymbolelast, i;
    Symbol * symboletable = NULL;
    Operation operdata[OPERATIONHASH];

    /*DATA: storing data in operation types*/
    Operation add = {"add", 2, 10, '4', '3'}, bne = {"bne", 9, 11, '0', '2'}, clr = {"clr", 5, 10, '0', '3'}, cmp = {"cmp", 1, 0, '4', '4'},
    dec = {"dec", 5, 13, '0', '3'}, inc = {"inc", 5, 12, '0', '3'}, jsr = {"jsr", 9, 12, '0', '2'}, jmp = {"jmp", 9, 10, '0', '2'}, 
    lea = {"lea", 4, 0, '2', '3'}, mov = {"mov", 0, 0, '4', '3'}, not = {"not", 5, 11, '0', '3'}, prn = {"prn", 13, 0, '0', '4'},
    red = {"red", 12, 0, '0', '3'}, rts = {"rts", 14, 0, '0', '0'}, sub = {"sub", 2, 11, '4', '3'}, stop = {"stop", 15, 0, '0', '0'};

    /*DATA: storing operation types in array - hashing with the first letter of the operation*/
    operdata[0] = add, operdata[1] = bne, operdata[2] = clr, operdata[3] = cmp, operdata[4] = dec, operdata[8] = inc, operdata[9] = jsr, operdata[10] = jmp,
    operdata[11] = lea, operdata[12] = mov, operdata[13] = not, operdata[15] = prn, operdata[17] = red, operdata[18] = rts, operdata[19] = sub, operdata[20] = stop;

    for (i = 1; i < argc; i++)
    {
    /* reseting all counter/pointers in the start of the iteration*/
    errorcounter = 0,  IC = INSTRUCTIONSTART, DC = 0, arraysymbolesize = 0, arraysymbolelast = -1, symboletable = NULL;
    
    /*macro spreading*/
    precompile(argv[i]); 

    /*first transition - syntex check and creating symbol table*/
    errorcounter = transitionNumOne(argv[i], &symboletable, operdata, &IC, &DC, &arraysymbolesize, &arraysymbolelast); 

    /*if syntex is ok - printing the ob, ent,ext files*/   
    if(!errorcounter)
        errorcounter = transitionNumTwo(argv[i], &symboletable, IC, DC, operdata, &arraysymbolelast);
    
    /*if there were 0 errors - the file compiled successfully*/
    if (!errorcounter)
        printf("file %s.as compiled successfully\n", argv[i]);
    else
        printf("file %s.as not compiled due to error\n", argv[i]);

    /*releasing dynamic mem*/
    if (symboletable)
        free(symboletable);
    }

    return 0;
}
