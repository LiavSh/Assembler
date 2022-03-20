# C Project: 20-bit Assembler
## About
This project is an assignment in a C programming course (20465) at the Open university. The purpose of this project is to build an
assembler for 16-instruction asssembly language, for an imaginary 20-bit CPU and 20kb of memory size.
This project was done by Liav Shitrit.

## Source structure (Modules)
1. `assemblermain` - main function definition, argv & argc processing, single file processing.
2. `precompiler` - contains functions for processing a single line and generating a .am file with spread macro lines.
3. `transition1` - contains functions for processing a single line in the first pass.
4. `transition2` - contains function for processing a single line in second pass by using symbols located in the first pass.
5. `common` - contains useful function for processing code.

## Usage
1. run `make`
2. run `./assembler file1 file2 file3 ...`

The assembler will start the processing of each file (specify WITHOUT extension!). No support for double-quoting file names.

## Computer and Language Structure

#### Computer Structure
In our imaginary computer the processor contains 16 general registers: r0, r1, r2 ... r15.

The size of each register is 20 bits, the least significant (LSB) bit will be bit number 0, and the most significant bit (MSB)

will be number 19.

The memory size of the computer is 8192 cells (represented by 0-8191), and each cell is 20 bits in size.

The computer supports integer numbers and also supports characters represented by ASCII code.


#### Word and Sentence Structure
There are 4 kinds of sentences the assembler knows:

1. Empty Sentence - A line contains only whitespaces.

2. Comment Sentence - A line that starts with `;`.

3. Instruction Sentence - Variables assignment and declaration.

4. Command Sentence - Creates an action for the machine to execute upon running the program.

Usage of labels is optional. A label is any word (reserved words not allowed) which is declared at the beginning of the sentence and 

ends with `:`. For example `myLabel:`.

Line maximum length is 80. 

Each computer instruction contains between 1-6 words, depending on the addressing method. 


#### Addressing methods
There are 4 different addressing methods:

1. **Immediate** - The instruction operand will be an immediate number.

2. **Direct** - The instruction operand will be a label defined in the text or imported from an external file.

3. **Index Direct** - The instruction operand will be a label defined in the text or imported from an external file, the labels will

receive in parentheses a number of reference register.

4. **Register direct** - The instruction operand will be one of the system registers.


#### Command Sentence

Command Sentence may or may not start with a label. Valid commands are: 

**mov** - copies origin operand to destination

**cmp** - performs comparison between 2 operands

**add** - destination operand receives the addition result of origin and destination

**sub** - destination operand receives the subtraction result of origin and destination

**lea** - load effective address

**clr** - clears operand

**not** - logical not, reverses all bits in operand

**inc** - increments operand's content by 1

**dec** - decrements operand's content by 1

**jmp** - jumps to instruction in destination operand

**bne** - branch if not equal to zero

**jsr** - calls a subroutine

**red** - reads a char from stdin

**prn** - prints char into stdout

**rts** - returns from subroutine

**stop** - stops the program


## The Two Pass Algorithm
Because the file may contain a number of macro instructions, in the first transition all the different macro instructions that 

appear in the text will be rewritten (if exist).

After performing the first step, the next transition will be performed, in which errors (syntax) as well as invalid commands will

be detected.

Also, at this point, data regarding the various labels will be collected and this data will be saved in a symbol table 

for later use.

Then (assuming no errors are detected), the second transition will be made in which all the instructions received will be

translated into the assembly language of the imaginary computer.

At the end of the proccess, assembler will output 4 files:

1. **.am** file: Will contain the instructions after writing macros.

2. **.ob** file: Will contain the instructions in the assembly language.

3. **.ent** file: Will contain the labels classified as ent.

4. **.ext** file: Will contain the labels classified as ext (externals).
