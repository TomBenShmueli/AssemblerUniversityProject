
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "codeStorage.h"
#include "syntexCheck.h"

/*
 * gets a code line, returns the number of instructions
 */
int storeCode(char** line, int linenum,int numOfWordsParam , sympointer *symhead)
{

    /*
     * values
     */
    int numofwords = numOfWordsParam;
    int index = 0;
    int lastbracketsindex = OFF;
    char* labelname = NULL;
    /*
     * IC calc vars
     */
    int registercount = 0; /*two regs +1 or 1 reg*/
    int indexcount = 0;
    int macrocount = 0; /*direct*/
    int decimalvaluecount = 0; /*direct*/
    int opcodecount = 0; /*opcode count, will be 1 if the line is correct*/
    int unknownlabelcount = 0;

    /*
     * flag vars
     */
    int bracketsflag = CLOSED_BRACKETS;
    int registerflag = OFF;
    int labelcontrol;
    /*
     * curr vars
     */
    char* currentWord = NULL;
    sympointer currentSymbol = getCurrentSymbol(*symhead);

    if( (*line) == NULL )/*NULL pointer check*/
    {
            fprintf(stderr,"File %s: NULL value at line %d",errorfilename ,linenum); /*no return in order to check for more errors */
    }

    if(isLabel(*line)== SUCCESS)/*TEST: opcode args*/
    {
        labelname = (char*)malloc(strlen(*line));
        if (extractLabelName(*line,&labelname) == FAILURE)
        {
            free(labelname);
            return DATA_FAILURE;
        }
        labelcontrol = addsymbol(labelname,CODE,symhead,&currentSymbol,linenum);
        if(labelcontrol == FAILURE)
        {
            fprintf(stderr,"File %s: error adding a symbol to symbol list at line %d\n",errorfilename,linenum);
        }
        free(labelname);

        index++; /*index = 1, currentWord = "opcode" */
        for(;index < numofwords;index++)/*explicit currentWord promotion is more readable imo */
        {
            currentWord = line[index];
            if(index == 1 && isWordOpcode(currentWord) != OFF) /*first word opcode?*/
            {
                opcodecount++;
               continue;
            }
            if(index != 1)
            {
                if(bracketsflag == OPEN_BRACKETS && lastbracketsindex == index-ONEINTERVAL) /* [ VALUE check */
                {
                    if(valueCheck(currentWord,*symhead) == SUCCESS)
                    {
                        decimalvaluecount++;
                        continue;
                    }
                }
                if(bracketsflag == OPEN_BRACKETS && lastbracketsindex == index-TWOINTERVAL) /*] check after [ VALUE */
                {
                    if(strcmp("]",currentWord)==0)
                    {
                        bracketsflag = CLOSED_BRACKETS;
                        lastbracketsindex = 0;
                        continue;
                    }
                }
                if(isMacro(currentWord,*symhead) == SUCCESS)/*define check */
                {
                    macrocount++;
                    continue;
                }
                if(valueCheck(currentWord,*symhead) == SUCCESS)/*#2 or #macro check */
                {
                    decimalvaluecount++;
                    continue;
                }
                if(strcmp("[",currentWord)==0)/* open brackets found [*/
                {
                    bracketsflag = OPEN_BRACKETS;
                    lastbracketsindex = index;
                    continue;
                }
                if(dataCheck(currentWord,*symhead) == SUCCESS) /*data symbol check*/
                {
                    macrocount++;
                    continue;
                }
                if(registerCheck(currentWord) == SUCCESS && registerflag == OFF) /*register found*/
                {
                    registerflag = ON;
                    registercount++;
                    continue;
                }
                if(registerCheck(currentWord) ==SUCCESS && registerflag == ON) /* two registers, no increase*/
                {
                    continue;
                }
                if(*currentWord == ',')/*comma skip*/
                {
                    continue;
                }
                if(strcmp(currentWord,"\n") == 0)/*\n skip*/
                {
                    continue;
                }
                else unknownlabelcount++; /*else unknown label */
            }
        }
    }
    else
    {
         /*index = 0, currentWord = "opcode" */
        for(;index < numofwords;index++)/*ditto as above */
        {
            currentWord = line[index];
            if(index == 0 && isWordOpcode(currentWord) != OFF)
            {
                opcodecount++;
                continue;
            }
            if(index != 0)
            {
                if(bracketsflag == OPEN_BRACKETS && lastbracketsindex == index-ONEINTERVAL)/* [ VALUE check */
                {
                    if(valueCheck(currentWord,*symhead) == SUCCESS)
                    {
                        decimalvaluecount++;
                        continue;
                    }
                }
                if(bracketsflag == OPEN_BRACKETS && lastbracketsindex == index-TWOINTERVAL) /*] check after [ VALUE */
                {
                    if(strcmp("]",currentWord)==0)
                    {
                        bracketsflag = CLOSED_BRACKETS;
                        lastbracketsindex = OFF;
                        continue;
                    }
                }
                if(isMacro(currentWord,*symhead) == SUCCESS)/*define check */
                {
                    macrocount++;
                    continue;
                }
                if(valueCheck(currentWord,*symhead) == SUCCESS)/*#2 or #macro check */
                {
                    decimalvaluecount++;
                    continue;
                }
                if(strcmp("[",currentWord)==0)/* open brackets found [*/
                {
                    bracketsflag = OPEN_BRACKETS;
                    lastbracketsindex = index;
                    continue;
                }
                if(dataCheck(currentWord,*symhead) == SUCCESS) /*data symbol check*/
                {
                    macrocount++;
                    continue;
                }
                if(registerCheck(currentWord) == SUCCESS && registerflag == OFF) /*register found*/
                {
                    registerflag = ON;
                    registercount++;
                    continue;
                }
                if(registerCheck(currentWord) ==SUCCESS && registerflag == ON) /* two registers, no increase*/
                {
                    continue;
                }
                if(*currentWord == ',')
                {
                    continue;
                }
                if(strcmp(currentWord,"\n") == 0)
                {
                    continue;
                }
                else unknownlabelcount++;
            }
        }
    }
    return opcodecount+registercount + indexcount + macrocount +decimalvaluecount + unknownlabelcount; /*1 for basic opcode, rest are counted */
}
/*
 * reg check ,1 for fail 0 for success as usual
 */
