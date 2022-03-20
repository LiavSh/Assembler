#include "transition2.h"

void fullWordPrint(FILE * ptr ,int data, int * line, char kind)
{
    int cast = 0xf;
    int a, b, c, d, e;

    switch (kind)
    {
    case ABSOLUTE: /*case of A: A section = 0100*/
        a = ABSOLUTEVALUE;
        break;
    case RELOCATBLE: /*case of R: A section = 0010*/
        a = RELOCATBLEVALUE;
        break;
    case EXTERNAL: /*case of A: A section = 0001*/
        a = EXTERNALVALUE;
        break;
    }

    e = data & cast; /*bit manipulations - casting with 0xf*/
    data >>= FIELD; /*moving 4 bits - srl*/
    d = data & cast; /*bit manipulations - casting with 0xf*/           
    data >>= FIELD; /*moving 4 bits - srl*/
    c = data & cast; /*bit manipulations - casting with 0xf*/           
    data >>= FIELD; /*moving 4 bits - srl*/
    b = data & cast; /*bit manipulations - casting with 0xf*/

    fprintf(ptr, "%04d A%d-B%0x-C%0x-D%0x-E%0x\n", *line, a, b, c, d, e); /*print the word into the file*/
    *line = *line + 1; /*updating the line number printed*/
}

int searchOperationHashTable(char commandname[], Operation operdata[])
{
    int i;
    for(i = 0 ; i < OPERATIONHASH ; i++)
    {
        if(!strcmp(operdata[i].name,commandname)) /*comparing the givin command name to the command in the table */
        return i;
    }
    return 0;
}

void dataPrint(FILE * dataptr, char commandline[], char operandsline[], int * dataline)
{
    char comma[] = ",\n", data[] = ".data";
    char * token, * fixedstring;
    int tempnum;

    if(!strcmp(commandline, data)) /* data type*/
    {
        token = strtok(operandsline, comma); /*separating first number*/
        while(token!=NULL)
        {
            tempnum =  atoi(token); /*convert string to int*/
            fullWordPrint(dataptr, tempnum, dataline, ABSOLUTE); /*print word*/
            token = strtok(NULL, comma); /*separate next number*/
        }
    }
    else /* string type*/
    {
        fixedstring = moveTabAndSpace(operandsline); /*moving tabs and spaces*/
        ++fixedstring; /*moving the first char (") */

        while(fixedstring[0] != '"')
        {
            tempnum = fixedstring[0]; /*storing letter*/
            fullWordPrint(dataptr, tempnum, dataline, ABSOLUTE); /*print word*/
            ++fixedstring; /* move on to the next letter*/
        }

        tempnum = 0;
        fullWordPrint(dataptr,tempnum, dataline, ABSOLUTE); /*printing 0 for '\0'*/
    }
}

int powerOfTwo(int num)
{
    if(num == 0)
        return 1;
    else 
        return (2 * powerOfTwo(num - 1)); /*recursive calculation 2^(num) */
}

void registerCase(Word * word, char operand[], char flag)
{
    int regnumber;

    regnumber = registerList(operand, 0) -1; /*converting register name (string) to int (number of register)*/

    if(flag) /*flag on for dest operand*/
    {
        word->destaddress = REGADDRESSVALUE;
        word->destregister = regnumber;
    }
    else /*flag off for source operand*/
    {
        word->sourceaddress = REGADDRESSVALUE;
        word->sourceregister = regnumber;
    }
}

int indexOperand(char operand[])
{
    char * token;
    char left[] = "[", right[] = "]";

    token = strtok(operand, left);
    token = strtok(NULL, right);    
    
    return registerList(token, REGISTERALLOWDININDEX) - 1;
}

void wordStructPrint(FILE * insptr, Word word, int * line)
{
    int b,c,d,e, uppertemp, bottomtemp;
    int uppercast = 0xc, bottomcast = 0x3;

    b = word.funct; /*loading funct*/
    c = word.sourceregister; /*loading source register*/

    uppertemp = word.sourceaddress; /*loading source address into num*/
    uppertemp <<= HALFFIELD; /*sll 2 spaces to left*/
    bottomtemp = word.destregister; /*loading dest register into temp*/
    bottomtemp = bottomtemp & uppercast; /*casting dest register with 0xc*/
    bottomtemp >>= HALFFIELD; /*srl 2 spaces to right*/
    d = bottomtemp | uppertemp; /*"oring" the 4 bits together*/

    uppertemp = word.destregister; /*loading dest register into temp*/
    uppertemp = uppertemp & bottomcast; /*casting dest register with 0x3*/
    uppertemp <<= HALFFIELD; /*sll 2 spaces to left*/
    bottomtemp = word.destaddress; /*loading dest register into temp*/
    e = bottomtemp | uppertemp; /*"oring" the 4 bits together*/

    fprintf(insptr, "%04d A4-B%0x-C%0x-D%0x-E%0x\n", *line, b, c, d, e); /*print the word into the file*/
    *line = *line + 1; /*updating the line number printed*/

}


