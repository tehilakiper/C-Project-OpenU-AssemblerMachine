#include "data.h"

void writeAdditionalOperandsWords(const Operation *op, AddrMethodsOptions active, char *value);
Bool writeOperationBinary(char *operationName, char *args)
{
    const Operation *op = getOperationByName(operationName);
    char *first, *second;
    AddrMethodsOptions active[2] = {{0, 0, 0}, {0, 0, 0}};
    first = strtok(args, ", \t\n\f\r");
    second = strtok(NULL, ", \t\n\f\r");
    

    if (first && second && (detectOperandType(first, active, 0) && detectOperandType(second, active, 1)))
    {

        writeSecondWord(first, second, active, op);
        writeAdditionalOperandsWords(op, active[0], first);
        writeAdditionalOperandsWords(op, active[1], second);
	if (!active[0].reg && active[1].reg)
        writeRegOperandWord(second);
    }
    else if (!second && first && detectOperandType(first, active, 1))
    {
        second = first;
        writeSecondWord(first, second, active, op);
        writeAdditionalOperandsWords(op, active[1], second);
    }
    else if (!first && !second ){
	writeSecondWord(first, second, active, op);
        return True;
	}
    else
        return False;

    return True;
}

void writeAdditionalOperandsWords(const Operation *op, AddrMethodsOptions active, char *value)
{

    if (active.direct)
        writeDirectOperandWord(value);
    else if (active.immediate)
        writeImmediateOperandWord(value);
    
}

Bool writeDataInstruction(char *token)
{
    int num;
    while (token != NULL)
    {
        num = atoi(token);
        addWord( num , Data);
        token = strtok(NULL, ", \t\n\f\r");
    }
    return True;
}

Bool writeStringInstruction(char *s)
{
    char *start = strchr(s, '\"');
    int i, len;
    start++;
    len = strlen(start);
    for (i = 0; i < len - 1; i++)
        addWord(start[i] , Data);

    addWord( A | '\0', Data);/***/
    return True;
}

void writeSecondWord(char *first, char *second, AddrMethodsOptions active[2], const Operation *op)
{
    unsigned secondWord = A | (op->optcode << 5);
    unsigned regWord = A ;
    if (first && active[0].reg )
        secondWord = secondWord | (active[0].reg ? (REGISTER_DIRECT_ADDR << 9) : 0);
    else if (first && (active[0].direct || active[0].immediate))
        secondWord = secondWord | (active[0].direct ? (DIRECT_ADDR << 9) : (IMMEDIATE_ADDR << 9));
    if (second && active[1].reg )
        secondWord = secondWord | (REGISTER_DIRECT_ADDR << 2);
    else if (second && (active[1].direct || active[1].immediate))
        secondWord = secondWord | (active[1].direct ? (DIRECT_ADDR << 2): (IMMEDIATE_ADDR << 2));

    if(!first && !second)
	secondWord = secondWord;

    addWord(secondWord, Code);


    if (first && active[0].reg  && second && active[1].reg){

	regWord = regWord | getRegisteryNumber(first) << 7 | getRegisteryNumber(second) << 2;

	addWord(regWord, Code);

	}
    else if (first && active[0].reg && !active[1].reg ){

	regWord = regWord | getRegisteryNumber(first) << 7;

   	addWord(regWord, Code);	
	
	}

}



void writeDirectOperandWord(char *labelName)
{
    unsigned value = 0;
    if (isExternal(labelName))
    {
        value = getIC();
        addWord( E | 0, Code);
       
        updateExtPositionData(labelName, value);
    }

    else
    {
        value = getSymbolAddress(labelName);
        addWord( R | (value) << 2, Code);
        
    }
}

void writeImmediateOperandWord(char *n)
{
    
    addWord( A | atoi(n) << 2, Code);
}

void writeRegOperandWord(char *n)
{
   
    addWord( A | getRegisteryNumber(n) << 7, Code);
}

Bool detectOperandType(char *operand, AddrMethodsOptions active[2], int type)
{

    if (isRegistery(operand))
        active[type].reg = 1;
    else if (isValidImmediateParamter(operand))
        active[type].immediate = 1;
    
    else
    {

        if (isSymbolExist(operand))
        {

            if (isEntry(operand) && !isNonEmptyEntry(operand))
                return yieldError(entryDeclaredButNotDefined);

            active[type].direct = 1;
        }
        else
            return yieldError(labelNotExist);
    }
    return True;
}



