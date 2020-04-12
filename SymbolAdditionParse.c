

#include <stdio.h>
#include <stdlib.h>
#include "SymbolAdditionParse.h"
#include "dataStorage.h"


int symbolMacroAddition(char** line,int numOfWords ,int linenum,sympointer *headsym,sympointer *currentsym)
{

    char** ptr = line;
    int intcontrol = MACROFLAGOFF;
    int control = MACROFLAGOFF;
    int strValue = MACROFLAGOFF;
    sympointer currentsymbol = getCurrentSymbol(*headsym);

    if(*line == NULL)/*NULL check for the first word of line[][]*/
    {
        fprintf(stderr,"Null pointer at %d\n",linenum);
        return FAILURE;
    }
    if (strcmp(*ptr,".define") == 0) /*first word is a .define*/
    {
        if (numOfWords == LONGMACRO) /*.define sz = 2,*ptr =".define"*/
        {
            intcontrol = checkNum(&strValue,*(ptr+3),*headsym);
            if(intcontrol == FAILURE)
            {
                fprintf(stderr,"Illegal value found at line %d\n",linenum);
            }
            control = addsymbol(*(ptr+MACROINDEXONE), MACRO, headsym, &currentsymbol, strValue);
            if (control == FAILURE) {
                return FAILURE;
            } else
                return SUCCESS;
        } else if (numOfWords == SHORTMACRO) /* define sz 2 */
        {
            intcontrol = checkNum(&strValue,*(ptr+3),*headsym);
            if(intcontrol == FAILURE)
            {
                fprintf(stderr,"Illegal value found at line %d\n",linenum);
            }            control = addsymbol(*(ptr+MACROINDEXONE), MACRO, headsym, &currentsymbol, strValue);
            if (control == FAILURE) {
                return FAILURE;
            } else
                return SUCCESS;
        }
    }
    else if(isLabel(*ptr) == FAILURE)
    {
        return FAILURE;
    }
    return FAILURE; /*no condition was filled ==>not a macro line*/
}

/*
 * checks if a string is a label
 */
int isLabel(char* string)
{
    char* ptr = string;

    while(*ptr != 0)
    {
        if(*ptr == ':' && *(ptr+MACROINDEXONE) == 0)/*if : is the last char in the word then it's a label!*/
            return SUCCESS;
        if ( (*ptr > 122 || ( *ptr > 90 && *ptr < 96 )|| (*ptr < 65 && *ptr > 57) || *ptr < 48 ) ) /*if char is not a number or letter */
        {
            return FAILURE;
        }
        ptr++;
    }
    return FAILURE;
}
/*
 * returns a string with the label's name, LABEL: will return LABEL.
 * using int as a return type allows me to report for errors.
 */
int extractLabelName(char* label,char** returnValue)
{
    int count = MACROZERO;
    char* ptr = label; /*pass on string*/

    if(*returnValue == NULL)
    {
        return FAILURE;
    }
    while(*ptr != ':')
    {
               *(*returnValue+count) = *ptr;
               count++;
               ptr++;
    }
    *(*returnValue+count) = '\0';
    return SUCCESS;

}
