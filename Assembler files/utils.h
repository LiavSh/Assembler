#ifndef UTILS_H
#define UTILS_H

/*C Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*defines*/
#define REGISTERLIST {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"}
#define READ "r+"
#define WRITE "w"
#define ABSOLUTE 'A'
#define RELOCATBLE 'R'
#define EXTERNAL 'E'
#define EXTERN 'X'
#define ENTRY 'E'
#define DATA 'D'
#define CODE 'C'
#define REGISTERSTART 'r'
#define IMMEDIATE '#'
#define OPERATIONAMMOUNT 16
#define MAXMEMSPACE 8192
#define REGISTERALLOWDININDEX 10
#define OPERATIONHASH 26
#define REGISTERAMMOUNT 16
#define MAXLENGTH 82
#define MAXREGISTERLENGTH 4
#define VALOFA 97
#define CALCBASE 16
#define MAXMACROLINES 6
#define INSTRUCTIONSTART 100
#define MAXLENGTHCOMMANDNAME 5
#define ABSOLUTEVALUE 4
#define RELOCATBLEVALUE 2
#define EXTERNALVALUE 1
#define REGADDRESSVALUE 3
#define DIRECTADDRESSVALUE 1
#define INDEXADDRESSVALUE 2
#define IMMADDRESSVALUE 0
#define FIELD 4
#define HALFFIELD 2


/*structs*/
typedef struct /*This structure holds the name and rows of the macro*/
{
    char name [MAXLENGTH];
    char text[MAXMACROLINES][MAXLENGTH];
}Macro;

typedef struct /*This structure holds data regarding the various commands of the machine*/
{
    char name [MAXLENGTHCOMMANDNAME];
    int opcode;
    int funct;
    char num_of_source;
    char num_of_destination;
}Operation;

typedef struct /*This structure holds data about the symbols collected during the first transition*/
{
    char name[MAXLENGTH];
    int value;
    char entry;
    char external;
    char data;
    char code;
}Symbol;

typedef struct /*This structure holds the data of the second word of each command in the language (names of registers / types of addresses, etc.)*/
{
    unsigned int destaddress:2;
    unsigned int destregister:4; 
    unsigned int sourceaddress:2;
    unsigned int sourceregister:4;
    unsigned int funct:4;
    unsigned int are:4;
}Word;

#endif
