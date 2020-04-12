#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbollist.h"
#include "enums.h"
#include "secondRunFunctions.h"
#include "syntexCheck.h"
#include "flags.h"


int registersToBits(char * register1, char * register2, int* output)
{
	int flag = 0, r1 = 0, r2 = 0;
	if(register1 != NULL)
	{
		r1 = isWordRegister(register1);
		
		if(r1 == -1)
		{
			flag++;
			r1 = 0;
		}
	}
	if(register2 != NULL)
	{
		r2 = isWordRegister(register2);
		if(r2 == -1)
		{
			flag++;
			r2 = 0;
		}
	}
	if(flag == 2)
	{
		return FAILURE;
	}
	*output = *output | r1;
	*output <<= 3;
	*output = *output | r2;
	*output <<= 2;
	return SUCCESS;
}

void indexValueToBits(char * value, int * output,sympointer symhead)
{

	if(checkNumber(value) == SUCCESS)
	{
		(*output) = atoi(value);
		(*output) <<= 2;
		return;
	}
	if(getValue(symhead, value, output) == SUCCESS)
	{
		(*output) <<= 2;
	}
	return;
}

int indexToBits(char * index, char * value,int * output, sympointer head, FILE *outFile, int IC, int *externFlag)
{
	int zero = 1, one = 2;
	if(isMacro(value, head) == FAILURE && checkNumber(value) == FAILURE)
	{
		return FAILURE;
	}
	if(isData(index, head) == FAILURE && isString(index, head) == FAILURE && isExtern(index, head) == FAILURE)
	{
		return FAILURE;
	}
	if(getValue(head, index, output) == SUCCESS)
	{
		(*output) <<= 2;
		if(isExtern(value, head) == SUCCESS)
		{
            *externFlag = 1;
		    *output = 0;
			*output = *output | zero;
            if(IC > 999)
            {
                fprintf(outFile,"%s %d\n",index,(IC));
            }
            else
            {
                fprintf(outFile,"%s 0%d\n",index,(IC));
            }
		}
		else
		{
			*output = *output | one;
		}
		return SUCCESS;
	}
	return FAILURE;
}

int instintToBits(char *input, int* output, sympointer head)
{
	int runner = 1;
	char word[82];
	
	do
	{
		word[runner-1] = input[runner];
		runner++;
	}while(input[runner] != '\0');
	word[runner-1] = '\0';
	if(checkNumber(word) == SUCCESS)
	{
		(*output) = atoi(word);
		(*output) <<= 2;
		return SUCCESS;
	}
	if(isMacro(word,head) == SUCCESS)
	{
		if(getValue(head, word, output) == SUCCESS)
		{
			(*output) <<= 2;
			return SUCCESS;
		}
	}
	return FAILURE;
}

int directToBits(char * input, int* output, sympointer head, FILE *outFile, int IC,  int *externFlag)
{
	int zero = 1, one = 2;
	if(isData(input,head) == SUCCESS)
	{
		if(getValue(head, input, output) == SUCCESS)
		{
			(*output) <<= 2;
			*output = *output | one;
			return SUCCESS;
		}
	}
	if(isString(input,head) == SUCCESS)
	{
		if(getValue(head, input, output) == SUCCESS)
		{
			(*output) <<= 2;
			*output = *output | one;
			return SUCCESS;
		}
	}
	if(isExtern(input,head) == SUCCESS)
	{
        *externFlag = 1;
        (*output) = 0;
        *output = *output | zero;
        if(IC > 999)
        {
            fprintf(outFile,"%s %d\n",input,(IC));
        }
        else
        {
            fprintf(outFile,"%s 0%d\n",input,(IC));
        }
        return SUCCESS;
	}
	if(isCode(input,head) == SUCCESS)
	{
		if(getValue(head, input, output) == SUCCESS)
		{
			(*output) <<= 2;
			*output = *output | one;
			return SUCCESS;
		}
	}
	return FAILURE;
}

void sourceOperand(int sourceOperand, int *output)
{
	int four = 16, five = 32;
	switch(sourceOperand)
	{
		case INSTINT:
			return;
		case DIRECT:
			*output = *output | four;
			return;
		case INDEX:
			*output = *output | five;
			return;
		case REGISTER:
			*output = *output | four;
			*output = *output | five;
			return;
		default:
			return;
	}
	return;
}

void destOperand(int destOperand, int *output)
{
	int two = 4, three = 8;
	switch(destOperand)
	{
		case INSTINT:
			return;
		case DIRECT:
			*output = *output | two;
			return;
		case INDEX:
			*output = *output | three;
			return;
		case REGISTER:
			*output = *output | two;
			*output = *output | three;
			return;
		default:
			return;
	}
	return;
}

