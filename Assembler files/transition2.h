#ifndef TRANSITION2_H
#define TRANSITION2_H

#include "utils.h"
#include "common.h"

/**
 * Prints word to file
 * @param ptr pointer to the file to print into
 * @param data Data to print
 * @param line Current line print
 * @param kind kind of word to print (external/entry/code)
 * @return Updated file and file pointer
 */
void fullWordPrint(FILE * ptr ,int data, int * line, char kind);

/**
 * Search command name in the operation table (hashing with the first letter of the name)
 * @param commandname Command name to search
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @return The index of the commad in the table if found, else 0
 */
int searchOperationHashTable(char commandname[], Operation operdata[]);

/**
 * Prints data word to file
 * @param dataptr pointer to data file stream
 * @param commandline Name of data type (.data/.string)
 * @param operandsline Line of data to print
 * @param dataline current line print
 * @return Updated file and file pointer
 */
void dataPrint(FILE * dataptr, char commandline[], char operandsline[], int * dataline);

/**
 * Convert decimal number into 2^(decimal number) for use in matrix
 * @param num Number to convert
 * @return 2^(given num)
 */
int powerOfTwo(int num);

/**
 * Set register number into a word struct(for later print)
 * @param word Word to update
 * @param operand Number of register
 * @param flag Determines the type of register (source or address)
 * @return Updated file and file pointer
 */
void registerCase(Word * word, char operand[], char flag);

/**
 * Seperate the register number from direct index operand 
 * @param operand The operand index
 * @return Register number
 */
int indexOperand(char operand[]);

/**
 * Prints into a file the second word of command (cont: opcode, regs, adressing type)
 * @param insptr pointer to instruction file stream
 * @param word Word with data to print
 * @param line Current line print
 * @return Updated file and file pointer
 */
void wordStructPrint(FILE * insptr, Word word, int * line);

/**
 * Analyze how much and which data to print with a given operand (one or two)
 * @param insptr Pointer of instruction file stream
 * @param firstoperand First operand to print (maybe NULL if no operands)
 * @param secondoperand Second operand to print (maybe NULL if only one operand or no operands)
 * @param funct Funct of the current commend
 * @param instuctionline Current line print
 * @return Updated file and file pointer
 */
void analyzeToPrint(FILE * insptr, char firstoperand[], char secondoperand[] , int funct, int * instuctionline);

/**
 * Prints out word (0/1/2) of the operand into file
 * @param insptr pointer to instruction file stream
 * @param oper The operand needed to be printed
 * @param line Current line print
 * @param table Pointer to the symbol table
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @param linenumber Current line read from the .am file
 * @param exptr Pointer to ext file, to print values of base and offset in filename.ext
 * @return Whether the the operand written (0:TRUE, 1:FALSE)
 */
int printOperand(FILE * insptr, char oper[], int * line, Symbol ** table, int * arraysymbolelast, int linenumber, FILE ** exptr);

/**
 * Analyze and Prints out word(s)(0/1/2) of the operands into file
 * @param insptr Pointer to instruction file stream
 * @param commandline Command name
 * @param operandsline Operand line 
 * @param instuctionline Current instruction line to print
 * @param table Pointer to the symbol table
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @param linenumber Current line read from the .am file
 * @param exptr Pointer to ext file, to print values of base and offset in filename.ext
 * @return Whether the the command and operand were written (0:TRUE, 1:FALSE)
 */
int analizeLinePrint(FILE * insptr, char commandline[], char operandsline[], int * instuctionline, Symbol ** table, Operation operdata[], int * arraysymbolelast, int linenumber, FILE ** exptr);

/**
 * Analyzes text received for printing and navigates the operands
 * @param dataptr Pointer to data file stream
 * @param insptr Pointer to instruction file stream
 * @param command Command line
 * @param table Pointer to the symbol table
 * @param instuctionline Current instruction line to print
 * @param dataline Current data line to print
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @param linenumber Current line read from the .am file
 * @param exptr Pointer to ext file, to print values of base and offset in filename.ext
 * @return Whether the the command and operand were written (0:TRUE, 1:FALSE)
 */
int assemblyPrint(FILE * dataptr, FILE * insptr, char command[], Symbol ** table, int * instuctionline, int * dataline, Operation operdata[], int * arraysymbolelast, int linenumber, FILE ** exptr);

/**
 * Open files for reading and writing (am,ext,ent and temporaries)
 * @param fd Pointer to am file
 * @param dataptr pointer to data file
 * @param insptr Pointer to ob file
 * @param entptr Pointer to ent file
 * @param extptr Pointer to ext file
 * @param filename Current file name
 * @return Whether the files were opened and the pointers were set up or not
 */
void openfiles(FILE ** fd, FILE ** dataptr, FILE ** insptr, FILE ** entptr, FILE ** extptr, char filename[]);

/**
 * Close files of reading and writing and remove temporaries
 * @param fd Pointer to am file
 * @param dataptr pointer to data file
 * @param insptr Pointer to ob file
 * @param extptr Pointer to ext file
 * @param filename Current file name
 * @return Updated pointers
 */
void closefiles(FILE ** fd, FILE ** dataptr, FILE ** insptr, FILE ** exptr, char filename[]);

/**
 * Delete all files created (in case that compilation didn't succeed)
 * @param filename name of file to delet
 */
void deleteFiles(char filename[]);

/**
 * Creating ent file, in case there arn't ent symbols - no file creation
 * @param table Pointer to the symbol table
 * @param arraysymbolelast size of the symbol table
 * @param filename Name of file to create (without .ent)
 * @return Updated ent file
 */
void createEntFile(Symbol ** table, int *arraysymbolelast, char filename[]);

/**
 * Combining instruction image and data image to one ob file
 * @param extptr Pointer to ext file
 * @param fd Pointer to am file
 * @param dataptr pointer to data file
 * @param insptr Pointer to ob file
 * @param table Pointer to the symbol table
 * @param arraysymbolelast size of the symbol table
 * @param filename Current file name
 * @return final ob and ent file
 */
void finalPrint(FILE ** exptr, FILE ** fd, FILE ** dataptr, FILE ** insptr, Symbol ** table, int *arraysymbolelast, char filename[]);

/**
 * Manages the second transition of the text file, cuts line by line and manage the necessary checks and prints
 * @param filename Current file name
 * @param table Pointer to the symbol table
 * @param instructionlinenumber Current instruction line to print
 * @param datalinenumber Current data line to print
 * @param operdata Hash table that contains all the data about the command(name, number of operands, type, etc.)
 * @param arraysymbolelast Last occupied cell in the symbol table
 * @return Whether the file printed well without errors (0:TRUE, 1:FALSE)
 */
int transitionNumTwo(char filename[] ,Symbol ** table, int instructionlinenumber, int datalinenumber, Operation operdata[], int * arraysymbolelast);

#endif