void analyzeToPrint(FILE * insptr, char firstoperand[], char secondoperand[] , int funct, int * instuctionline)
{
    Word secondword = {0};
    char secondoperandflag = 0, secondoper[MAXLENGTH];
    int indexval;

    if(secondoperand) /*command with 2 operands*/
    {
        if(firstoperand[0] == REGISTERSTART) /*the first operand is register*/
            registerCase(&secondword, firstoperand, secondoperandflag);
        else if(firstoperand[0] == IMMEDIATE) /*the first operand is immediate*/
            secondword.sourceaddress = IMMADDRESSVALUE;
        else if(firstoperand[strlen(firstoperand)-1] == ']') /*the first operand is label with index*/
        {
            secondword.sourceaddress = INDEXADDRESSVALUE;
            indexval = indexOperand(firstoperand);
            secondword.sourceregister = indexval;
        }
        else /*the first operand is label*/
            secondword.sourceaddress = DIRECTADDRESSVALUE;
        
        strcpy(secondoper, secondoperand);
    }
    else /*command with one operand*/
        strcpy(secondoper, firstoperand); /* destenation <-> source */

        secondoperandflag = 1;

        if(secondoper[0] == REGISTERSTART) /*the first operand is register*/
            registerCase(&secondword, secondoper, secondoperandflag);
        else if(secondoper[0] == IMMEDIATE) /*the first operand is immediate*/
            secondword.destaddress = IMMADDRESSVALUE;
        else if(secondoper[strlen(secondoper)-1] == ']') /*the first operand is label with index*/
        {
            secondword.destaddress = INDEXADDRESSVALUE;
            indexval = indexOperand(secondoper);
            secondword.destregister = indexval;
        }
        else  /*the first operand is label*/
            secondword.destaddress = DIRECTADDRESSVALUE;

        secondword.funct = funct; /*updating funct of command*/

        wordStructPrint(insptr, secondword, instuctionline); /*print to word into file*/
}

int printOperand(FILE * insptr, char oper[], int * line, Symbol ** table, int * arraysymbolelast, int linenumber, FILE ** exptr)
{
    char * fixed, * token;
    int num, base, heist, value;
    char s[] = "[";
    char are;

    if(oper[0] == REGISTERSTART) /*register operand - no added words*/
        return 0;

    else if(oper[0] == IMMEDIATE) /*immediate operand - one added word*/
    {
        fixed = oper + 1;
        num = atoi(fixed);
        fullWordPrint(insptr, num, line, ABSOLUTE);
    }
    else /*direct or index operand - two added words*/
    {
        if(oper[strlen(oper)-1] == ']') /*index case*/
            token = strtok(oper, s);
        else /*regular label case*/
            token = oper;


        num = symboleTableSearch(token, table, arraysymbolelast); /*searching label in label list*/

        if(num == -1) /*error - label not found*/
        {              
            printf("Line %d: Label name not exist\n", linenumber);
            return 1;
        }
        else if((*table)[num].value == 0 && (!(*table)[num].external)) /*label not defined in code nor external*/
        {
            printf("Line %d: Label name undefined\n", linenumber);
            return 1;        
        }
        else /*label found*/
        {
            if((*table)[num].external) /*label found is external*/
            {
                are = EXTERNAL;
                fprintf((*exptr), "%s BASE %04d\n%s OFFSET %04d\n\n" , (*table)[num].name, *line, (*table)[num].name, *(line) + 1);
            }
            else
                are = RELOCATBLE; /*label found in relocatble*/

            value = (*table)[num].value; /*updating value field in struct*/
            heist = calcHeist(value); /*updating heist field in struct*/
            base = value - heist; /*updating base field in struct*/

            fullWordPrint(insptr, base, line, are); /*print word - base*/
            fullWordPrint(insptr, heist, line, are); /*print word - heist*/
        }
    }
    return 0;
}