int registerCheck(char* string)
{
    int i = 0;
    for(;i<NUMBER_OF_REGISTERS;i++)
    {
        if(strcmp(string,registers[i])==0)
            return SUCCESS;
    }
    return FAILURE;
}

/*
 * checks for #number or #macro, 1 for fail 0 for success
 */
int valueCheck(char* string,sympointer head)
{
    char* ptr = string;
        if(*ptr == '#') {
            ptr++;

            if (isMacro(ptr, head) == SUCCESS) /* #macro*/
            {
                return SUCCESS;
            }
            if(*ptr == '+' || *ptr == '-')
            {
                ptr++;
            }
            while (*ptr != '\0') /*scan string for numeric values*/
            {
                if (*ptr < 48 || *ptr > 57)
                    return FAILURE;
                else ptr++;
            }
            return SUCCESS;
        }
    if (isMacro(ptr, head) == SUCCESS) /* #macro*/
    {
        return SUCCESS;
    }
    if(*ptr == '+' || *ptr == '-') /*sign skip*/
    {
        ptr++;
    }
    while (*ptr != '\0') /*scan string for numeric values*/
    {
        if (*ptr < 48 || *ptr > 57)
            return FAILURE;
        else ptr++;
    }
    return SUCCESS;

}
/*
 * check for data symbols and return SUCCESS/FAILURE
 */
int dataCheck(char* string, sympointer head)
{
    while(head != NULL)
    {
        if(strcmp(string,head->name) == 0 && head->type == DATA)
        {
            return SUCCESS;
        }
        head= head->next;
    }
    return FAILURE;
}