#ifndef COMMON_H
#define COMMON_H

#include "utils.h"

/**
 * Searches for a symbol in the symbol table
 * @param operand Name of symbol to search
 * @param symboletable Pointer to the symbol table
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @return Index of the symbol in the symbol table
 */
int symboleTableSearch(char operand[], Symbol ** symboletable, int * arraysymbolelast);

/**
 * Finding the modulo (16) of a given number  
 * @param mod The number to mod
 * @return Mudulo of the given operand
 */
int calcHeist(int mod);

/**
 * Searches for a register name in the list of registers
 * @param regname The number to mod
 * @param lim Limiting the search to a specific value
 * @return register number +1
 */
int registerList(char regname[], int lim);

/**
 * Move tabs and spaces from the beginning and the end of a string  
 * @param txt String to fix
 * @return Pointer to string without spaces and tabs
 */
char * moveTabAndSpace(char txt[]);

/**
 * Check if the given line contains label + .entry/.extern 
 * @param line Command line to check
 * @return Does the line have a label before .entry/.extern (1:TRUE, 0:FALSE)
 */
int ignoreLabel(char line[]);

#endif