int analizeLinePrint(FILE * insptr, char commandline[], char operandsline[], int * instuctionline, Symbol ** table, Operation operdata[], int * arraysymbolelast, int linenumber, FILE ** exptr)
{
    char * fixedcommand, * firstoper, * secondoper;
    int commandindex, opcodefixed, funct, labelflag = 0;
    char comma[] = ", \t";

    fixedcommand = moveTabAndSpace(commandline); /*moving tabs and spaces*/

    commandindex = searchOperationHashTable(fixedcommand, operdata); /*seraching for the command in the command table*/
    opcodefixed = powerOfTwo(operdata[commandindex].opcode); /*converting opcode into 2^opcode*/
    funct = operdata[commandindex].funct; /*updating funct*/

    fullWordPrint(insptr, opcodefixed, instuctionline, ABSOLUTE); /*print word of the command (commend index)*/


    if(operdata[commandindex].num_of_source == '0' && operdata[commandindex].num_of_destination == '0') /*case of command without operands*/
        return 0;
    else if(operdata[commandindex].num_of_source != '0') /*two operand command*/
    {  
        firstoper = strtok(operandsline, comma); /*token of the first operand*/
        secondoper = strtok(NULL, comma); /*token of the second operand*/

        analyzeToPrint(insptr, firstoper, secondoper, funct, instuctionline); /*print the first word after command index (registers and addressing types)*/

        labelflag = printOperand(insptr, firstoper, instuctionline, table, arraysymbolelast, linenumber, exptr); /*print out first operand*/

        labelflag = + labelflag + printOperand(insptr, secondoper, instuctionline, table, arraysymbolelast, linenumber, exptr); /*print out second operand*/

    }
    else /*one operand command*/
    {
        firstoper = strtok(operandsline, comma); /*token of the first operand*/

        analyzeToPrint(insptr, firstoper, NULL, funct, instuctionline); /*print the first word after command index (registers and addressing types)*/

        labelflag = printOperand(insptr, firstoper, instuctionline, table, arraysymbolelast, linenumber, exptr); /*print out first operand*/

    }
    return labelflag;
}           


int assemblyPrint(FILE * dataptr, FILE * insptr, char command[], Symbol ** table, int * instuctionline, int * dataline, Operation operdata[], int * arraysymbolelast, int linenumber, FILE ** exptr)
{
    char * token,  * fixedline, * commandline, * operandsline,  space[] = " \r\v\f\t\n", endline[] = "\n", data[] = ".data", string[] = ".string", linecpy[MAXLENGTH];
    int labelflag = 0;

    fixedline = moveTabAndSpace(command);
    strcpy(linecpy, fixedline);

    if( fixedline[0] == '\0' || fixedline == NULL || fixedline[0] == ';' || fixedline[0] == '\n' ) /*empty sentence or comment or last line*/
        return 0;
    
    if(ignoreLabel(linecpy)) 
    {   
        token = strtok (command, space);
        token = strtok (NULL, space);
    }
    else
        token = strtok (command, space);

    if(token[strlen(token)-1] == ':') /*case of label*/
    {
        commandline = strtok(NULL, space); /*separating the second word in the line (command)*/
        operandsline = strtok(NULL, endline); /*separating the third word in the line (operands)*/
        
        if(commandline[0] == '.')/*label + data or string*/
        {
            dataPrint(dataptr, commandline, operandsline, dataline); /*printing all data words*/
        }
        else /*label + regular command line*/
        {
            labelflag = analizeLinePrint(insptr, commandline, operandsline, instuctionline, table, operdata, arraysymbolelast, linenumber, exptr); /*analyize + print line*/  
        }
    }
    else if (token[0] == '.') /* .data or .string treatment*/
    {   
        operandsline = strtok(NULL, endline); /*separating the second word in the line (operands)*/

        if(!strcmp(token, data) || !strcmp(token, string)) /* data or string*/
            dataPrint(dataptr, token, operandsline, dataline); /*printing all data words*/

        else /* .entry or .extern */
            return 0;

    }
    else /*regular command line*/
    {
        operandsline = strtok(NULL, endline); /*separating the second word in the line (operands)*/

        labelflag = analizeLinePrint(insptr, token, operandsline, instuctionline, table, operdata, arraysymbolelast, linenumber, exptr); /*analyize + print line*/
    }
    return labelflag;
}

void openfiles(FILE ** fd, FILE ** dataptr, FILE ** insptr, FILE ** entptr, FILE ** extptr, char filename[])
{
    char * token, am[] = ".am", ob[] = ".ob", dot[] = ".", ext[] = ".ext", temp[MAXLENGTH];

    strcpy(temp, filename);

    if ( ! (*fd = fopen(strcat(temp, am), READ)) ) /*opening the file to read (after macro spread)*/
    {
        fprintf(stderr, "cannot open file\n");
        exit(0);
    }

    token = strtok(temp, dot);

    if ( ! (*insptr = fopen(strcat(token, ob), WRITE)) ) /*this file  will contain the final machine code */
    {
        fprintf(stderr, "cannot open file\n");
        exit(0);
    }

    token = strtok(token, dot);

    if ( ! (*extptr = fopen(strcat(token, ext), WRITE)) ) 
    {
        fprintf(stderr, "cannot open file,ext\n"); 
        exit(0);
    }

    if ( ! (*dataptr = fopen("temp.txt", WRITE)) ) /*A temporary file that will be deleted later and will contain the machine code of the data part */
    {
        fprintf(stderr, "cannot open file\n");
        exit(0);
    }
}

