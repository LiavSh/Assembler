#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#include "utils.h"
#include "common.h"


/**
 * Remove blanks from a string (/v,/f,/r)
 * @param s String to fix
 * @return Corrected string without white notes
 */
void moveBlanks(char s[]);

/**
 * Counts how many time the word 'macro' appears in a file
 * @param ptr Pointer to a file stream
 * @return The number of times the word macro appears in the file
 */
int countMacros (FILE *ptr);

/**
 * Copy macro lines from .as file
 * @param mptr Pointer to a macro stream
 * @param asptr Pointer to a file stream (as file)
 * @param macroamount amount of macros in the file
 */
void getMacro(Macro * mptr[], FILE * aspointer, int * macroamount);

/**
 * Search for possible macro in the macro list, if found - prints it to the file
 * @param mptr Pointer to macro table
 * @param asptr Pointer to a file stream (am file)
 * @param token Possible macro name
 * @param macroamount amount of macros in the file
 */
int macroList(Macro * mptr[], FILE * ampointer, char token[], int macroamount);

/**
 * Analyzes whether a line is a regular line or a macro line
 * @param line Line to analyze
 * @param ptr Pointer to table of macros
 * @param aspointer Pointer to .as file
 * @param ampointer Pointer to .am file
 * @param macroacounter Counter of macros in the file
 * @param intmacro Amount of macros in the file(static)
 */
void lineAnalysis (char line[], Macro * ptr[], FILE *aspointer, FILE *ampointer, int * macroacounter, int intmacro);

/**
 * Open files for the macro(s) unloading process
 * @param asptr Pointer to .as file
 * @param amptr Pointer to .am file
 * @param filename File name to analyze
 * @return Two updated pointers to files
 */
int openPreCompileFiles(FILE ** asptr, FILE **amptr, char filename[]);

/**
 * Manages the precompile transition of the text file, cuts line by line and manage the necessary checks
 * @param filename File name to analyze
 * @return A file (.am) containing the spread macros
 */
void precompile(char filename[]);

#endif
