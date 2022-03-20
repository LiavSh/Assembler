#ifndef TRANSITION1_H
#define TRANSITION1_H

#include "utils.h"
#include "common.h"


/**
 * Removes white characters, ie spaces and tabs
 * @param s The string to fix
 * @return Fixed string
 */
void removeSpaces(char s[]);

/**
 * Checks if the given string is a valid number
 * @param token The string to analyze
 * @param linenumber The current analyzed line number
 * @return Whether the given string is a number (0:TRUE, 1:FALSE)
 */
int isNum(char token[], int * linenumber);

/**
 * Counts the amount of commas in the given string
 * @param dataline The string to analyze
 * @return The ammount of commas in the given string
 */
int commaCounter(char dataline[]);

/**
 * Checks if the given string contains " in the first and the last char
 * @param str The string to analyze
 * @param linenumber The current analyzed line number
 * @return Whether the given string is a valid string "TXT" (0:TRUE, 1:FALSE)
 */
int isString(char str[], int * linenumber);

/**
 * Checks if the given label name is valid (not a word reserved for the system)
 * @param labelname Label name to check
 * @param operdata The current analyzed line number
 * @return Whether he given string is a valid label name (0:TRUE, 1:FALSE)
 */
int checkLabelName(char labelname[], Operation operdata[]);

/**
 * Analyze operand type - check if the given operand is index address or direct address
 * @param operand Operand to Analyze
 * @param linenumber The current analyzed line number
 * @return Whether the given operand suits to case 2: index address or direct address (0:TRUE, 1:FALSE)
 */
int secondCase(char operand[] , int * linenumber);

/**
 * Analyze operand type - check if the given operand is index address/direct address/register direct
 * @param operand Operand to Analyze
 * @param linenumber The current analyzed line number
 * @return Whether the given operand suits to case 3: index address/direct address/register direct (0:TRUE, 1:FALSE)
 */
int thirdCase(char operand[] , int * linenumber);

/**
 * Analyze operand type - check if the given operand is index address/direct address/register direct/immediate
 * @param operand Operand to Analyze
 * @param linenumber The current analyzed line number
 * @return Whether the given operand suits to case 4: index address/direct address/register direct/immediate (0:TRUE, 1:FALSE)
 */
int fourthCase(char operand[] , int * linenumber);

/**
 * Determines which test will be performed on the operand according to its type (case 2/3/4)
 * @param opertype key word to nevigate between case 2/3/4
 * @param operand Operand to check syntactically
 * @param linenumber The current analyzed line number
 * @return Whether the given operand is valid (0:TRUE, 1:FALSE)
 */
int valueCheck(char opertype, char operands[], int * linenumber);

/**
 * Break down the operands line into pieces, and check each one of the pieces syntactically
 * @param index Command index as shown in the hash table
 * @param operands Operand line to check
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param linenumber The current analyzed line number
 * @return Whether the given operand line is valid (0:TRUE, 1:FALSE)
 */
int operandsCheck(int index, char operands[], Operation operdata[], int * linenumber);

/**
 * Search thorugh operation list for the given commend name 
 * @param name Potentially command name
 * @param operands operands that follows the command name
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param linenumber The current analyzed line number
 * @return Whether the given name is valid command (0:TRUE, 1:FALSE)
 */
int operationList(char name[], char operands[], Operation operdata[], int * linenumber);

/**
 * Check that a label name exists and that the first letter is not a number
 * @param label Label name to check
 * @param linenumber The current analyzed line number
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @return Whether the given label is syntactically valid (0:TRUE, 1:FALSE)
 */
int checkLabel(char label[], int * linenumber, Operation operdata[]);

/**
 * Check the syntax of the given line
 * @param line Line to check
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param linenumber The current analyzed line number
 * @return Whether the given line is syntactically valid (0:TRUE, 1:FALSE)
 */
int syntexCheck(char line[], Operation operdata[], int * linenumber);

/**
 * Add a new symbol (label) to the symbol table
 * @param label Label name to add
 * @param symboletable Pointer to the symbol table
 * @param arraysymbolesize Current size of the symbol table
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @param kind Lind of the label (entry/extern/code/data)
 * @param address The adress of the label in the object file
 * @return Whether the addition was successful or not (1:TRUE, 0:FALSE(and exit))
 */
int insertSymbole(char label[], Symbol ** symboletable, int * arraysymbolesize, int * arraysymbolelast, char kind, int *address);

/**
 * Counts the number of machine words needed for a operand
 * @param operand Operand to analyze
 * @return Amount of machine words needed for the given operand
 */
int wordsInOperand(char operand[]);

/**
 * Counts the number of machine words needed for a operands line
 * @param operands Line to analyze
 * @return Amount of machine words needed for the given operands line
 */
int instructionCounter(char operandsLine[]);

/**
 * Counts the number of machine words needed for a line (include command)
 * @param line Line to count
 * @param linenumber The current analyzed line number
 * @param IC Instruction counter
 * @param DC Data Counter
 * @param symboletable Pointer to the symbol table
 * @param arraysymbolesize Current size of the symbol table
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @return Whether the addition was successful or not (0:TRUE, 1:FALSE)
 */
int countInstructionAndSymboles(char line[], int * linenumber, int * IC, int * DC, Symbol ** symboletable, int * arraysymbolesize, int * arraysymbolelast);

/**
 * Analyze a given line from the file, checks syntex and count number for mechine word needed
 * @param line Line to analyze
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param linenumber The current analyzed line number
 * @param IC Instruction counter
 * @param DC Data Counter
 * @param symboletable Pointer to the symbol table
 * @param arraysymbolesize Current size of the symbol table
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @return Whether the given line is syntactically valid (0:TRUE, 1:FALSE)
 */
int lineAnalyze(char line[], Operation operdata[], int * linenumber, int * IC, int *DC, Symbol ** symboletable, int * arraysymbolesize, int * arraysymbolelast);

/**
 * Adding the IC value to data label (seperate them from instruction image)
 * @param symboletable Pointer to the symbol table
 * @param IC Updated instruction counter
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @return Corrected pointer to symbol table with updtated values(of data labels only)
 */
void addValuesToDataTypes(Symbol ** symboletable, int * IC, int * arraysymbolelast);

/**
 * Checks if the given line does not exceed the maximum limit 
 * @param ptr Pointer to the file stream
 * @param linenumber The current analyzed line number
 * @return Whether the line is within the limit(0:TRUE, 1:FALSE)
 */
int checkMAxLength(FILE * ptr, int * linenumber);

/**
 * Manages the first transition of the text file, cuts line by line and manage the necessary checks
 * @param filename Line to analyze
 * @param symboletable Pointer to the symbol table
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param IC Instruction counter
 * @param DC Data Counter
 * @param arraysymbolesize Current size of the symbol table
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @return Whether the file is syntactically ok (0:TRUE, 1:FALSE)
 */
int transitionNumOne(char filename[], Symbol ** symboletable, Operation operdata[], int * IC, int * DC, int * arraysymbolesize, int * arraysymbolelast);

#endif