void opcodeToBits(int opcodeType, int *output)
{
	int six = 64, seven = 128, eight = 256, nine = 512;
	switch (opcodeType)
		{
			case CMP:
				*output = *output | six;
				return;
			case MOV:
				return;
			case ADD:
				*output = *output | seven;
				return;
			case SUB:
				*output = *output | six;
				*output = *output | seven;
				return;
			case LEA:
				*output = *output | seven;
				*output = *output | eight;
				return;
			case JMP:
				*output = *output | six;
				*output = *output | nine;
				return;
			case BNE:
				*output = *output | seven;
				*output = *output | nine;
				return;
			case JSR:
				*output = *output | six;
				*output = *output | eight;
				*output = *output | nine;
				return;
			case DEC:
				*output = *output | nine;
				return;
			case RED:
				*output = *output | six;
				*output = *output | seven;
				*output = *output | nine;
				return;
			case INC:
				*output = *output | six;
				*output = *output | seven;
				*output = *output | eight;
				return;
			case NOT:
				*output = *output | eight;
				return;
			case CLR:
				*output = *output | six;
				*output = *output | eight;
				return;
			case PRN:
				*output = *output | eight;
				*output = *output | nine;
				return;
			case RTS:
				*output = *output | seven;
				*output = *output | eight;
				*output = *output | nine;
				return;
			case STOP:
				*output = *output | six;
				*output = *output | seven;
				*output = *output | eight;
				*output = *output | nine;
				return;
			default:
				return;	
		}
	return;
}

void freeFile(FILE ** f1, FILE ** f2, char** n1, char** n2, char** n3)
{
	if(fclose(*f1) != 0)
	{
		fprintf(stderr,"File %s :Unable to close file, Unexpected behavior may occur." , *n1);
	}
	if(remove(*n1) != 0)
	{
        fprintf(stderr,"File %s :Unable to close file, Unexpected behavior may occur." , *n1);
	}
    if(fclose(*f2) != 0)
    {
        fprintf(stderr,"File %s :Unable to close file, Unexpected behavior may occur." , *n3);
    }
    if(remove(*n3) != 0)
    {
        fprintf(stderr,"File %s :Unable to close file, Unexpected behavior may occur." , *n3);
    }
	free(*n1);
    free(*n2);
    free(*n3);
}

void writeToObFile(FILE * file, int numToConvert, int lineNumber)
{
	int mark1 = 8192;
	int mark2 = 4096;
	int markSum = 0;
	char out[8];
	int index = 0;

	for(;index < 7; index++)
	{
		if((mark1&numToConvert) > 0)
		{
			markSum += 2;
		}
		if((mark2&numToConvert) > 0)
		{
			markSum += 1;
		}

		switch (markSum)
		{
			case 3:
				out[index] = '!';
				break;
			case 2:
				out[index] = '%';
				break;
			case 1:
				out[index] = '#';
				break;
			case 0:
				out[index] = '*';
				break;
			default:
				break;
		}

		markSum = 0;
		mark1 = mark1 >> 2;
		mark2 = mark2 >> 2;

	}
	out[index] = '\0';

	if(lineNumber > 999)
	{
		fprintf(file,"%d\t%s\n",lineNumber ,out);
	}
	else
	{
		fprintf(file,"0%d\t%s\n",lineNumber ,out);
	}
	

}

/*
 * check for entry symbols, return 1 if a matching string/data/extern sym is found, else 0.
 */
int checkExistingEntryValue(sympointer head)
{
    sympointer entryPtr = NULL;/* will point on entry symbols */
    sympointer outerindex = head, innerindex = head;

    int foundmatch = -1;
    /*
     * naive approach
     */
    for(; outerindex != NULL ; outerindex= outerindex->next)
    {
        if(outerindex->type == ENTRY)
        {
            entryPtr = outerindex;
            for(;innerindex != NULL ; innerindex = innerindex->next)
            {
                if( (strcmp(entryPtr->name,innerindex->name) == 0 && innerindex->type == EXTERN ) ||
                    (strcmp(entryPtr->name,innerindex->name) == 0 && innerindex->type == STRING ) ||
                    (strcmp(entryPtr->name,innerindex->name) == 0 && innerindex->type == DATA ))
                {
                    entryPtr->data = innerindex->data;
                    foundmatch = 1;
                    break;
                }
            }
            if(foundmatch == -1)
            {
                fprintf(stderr,"File %s: entry declerations without matching variables declared",errorfilename);
                return FAILURE;
            }
        }
    }
    return SUCCESS;

}