void closefiles(FILE ** fd, FILE ** dataptr, FILE ** insptr, FILE ** exptr, char filename[])
{
    int size;
    char dot[] = ".", ext[] = ".ext", * token, filenamecpy[MAXLENGTH];

    fclose(*fd); /*closing files*/
    fclose(*dataptr);
    fclose(*insptr);
    remove("temp.txt");

    size = ftell(*exptr);
    fclose(*exptr);

    if(!size)
    {
        strcpy(filenamecpy, filename);
        token = strtok(filenamecpy, dot);
        remove(strcat(token, ext));
    }
}

void deleteFiles(char filename[])
{
    char * token, am[] = ".am", ob[] = ".ob", dot[] = ".", ext[] = ".ext", temp[MAXLENGTH];

    strcpy(temp, filename);
    remove(strcat(temp, am)); /*Deleting files due to a syntax error*/

    token = strtok(temp, dot);
    remove(strcat(token, ob));

    token = strtok(temp, dot);
    remove(strcat(token, ext));

    remove("temp.txt");
}


void createEntFile(Symbol ** table, int *arraysymbolelast, char filename[])
{
    char ent[] = ".ent", temp[MAXLENGTH];
    FILE * entptr;
    int i, entcounter = 0, offset;

    strcpy(temp, filename);
    
    if ( ! (entptr = fopen(strcat(temp, ent), WRITE)) ) /*searching for possible ent labels*/
        fprintf(stderr, "cannot open file(.ent)\n");

    for (i = 0; i < *arraysymbolelast+1; i++)
    {
        if ((*table)[i].entry)
        {
            offset = calcHeist((*table)[i].value);
            fprintf(entptr, "%s, %04d, %04d\n",(*table)[i].name ,(*table)[i].value - offset, offset); /*printibg entry labels*/
            ++entcounter;
        }
    }

    if(! entcounter)
    {
        fclose(entptr);
        remove(temp);
    }
}

void finalPrint(FILE ** exptr, FILE ** fd, FILE ** dataptr, FILE ** insptr, Symbol ** table, int *arraysymbolelast, char filename[])
{
    int c;
    FILE * ptr;

    fclose(*dataptr); /*closing data file*/

    if ( ! (ptr = fopen("temp.txt", READ)) ) /*open data file to read only*/
    {
        fprintf(stderr, "cannot open file\n");
        exit(0);
    }
        
    while( (c = fgetc(ptr)) != EOF) /*copying entire data file to the end of instruction file*/
        fputc(c, *insptr);


    createEntFile(table, arraysymbolelast, filename);

    closefiles(fd, &ptr, insptr, exptr, filename); /*close all files*/
}

int transitionNumTwo(char filename[] ,Symbol ** table, int instructionlinenumber, int datalinenumber, Operation operdata[], int * arraysymbolelast)
{
    int startdata, labelflag = 0, startcommand = INSTRUCTIONSTART, linenumber = 1;
    char buff[MAXLENGTH];
    FILE * dataptr, * insptr, * fd, * entptr, * extptr;
    
    startdata = instructionlinenumber;

    openfiles(&fd, &dataptr, &insptr, &entptr, &extptr, filename); /*opening several files to work with*/
    fprintf(insptr, "\t%d %d\n", instructionlinenumber-100, datalinenumber); /*printing the first line of the obj file - instruction size and data size*/

    while (!feof(fd)) /*running through the entire file, analyzing line after line*/
    {
        fgets(buff, MAXLENGTH, fd); /*copy line into buffer*/
        labelflag = labelflag + assemblyPrint(dataptr, insptr, buff, table, &startcommand, &startdata, operdata, arraysymbolelast, linenumber, &extptr); /*print the line*/
        linenumber = linenumber + 1; /*counting line numbers*/
        buff[0] = '\0';
    }

    if (labelflag) /*error found during file analyzing - delete outpot*/
        deleteFiles(filename);

    else /*assembly code is ok! - outputing file files*/
        finalPrint(&extptr, &fd, &dataptr, &insptr, table, arraysymbolelast, filename);
    
    return labelflag;
} 
