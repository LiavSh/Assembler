#include "transition1.h"

void removeSpaces(char s[])
{
    int k = 0, i;

    for(i = 0 ; s[i] ; i++ )
    {
     	s[i] = s[i + k];
     	if(s[i] == ' ' || s[i] == '\t') /*ignoring tabs and spaces*/
     	{
		  ++k; 
		  --i;
	    }
    }
}

int isNum(char token[], int * linenumber)
{
    int i = 0;

    if (token[0] == '-' || token[0] == '+') /*ignoring '+' or '-' if they are located in the begining of the number*/
        i = 1;

    while (token[i] != '\0') /*running until the end of the token*/
    {
        if (! isdigit(token[i])) /*checking if the giving string is a number*/
        {
            printf("Line %d: Invalid type of data - not an integer\n", *(linenumber));
            return 1;
        }
        ++i;
    }
    return 0;
}

int commaCounter(char dataline[])
{
    int i = 0, comma = 0;

    for (i = 0 ; i < strlen(dataline) ; i++) /*countung the number of commas in the string*/
    {
        if (dataline[i] == ',') /*comma detected*/
            comma = comma + 1; /*adding one to the counter*/
    }
    return comma; /*returinig the total ammount of commas*/
}

int isString(char str[], int * linenumber)
{
    if ( (str[0] == '"') && (str[strlen(str)-1] == '"') ) /*making sure the giving str is a in template of "STRING" */
        return 0;
    else
    {
        printf("line %d: Invalid string format\n", *(linenumber));
        return 1;
    }
}

int checkLabelName(char labelname[], Operation operdata[])
{
    int i = 0;
    int temp;
    
    temp = strlen(labelname) -1;
    
    if(labelname[temp] == ':') /*moving the ':' from the end of the label*/
        labelname[strlen(labelname) -1] = '\0';

    for ( i = 0 ; i < REGISTERAMMOUNT; i++) /*checking if the giving label name dont match a register name*/
    {
        if (! strcmp(operdata[i].name, labelname))
            return 1;
    }
    
    if(registerList(labelname, 0))
        return 1;    

    return 0;
}

int secondCase(char operand[] , int * linenumber)
{
    char left[] = "[\0", right[] = "]";
    char * token;

    token = strtok(operand, left); /*first step in checking if index addressing label is syntactically correct - separate the label name*/

    if (! isalpha(token[0]) || registerList(token, 0)) /*making sure label is syntactically correct*/
    {
        printf("Line %d: Unvalid operand type\n", *(linenumber));
        return 1;
    }

    token = strtok (NULL, right); /*second token - the register name*/

    if (!token)
        return 0;
    
    if (! registerList(token, REGISTERALLOWDININDEX)) /*checking if the giving register inside the index is between r10 - r15*/
    {
        printf("Line %d: Invalid register number\n", *(linenumber));
        return 1;
    }
    return 0;
}

int thirdCase(char operand[] , int * linenumber)
{
    char *token, col[] = "[", endline[] = "\0", temp[MAXLENGTH];

    if(operand[0] == REGISTERSTART) /*case of register addressing - checking if operand statrs with 'r'*/
    {
        strcpy(temp, operand);
        token = strtok(temp, col);
        token = strtok(NULL, endline); /*seperating the register name*/

        if(!token)
        {
            if(!registerList(operand, 0)) /*chacking if the register name is between r0...r15*/
            {
                printf("Line %d: Invalid register number\n", *(linenumber));
                return 1;
            }
            else
                return 0;
        }
    }
    return secondCase(operand, linenumber); /*checking second case as well*/
}


int fourthCase(char operand[] , int * linenumber)
{
    char * numsign;

    if (operand[0] == IMMEDIATE) /*case of operand statr with # - for immediate*/
    {
        numsign = operand; /*moving '#' */
        numsign = numsign + sizeof(char);

        if (isNum(numsign, linenumber)) /*chching is its a valid number*/
            return 1;
        else
            return 0;
    }
    return thirdCase(operand ,linenumber); /*checking index adressing and direct adressing as well*/
}


