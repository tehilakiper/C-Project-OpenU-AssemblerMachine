#include "data.h"

static const Operation operations[OP_SIZE] = {
    { 0, "mov", {1, 1, 1}, {0, 1, 1}},
    { 1, "cmp", {1, 1, 1}, {1, 1, 1}},
    { 2, "add", {1, 1, 1}, {0, 1, 1}},
    { 3, "sub", {1, 1, 1}, {0, 1, 1}},
    { 4, "not", {0, 0, 0}, {0, 1, 1}},
    { 5, "clr", {0, 0, 0}, {0, 1, 1}},
    { 6, "lea", {0, 1, 0}, {0, 1, 1}},
    { 7, "inc", {0, 0, 0}, {0, 1, 1}},
    { 8, "dec", {0, 0, 0}, {0, 1, 1}},
    { 9, "jmp", {0, 0, 0}, {0, 1, 0}},
    { 10, "bne", {0, 0, 0}, {0, 1, 0}},
    { 11, "red", {0, 0, 0}, {0, 1, 1}},
    { 12, "prn", {0, 0, 0}, {1, 1, 1}},
    { 13, "jsr", {0, 0, 0}, {0, 1, 0}},
    { 14, "rts", {0, 0, 0}, {0, 0, 0}},
    { 15, "stop", {0, 0, 0}, {0, 0, 0}},
};

/* @ Function: Bool isOperationName(char *s)
   @ Arguments: The function gets char *s- the name of the operation it checks if it is valid
   @ Description: The function checks if the operation (char *s) name is valid by using the function
   getOpIndex. Returns true if the operation's name is valid, and false if it isn't.
   */
Bool isOperationName(char *s)
{
    return getOpIndex(s) != -1 ? True : False;
}

/* @ Function: Operation *getOperationByName(char *s)
   @ Arguments: The function gets char *s- the name of the operation that the function returns the information about
   @ Description: The function checks what is the opcode of the operation it got (char *s) using getOpIndex.
   If the operation's opcode is not -1 (which means the operation is valid)
   it returns the information about the operation (from the operations table). Else, it returns NULL.
*/
const Operation *getOperationByName(char *s)
{
    int i = getOpIndex(s);

    if (i != -1)
        return &operations[i];

    return NULL;
}

/* @ Function: Operation *getOperationByIndex(unsigned int i)
   @ Arguments: The function gets unsigned int i which represent the opcode index of the operation
   @ Description: The function checks if i (the opcode index of the operation) is valid- smaller than
   OP_SIZE (which equals to 16- the number of operations).
   If it is valid it returns the information about the operation that the opcode's index i
   represents (from the operations table). Else, it returns NULL.
*/
const Operation *getOperationByIndex(unsigned int i)
{
    return i < OP_SIZE ? &operations[i] : NULL;
}

/* @ Function: char *getOperationName(char *s)
   @ Arguments: The function gets char *s- the name of the operation that the function
   returns its keyword (name)
   @ Description: The function checks if the operation its got (char *s) is a sub string of every
   given operation. If it is, it returns the operation's keyword (name). Else, it returns 0.
*/
const char *getOperationName(char *s)
{
    int i = 0;
    while (i < OP_SIZE)
    {

        if (strstr(operations[i].keyword, s) != NULL)
            return operations[i].keyword;
        i++;
    }
    return 0;
}

/* @ Function: int getOpIndex(char *s)
   @ Arguments: The function gets char *s- the name of the operation it gets it's opcode index
   @ Description: The function compares the operation it got (char *s) with every operation,
   if it is one of the given operations, it returns it's opcode index.
   Else (the operation char *s is not valid), it return -1.
*/
int getOpIndex(char *s)
{
    int i = 0;
    while (i < OP_SIZE)
    {

        if (strcmp(operations[i].keyword, s) == 0)
            return i;
        i++;
    }
    return -1;
}
/* @ Function: Bool isOperationNotStrict(char *s)
   @ Arguments: The function gets char *s- the name of the operation that the function
   checks if it is a non strict given operation.
   @ Description: The function checks if the operation its got (char *s) is a sub string of every
   given operation. If it is, it returns true. Else, it returns false. */
Bool isOperationNotStrict(char *s)
{
    int i = 0;
    while (i < OP_SIZE)
    {

        if (strstr(operations[i].keyword, s) != NULL)
            return True;
        i++;
    }
    return False;
}