int valueCheck(char opertype, char operand[], int * linenumber)
{
    int flag = 0;
    char * fixed;
    
    fixed = moveTabAndSpace(operand);

    switch (opertype)
    {
    case '0': /*first case = no operand*/
        {
            if(fixed) /*In this case there is an operand where it should not be */
            {
                printf("Line %d: Extraneous text after end of command\n", *(linenumber));
                flag = 1;
                break;
            }
        }
        return 0;
        break;

    case '2': /*second case = possible oprands are index and direct*/
        {
            if (!fixed) /*making sure operand != NULL*/
            {
                printf("Line %d: Missing parameter after command\n", *(linenumber));
                flag =  1;
                break;
            }
            flag = secondCase(fixed, linenumber); /*case of direct/index adressing*/
        }
        break;

    case '3':
        {
            if (!fixed) /*making sure operand != NULL*/
            {
                printf("Line %d: Missing parameter after command\n", *(linenumber));
                return 1;
                break;
            }
            flag = thirdCase(fixed, linenumber); /*case of direct/index/register direct adressing*/       
            
        }
        break;

    case '4':
        {
            if (!fixed) /*making sure operand != NULL*/
            {
                printf("Line %d: Missing parameter after command\n", *(linenumber));
                flag = 1;
                break;
            }
            flag = fourthCase(fixed, linenumber); /*case of direct/index/register direct/immediate adressing*/
        }
        break;
    }

    return flag;
}

int operandsCheck(int index, char operands[], Operation operdata[], int * linenumber)
{
    char * frsttoken, * scndtoken, * trdtoken;
    char comma[] = ", \r\v\f\t\n\n", endline[] = "\r\v\f\t\n", temp[MAXLENGTH];
    int numofcommas;
    int flag = 0;

    if(operands)
        strcpy(temp, operands);

    frsttoken = strtok(operands, comma); /*separate first token -  first operand*/
    scndtoken = strtok(NULL, comma); /*separate second token -  second operand*/
    trdtoken = strtok(NULL, endline); /*separate third token -  third operand*/

    if (operdata[index].num_of_source != '0') /*case of command with 2 operands*/
    {
        numofcommas =  commaCounter(temp) -1;  
        flag = flag + valueCheck(operdata[index].num_of_source, frsttoken, linenumber); /*first operand check (source)*/
        flag = flag + valueCheck(operdata[index].num_of_destination, scndtoken, linenumber); /*second operand check (dest)*/
    }
    else if (operdata[index].num_of_destination != '0' && operdata[index].num_of_source == '0')/*case of command with 1 operand*/
    {
        numofcommas =  commaCounter(temp);
        flag = valueCheck(operdata[index].num_of_destination, frsttoken, linenumber); /*case of only 1 operand to check (dest)*/
        trdtoken = scndtoken;
    }
    else /*case of command with 0 operands (rts/stop)*/
    {
        numofcommas =  commaCounter(temp);
        frsttoken = scndtoken;
    }

    if(trdtoken)
    {
        printf("Line %d: Extraneous text after end of command\n", *(linenumber)); /*extra text*/
        flag = 1;
    }
    else if (numofcommas)
    {
        printf("Line %d: Unvalid amount of commas\n", *(linenumber)); /*number of commas is bigger then it should be*/
        flag = 1;
    }

    return flag;
}

int operationList(char name[], char operands[], Operation operdata[], int * linenumber)
{
    int operationindex, returnval, flag = 1;
    char * operandswithoutspace;
    
    for (operationindex = (name[0] - VALOFA) ; operationindex < OPERATIONHASH ; operationindex++) /*searching through the hash table */
    {
        if ( ! strcmp(operdata[operationindex].name, name)) /*command name match*/
        {
            flag = 0;
            break;
        }
    }

    if (flag) /*command name not found*/
    {
        printf("Line %d: Operation name undefined\n", *(linenumber)); 
        return flag;
    }
    
    operandswithoutspace = moveTabAndSpace(operands); /*moving tabs and spaces from the operand line*/

    returnval = operandsCheck(operationindex, operandswithoutspace, operdata, linenumber); /*anaylizing the operands syntex*/

    return returnval;   
}

int checkLabel(char label[], int * linenumber, Operation operdata[])
{
    if(!label)
    {
        printf("Line %d: label name missing\n", *(linenumber));
        return 1;
    }
    else if (! isalpha (label[0])) /*make sure a label does not start with a number*/
    {
        printf("Line %d: Invalid label\n", *(linenumber));
        return 1;
    }
    else if (checkLabelName(label, operdata)) /*make sure a label dont match register name*/
    {
        printf("Line %d: Invalid label - name reserved for the system\n", *(linenumber));
        return 1;
    }
    return 0;
}

int syntexCheck(char line[], Operation operdata[], int * linenumber)
{
    char * token, * fixedline, * operandline, * numchar, * fixedtoken;
    char buff[MAXLENGTH], space[] = " \r\v\f\t\n", comma[] = ",", newline[] = "\n", data[] = ".data", string[] = ".string", externi[] = ".extern", entry[] = ".entry"; 
    int flag = 0, datacouner = 0, numofcommas = 0;

    strcpy(buff, line); /*analyzing with copy of the string*/
    token = strtok (buff, space);

    if(token[strlen(token)-1] == ':') /*removing labeles*/
    {
        if (checkLabel(token, linenumber, operdata)) /*checking label syntex*/
            return 1;

        else
        {
            token = strtok(NULL, space);
            if(!token) /*no text after label definition*/
            {
                printf("Line %d: Missing command line after label\n", *(linenumber));
                return 1;
            }
        }
    }

    if (token[0] == '.') /* .data or .string treatment*/
    {   
        if(!strcmp(token, data)) /* .data type*/
        {
            token = strtok(NULL, newline);
            numofcommas = commaCounter(token); /*counting number of commas in data*/
            token = strtok(token, comma); /*locating first int*/

            while(token != NULL)
            {
                numchar = moveTabAndSpace(token); /*moving tabs and spaces from the word*/
                flag = flag + isNum(numchar, linenumber); /*checks if the giving token is an integer*/
                token = strtok(NULL, comma);
                ++datacouner;
            }

            if (datacouner-1 != numofcommas)/*compering number of data types and commas*/
            {
                printf("Line %d: Invalid number of commas\n", *(linenumber));
                return 1;
            }
            else if(flag)
                return 1;
        }
        else if (!strcmp(token, string)) /* .string type*/
        {
            token = strtok(NULL, newline);
            fixedline = moveTabAndSpace(token); /*moving tabs and spaces from the word*/
            return isString(fixedline, linenumber); /*checking valid string*/
        }
        else if ( (!strcmp (token, externi)) || (!strcmp(token, entry)) )
        {
            token = strtok(NULL, newline);
            fixedtoken = moveTabAndSpace(token); /*moving tabs and spaces from the word*/
            return checkLabel(fixedtoken, linenumber, operdata); /*checking if the operand of extern or entry is a valid label*/
        }
        else
        {
            printf("Line %d: Unknown data type\n", *(linenumber)); /*start with dot (.) but not .data or .string*/
            return 1;
        }
    }
    else 
    {
        operandline = strtok(NULL, newline); /*saving operands for later analyze*/
        return operationList(token, operandline, operdata, linenumber); /*Instructional sentence*/
    }
    return 0;
}

int insertSymbole(char label[], Symbol ** symboletable, int * arraysymbolesize, int * arraysymbolelast, char kind, int *address)
{

    if(*(arraysymbolesize) == 0) /*first symbole - creating the symbol table*/
    {
        *(arraysymbolesize) = 1;
        *(symboletable) = (Symbol*)malloc(sizeof(Symbol)); /*allocating the first symbole*/
    }
    else if ((*arraysymbolesize) == (*arraysymbolelast)+1) /*case the the symbol table is full*/
    {
        (*arraysymbolesize) = (*arraysymbolesize) * 2; /* doubel the vector size */
        (*symboletable) = (Symbol*)realloc((*symboletable), (*arraysymbolesize) * sizeof(Symbol));
    }

    if(symboletable == NULL) /*in case of memory alocation not completed*/
    {
        printf("ERROR. Memory allocation failed\n");
        exit(0);
    }

    *(arraysymbolelast) = *(arraysymbolelast) + 1;
    strcpy((*symboletable)[*arraysymbolelast].name, label); /*setting the name of the symbol*/

    (*symboletable)[*arraysymbolelast].code = 0; /*reseting all values(evoid garbage)*/
    (*symboletable)[*arraysymbolelast].data = 0;
    (*symboletable)[*arraysymbolelast].entry = 0;
    (*symboletable)[*arraysymbolelast].external = 0;
    (*symboletable)[*arraysymbolelast].value = 0;


    if (kind != ENTRY)
        (*symboletable)[*(arraysymbolelast)].value = (*address); 

    switch (kind)
    {
    case ENTRY: /*case of entry*/
        (*symboletable)[*(arraysymbolelast)].entry = ENTRY;
        break;
    case EXTERN: /*case of extern*/
        (*symboletable)[*(arraysymbolelast)].external = EXTERN;
        break;
    case DATA: /*case of data*/
        (*symboletable)[*(arraysymbolelast)].data = DATA;
        break;
    case CODE: /*case of regular label*/
        (*symboletable)[*(arraysymbolelast)].code = CODE;
    break;

    default:
        break;
    }            

    return 1;
}

int wordsInOperand(char operand[])
{
    if(operand[0] == IMMEDIATE)  /*one extra word for immediate*/
        return 1;
    else if (operand[0] == REGISTERSTART) /*no extra words for register addressing*/
        return 0;
    else /*two extra words for index addressing or direct adrresing*/
        return 2; 
}

int instructionCounter(char operandsLine[])
{
    char comma[] = ",\n";
    char * token;
    int counter = 0;

    token = strtok(operandsLine, comma); /*checking how many words need for the first operand (if exist*/
    if(token)
        counter = wordsInOperand(token) + 1; /* +1 is for the second word that (funct and regs)*/

    token = strtok(NULL, comma); /*checking how many words need for the second operand (if exist*/
    if(token)
        counter = counter + wordsInOperand(token);

    return counter; /*returning the ammount of word needed for the command line*/
}

int countInstructionAndSymboles(char line[], int * linenumber, int * IC, int * DC, Symbol ** symboletable, int * arraysymbolesize, int * arraysymbolelast)
{
    char * token, * oper, * fixedstring;
    char data[] = ".data", string[] = ".string", entry[] = ".entry", space[] = " \r\v\f\t\n", endline[] = "\r\v\f\t\n", label[MAXLENGTH], linecpy[MAXLENGTH];
    int commaammount = 0, symboleindex = 0, extenaladdress = 0;

    strcpy(linecpy, line);


    if(ignoreLabel(linecpy)) /*moving label before entry or extern*/
    {   
        token = strtok(line, space);
        token = strtok(NULL, space);
    }
    else
        token = strtok(line, space);

    
    if(token[strlen(token)-1] == ':') /*analyze label (code/string/data/extern/entry) */
    {
        token[strlen(token)-1] = '\0';
        strcpy(label, token); /*copying label name for later use*/
        
        token = strtok(NULL, space); /*command after label*/
        oper = strtok(NULL, endline); /*operand after command*/
        
        symboleindex = symboleTableSearch(label, symboletable, arraysymbolelast); /*searching the symbole through symbole table*/

        if (token[0] == '.') /*for label: .data/.string*/
        {
            if(symboleindex == -1) /*case that the symbol does not exist in the symbol table*/
                insertSymbole(label, symboletable, arraysymbolesize, arraysymbolelast, DATA, DC); /*inserting the new symbol*/

            else if((*symboletable)[symboleindex].external) /*symbol found and its already exist as extern*/
            {
                printf("Line %d: label name already set as external\n", *(linenumber));
                return 1;
            }
            else if((*symboletable)[symboleindex].code || (*symboletable)[symboleindex].data)  /*symbol found but its already exist*/
            {
                printf("Line %d: label name already set\n", *(linenumber)); 
                return 1;
            }
            else
            {
                (*symboletable)[symboleindex].data = DATA; /*updating symbole to be data*/
                (*symboletable)[symboleindex].value = (*DC); /*updating DC counter*/  
            }

            if(!strcmp(token, string))
            {
                fixedstring = moveTabAndSpace(oper); /*moving "white chars" for the line*/
                (*DC) = (*DC) + strlen(fixedstring) - 1; /*updating the amount of words needed for the string, inculding '\0'*/
            }
            else if(!strcmp(token, data))
            {
                commaammount = commaCounter(oper);  
                (*DC) = (*DC) + commaammount + 1;
            }
        }
        else /*for normal command line with operands*/
        {
            if(symboleindex == -1) /*case that the symbol does not exist in the symbol table*/
                insertSymbole(label, symboletable, arraysymbolesize, arraysymbolelast, CODE, IC); /*inserting the new symbol*/
            
            else if((*symboletable)[symboleindex].external) /*symbol found and its already exist as extern*/
            {
                printf("Line %d: label name already set as external\n", *(linenumber)); 
                return 1;
            }
            else if((*symboletable)[symboleindex].code || (*symboletable)[symboleindex].data) /*symbol found but its already exist*/
            {
                printf("Line %d: label name already set\n", *(linenumber));  
                return 1;
            }
            else /*updating symbole to be regular label*/
            {
                (*symboletable)[symboleindex].code = CODE;
                (*symboletable)[symboleindex].value = *IC;
            }
            
            if(oper)
                removeSpaces(oper); /*moving "white chars" for the line*/

            (*IC) = (*IC) + instructionCounter(oper) + 1; /*updating the number of lines needed for this line*/
        } 
    }
    else if(token[0] == '.') /*analyze .string/.data/.entry/.extern without labale*/
    {
        if(!strcmp(token, string))/*analyze string without labales*/
        {
            token = strtok(NULL, endline);
            fixedstring = moveTabAndSpace(token); /*moving "white chars" for the line*/
            (*DC) = (*DC) + strlen(fixedstring) - 1; /*updating the number of lines needed for this line*/
        }
        else if(!strcmp(token, data))/*analyze data without labales*/
        {
            token = strtok(NULL, endline);
            commaammount = commaCounter(token);  /*counting commas*/
            (*DC) = (*DC) + commaammount + 1; /*updating the number of lines needed for this line*/
        }
        else if(!strcmp(token, entry))
        {
            token = strtok(NULL, space);
            symboleindex = symboleTableSearch(token, symboletable, arraysymbolelast); /*searching the symbole through symbole table*/

            if(symboleindex == -1) /*case that the symbol does not exist in the symbol table*/
                insertSymbole(token, symboletable, arraysymbolesize, arraysymbolelast, ENTRY, IC); /*insert new symbol*/

            else if((*symboletable)[symboleindex].external)
            {
                printf("Line %d: lable name error - already exist as external label\n", *(linenumber)); /*symbol found and its already exist as extern*/
                return 1;
            }
            else
                (*symboletable)[symboleindex].entry = ENTRY; /*updating the exis symbol to be entry*/
        }
        else /*(!strcmp (token, externi)*/
        {
            token = strtok(NULL, space);
            symboleindex = symboleTableSearch(token, symboletable, arraysymbolelast); /*searching the symbole through symbole table*/

            if(symboleindex == -1) /*case that the symbol does not exist in the symbol table*/
                insertSymbole(token, symboletable, arraysymbolesize, arraysymbolelast, EXTERN, &extenaladdress); /*insert new symbol*/

            else if((*symboletable)[symboleindex].entry)
            {
                printf("Line %d: lable name error - already exist as entry label\n", *(linenumber)); /*symbol found and its already exist as entry*/
                return 1;
            } 
        }    
    }
    else
    {
        token = strtok(NULL, endline);
        if(token)
            removeSpaces(token); /*moving any spaces from the line*/   

        (*IC) = (*IC) + instructionCounter(token) + 1; /*updating the number of lines needed for this line*/
    }
    return 0;
}


int lineAnalyze(char line[], Operation operdata[], int * linenumber, int * IC, int *DC, Symbol ** symboletable, int * arraysymbolesize, int * arraysymbolelast) 
{
    char linecpy[MAXLENGTH];
    char * fixed = NULL;
    int syntexflag = 0, symboleflag = 0;

    strcpy(linecpy, line);
    fixed = moveTabAndSpace(linecpy);

    if( fixed[0] == '\0' || fixed == NULL || fixed[0] == ';' || fixed[0] == '\n') /*empty sentence or comment or last line*/
        return 0;

    syntexflag = syntexCheck(fixed, operdata, linenumber); /*checking syntex of the line*/

    if (! syntexflag) /*in case the syntex is ok, we will count the number of words nedded for that line*/
       symboleflag = countInstructionAndSymboles(line, linenumber, IC, DC, symboletable, arraysymbolesize, arraysymbolelast);
    
    return (symboleflag + syntexflag); /*case both are 0 - the syntax is ok*/
}

void addValuesToDataTypes(Symbol ** symboletable, int * IC, int * arraysymbolelast)
{
    int i;

    for( i = 0; i < *arraysymbolelast+1; i++) /*running through the symbol table*/
    {
        if((*symboletable)[i].data)
        { 
            (*symboletable)[i].value = (*symboletable)[i].value + *(IC); /*adding the IC value the locate them in the end of the instruction*/
        }
    }
}

int checkMAxLength(FILE * ptr, int * linenumber)
{
    char c = 0;
    int counter = 0;
    fpos_t tempptr;

    fgetpos(ptr, &tempptr); /*saving the current position of the pointer in the file*/

    while( (c != '\n') && (c != EOF)) /* running through the line and counting the chars*/
    {
        ++counter;
        c = fgetc(ptr);
    }

    if(counter >= MAXLENGTH) /*case the line is greater then the max possible line (80 chars)*/
    {
        c = fgetc(ptr);
        printf("Line %d: Line limit of 80 digits exist - current line is too long\n", *(linenumber));
        return 1;
    }
    fsetpos(ptr, &tempptr); /*case that the line in less then 80 - returning to original position (start of the line)*/
    return 0;
}

int transitionNumOne(char filename[], Symbol ** symboletable, Operation operdata[], int * IC, int * DC, int * arraysymbolesize, int * arraysymbolelast)
{        
    char buff[MAXLENGTH], am[] = ".am", temp[MAXLENGTH];
    int linenumber = 1, errcounter = 0;
    FILE * fd;

    strcpy(temp, filename);

    if ( ! (fd = fopen(strcat(temp, am), READ)) ) /*opening the file to read*/
    {
        fprintf(stderr, "cannot open file\n");
        exit(0);
    }

    while (!feof(fd)) /*running through the entire file, analyzing line by line*/
    {

        if( checkMAxLength(fd, &linenumber)) /*case that the line is greater then 80 chars*/
            errcounter = errcounter + 1; 

        else
        {
            fgets(buff, MAXLENGTH, fd); /*get the next line*/
            errcounter = errcounter + lineAnalyze(buff, operdata, &linenumber, IC, DC, symboletable, arraysymbolesize, arraysymbolelast); /*analyze the line*/
            ++linenumber; /*updating the line number*/ 
            buff[0] = '\0';
        }
    }

    addValuesToDataTypes(symboletable, IC, arraysymbolelast);

    fclose(fd);
    
    if(errcounter) /*an error found during syntax check*/
        remove(temp);
    else if(*IC + *DC > MAXMEMSPACE) /*Make sure the code size doesn't exceed the memory*/
    {
        remove(temp);
        errcounter = 1;
        printf("Error: Memory abnormality\n");
    }

    return errcounter;
}
