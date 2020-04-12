#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntexCheck.h"


int SyntaxCheck(char **splittedLine, int numOfWords, int lineCounter, int lineExtractResualt)
{
    int resualt = SUCCESS;
    int runner = 0;
    int checker = 0;
    int flag = 0;
    int opcodeType = 0;

    if(lineExtractResualt == 1)
    {
        fprintf(stderr,"File %s: Line is bigger than allowed on line: %d.\n", errorfilename,lineCounter);
        resualt = FAILURE;
    }
    if(numOfWords == 0)
    {
        return resualt;
    }
    if(numOfWords < 0)
    {
        fprintf(stderr,"File %s: Illegal syntax on line  %d.\n", errorfilename,lineCounter);
        return FAILURE;
    }
    if(splittedLine[0][0] == ';')
    {
        return resualt;
    }

    if(strcmp(splittedLine[0],"\n") == 0)
    {
        return resualt;
    }

    if(strcmp(splittedLine[0],".define") == 0)
    {
        runner = 1;
        if(numOfWords < 4)
        {
            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
            return FAILURE;
        }
        while(runner < numOfWords)
        {
            if(runner == 1 && checkName(splittedLine[runner]) == FAILURE)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                resualt = FAILURE;
            }
            if(runner == 2 && strcmp(splittedLine[runner],"=") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            if(runner == 3 && checkNumber(splittedLine[runner]) == FAILURE)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            if(runner >= 4 && strcmp(splittedLine[runner],"\n") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            runner++;
        }
        return resualt;
    }
    if(isFirstWordSymbolConfiguration(splittedLine[0]) == SUCCESS)
    {
        if(checkConfigName(splittedLine[0]) != SUCCESS)
        {
            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
            resualt = FAILURE;
        }
        runner = 1;
        if(numOfWords < 2)
        {
            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
            return FAILURE;
        }
    }
    if(strcmp(splittedLine[runner],".data") == 0)
    {
        runner++;
        checker = 0;
        while(runner < numOfWords)
        {
            if(checker >= 1 && (checker%2) == 1 && strcmp(splittedLine[runner],"\n") == 0)
            {
                checker++;
                runner++;
                continue;
            }
            else if((checker%2) == 0 && checkNumber(splittedLine[runner]) == FAILURE && checkName(splittedLine[runner]) == FAILURE)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            if((checker%2) == 1 && strcmp(splittedLine[runner],",") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            checker++;
            runner++;
        }
        if(checker == 0)
        {
            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
            return FAILURE;
        }
        return resualt;
    }
    if(strcmp(splittedLine[runner],".string") == 0)
    {
        checker = 0;
        runner++;
        while(runner < numOfWords)
        {
            if(checker == 0 && strcmp(splittedLine[runner],"\"") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            if(checker == 1 && strcmp(splittedLine[runner],"\n") == 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            if(checker == 1 && strcmp(splittedLine[runner],"\"") == 0)
            {
                flag = 1;
            }
            if(flag == 1 && checker >= 2 && strcmp(splittedLine[runner],"\n") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            if(flag == 0 && checker == 2 && strcmp(splittedLine[runner],"\"") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            if(flag == 0 && checker >= 3 && strcmp(splittedLine[runner],"\n") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            runner++;
            checker++;
        }
        if(checker <= 1)
        {
            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
            return FAILURE;
        }
        return resualt;
    }

    if(strcmp(splittedLine[runner],".extern") == 0 || strcmp(splittedLine[runner],".entry") == 0)
    {
        runner++;
        checker = 0;
        while(runner < numOfWords)
        {
            if(checker == 0 && checkName(splittedLine[runner]) == FAILURE)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }

            if(checker >= 1 && strcmp(splittedLine[runner],"\n") != 0)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            runner++;
            checker++;
        }
        if(checker == 0)
        {
            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
            return FAILURE;
        }
        return resualt;
    }

    opcodeType = isWordOpcode(splittedLine[runner]);
    runner++;

    switch (opcodeType)
    {
        case CMP:
        case MOV:
        case ADD:
        case SUB:
            checker = 0;
            flag = 0;
            while(runner < numOfWords && strcmp(splittedLine[runner],",") != 0)
            {
                if(flag == 101 && checker >= 1 && strcmp(splittedLine[runner],",") != 0)
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }

                if(checker == 0)
                {
                    if(isWordRegister(splittedLine[runner]) == -1 && checkOpcodeDiracetAddressing(splittedLine[runner]) ==  FAILURE)
                    {
                        if(checkName(splittedLine[runner]) == FAILURE)
                        {
                            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                            return FAILURE;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else
                    {
                        flag = 101;
                    }
                }


                if(flag == 1 && checker == 1)
                {
                    if(strcmp(splittedLine[runner],",") == 0)
                    {
                        flag = 101;
                    }
                    else if(strcmp(splittedLine[runner],"[") == 0)
                    {
                        flag = 2;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                if(flag == 2 && checker == 2)
                {
                    if(checkName(splittedLine[runner]) == FAILURE && checkNumber(splittedLine[runner]) == FAILURE)
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                    flag = 3;
                }

                if(flag == 3 && checker == 3)
                {
                    if(strcmp(splittedLine[runner],"]") == 0)
                    {
                        flag = 101;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                runner++;
                checker++;
            }
            if(runner == numOfWords)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            runner++;
            checker = 0;
            flag = 0;
            if(opcodeType == SUB || opcodeType == ADD || opcodeType == MOV)
            {
                if(runner < numOfWords)
                {
                    if(splittedLine[runner][0] == '#')
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }
            }
            while(runner < numOfWords)
            {
                if(flag == 101 && checker >= 1 && strcmp(splittedLine[runner],"\n") != 0)
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }

                if(checker == 0)
                {
                    if(isWordRegister(splittedLine[runner]) == -1 && checkOpcodeDiracetAddressing(splittedLine[runner]) ==  FAILURE)
                    {
                        if(checkName(splittedLine[runner]) == FAILURE)
                        {
                            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                            return FAILURE;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else
                    {
                        flag = 101;
                    }
                }


                if(flag == 1 && checker == 1)
                {
                    if(strcmp(splittedLine[runner],"\n") == 0)
                    {
                        flag = 101;
                    }
                    else if(strcmp(splittedLine[runner],"[") == 0)
                    {
                        flag = 2;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                if(flag == 2 && checker == 2)
                {
                    if(checkName(splittedLine[runner]) == FAILURE && checkNumber(splittedLine[runner]) == FAILURE)
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                    flag = 3;
                }

                if(flag == 3 && checker == 3)
                {
                    if(strcmp(splittedLine[runner],"]") == 0)
                    {
                        flag = 101;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                runner++;
                checker++;
            }
            if(flag != 101 && flag != 1)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            break;

        case LEA:
            checker = 0;
            flag = 0;
            while(runner < numOfWords && strcmp(splittedLine[runner],",") != 0)
            {
                if(flag == 101 && checker >= 1 && strcmp(splittedLine[runner],",") != 0)
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }

                if(checker == 0)
                {
                    if(checkNumber(splittedLine[runner]) == FAILURE)
                    {
                        if(checkName(splittedLine[runner]) == FAILURE)
                        {
                            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                            return FAILURE;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else
                    {
                        flag = 101;
                    }

                }

                if(flag == 1 && checker == 1)
                {
                    if(strcmp(splittedLine[runner],",") == 0)
                    {
                        flag = 101;
                    }
                    else if(strcmp(splittedLine[runner],"[") == 0)
                    {
                        flag = 2;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                if(flag == 2 && checker == 2)
                {
                    if(checkName(splittedLine[runner]) == FAILURE && checkNumber(splittedLine[runner]) == FAILURE)
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                    flag = 3;
                }

                if(flag == 3 && checker == 3)
                {
                    if(strcmp(splittedLine[runner],"]") == 0)
                    {
                        flag = 101;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                runner++;
                checker++;
            }
            if(runner == numOfWords)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            runner++;
            checker = 0;
            flag = 0;
            while(runner < numOfWords)
            {
                if(flag == 101 && checker >= 1 && strcmp(splittedLine[runner],"\n") != 0)
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }

                if(checker == 0)
                {
                    if(checkNumber(splittedLine[runner]) == FAILURE && isWordRegister(splittedLine[runner]) == -1)
                    {
                        if(checkName(splittedLine[runner]) == FAILURE)
                        {
                            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                            return FAILURE;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else
                    {
                        flag = 101;
                    }

                }

                if(flag == 1 && checker == 1)
                {
                    if(strcmp(splittedLine[runner],"\n") == 0)
                    {
                        flag = 101;
                    }
                    else if(strcmp(splittedLine[runner],"[") == 0)
                    {
                        flag = 2;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                if(flag == 2 && checker == 2)
                {
                    if(checkName(splittedLine[runner]) == FAILURE && checkNumber(splittedLine[runner]) == FAILURE)
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                    flag = 3;
                }

                if(flag == 3 && checker == 3)
                {
                    if(strcmp(splittedLine[runner],"]") == 0)
                    {
                        flag = 101;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                runner++;
                checker++;
            }
            if(flag != 101 && flag != 1)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }

            break;

        case JMP:
        case BNE:
        case JSR:
            checker = 0;
            flag = 0;
            while(runner < numOfWords)
            {
                if(flag == 101 && checker >= 1 && strcmp(splittedLine[runner],"\n") != 0)
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }

                if(checker == 0)
                {
                    if(isWordRegister(splittedLine[runner]) == -1 &&         checkNumber(splittedLine[runner]) == FAILURE && checkName(splittedLine[runner]) == FAILURE)
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                    else
                    {
                        flag = 101;
                    }
                }

                runner++;
                checker++;
            }
            if(flag != 101)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }

            break;
        case DEC:
        case RED:
        case INC:
        case NOT:
        case CLR:
            if(runner < numOfWords)
            {
                if(splittedLine[runner][0] == '#')
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }
            }
        case PRN:
            checker = 0;
            flag = 0;
            while(runner < numOfWords)
            {
                if(flag == 101 && checker >= 1 && strcmp(splittedLine[runner],"\n") != 0)
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }

                if(checker == 0)
                {
                    if(isWordRegister(splittedLine[runner]) == -1 && checkOpcodeDiracetAddressing(splittedLine[runner]) ==  FAILURE)
                    {
                        if(checkName(splittedLine[runner]) == FAILURE)
                        {
                            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                            return FAILURE;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else
                    {
                        flag = 101;
                    }
                }


                if(flag == 1 && checker == 1)
                {
                    if(strcmp(splittedLine[runner],"\n") == 0)
                    {
                        flag = 101;
                    }
                    else if(strcmp(splittedLine[runner],"[") == 0)
                    {
                        flag = 2;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                if(flag == 2 && checker == 2)
                {
                    if(checkName(splittedLine[runner]) == FAILURE && checkNumber(splittedLine[runner]) == FAILURE)
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                    flag = 3;
                }

                if(flag == 3 && checker == 3)
                {
                    if(strcmp(splittedLine[runner],"]") == 0)
                    {
                        flag = 101;
                    }
                    else
                    {
                        fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                        return FAILURE;
                    }
                }

                runner++;
                checker++;
            }
            if(flag != 101 && flag != 1)
            {
                fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                return FAILURE;
            }
            break;

        case RTS:
        case STOP:
            checker = 0;
            while(runner < numOfWords)
            {
                if(checker >= 0 && strcmp(splittedLine[runner],"\n") != 0)
                {
                    fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
                    return FAILURE;
                }
                runner++;
                checker++;
            }
            break;

        default:
            fprintf(stderr,"File %s: Illegal syntax on line : %d.\n", errorfilename,lineCounter);
            return FAILURE;
    }
    return resualt;
}

int checkOpcodeDiracetAddressing(char* name)
{
    char word[82];
    int i = 1;

    if(name[0] != '#')
    {
        return FAILURE;
    }

    while(name[i] != '\0')
    {
        word[(i-1)] = name[i];
        i++;
    }
    word[(i-1)] = '\0';
    if(word[0] == '\0')
    {
        return FAILURE;
    }
    if(checkNumber(word) == SUCCESS || checkName(word) == SUCCESS)
    {
        return SUCCESS;
    }
    return FAILURE;
}

int checkNumber(char* number)
{
    int runner = 0;
    if(number == NULL)
        return FAILURE;
    if(number[0] == '-' || number[0] == '+')
    {
        runner = 1;
        if(number[runner] == '\0')
        {
            return FAILURE;
        }
    }
    while(number[runner] != '\0')
    {
        if(number[runner] < 48 || number[runner] > 57)
        {
            return FAILURE;
        }
        runner++;
    }
    return SUCCESS;
}

int checkConfigName(char* name)
{
    int runner = 0;
    int resualt = 0;

    while(name[runner] != '\0')
    {
        runner++;
    }
    if(runner == 0)
    {
        return FAILURE;
    }
    runner--;
    if(name[runner] == ':')
    {
        name[runner] = '\0';
        if(name[0] == '\0')
        {
            name[runner] = ':';
            return FAILURE;
        }
        resualt = checkName(name);
        name[runner] = ':';
        return resualt;
    }
    return FAILURE;

}

/*
 * illegal names check
 */
int checkName(char* name)
{

    int stringlen = 0;
    char *pointer = name;
    int runner = 0;

    while(*pointer != '\0') /* get string len */
    {
        pointer++;
        stringlen++;
    }
    if(isWordDirective(name) != -1)
    {
        return FAILURE;
    }
    if(isWordRegister(name) != -1)
    {
        return FAILURE;
    }
    if(isWordOpcode(name) != -1)
    {
        return FAILURE;
    }

    if(stringlen > 31) /* len check*/
    {
        return FAILURE;
    }

    pointer = name; /*repointer for string check*/
    if (pointer == NULL)
    {
        return FAILURE;
    }
    if(*pointer == 0) /*check for null*/
    {
        return FAILURE;
    }
    if( (*pointer < 97 && *pointer > 90) || *pointer < 65 || *pointer > 122)/* checking for letter at the beginning */
    {
        return FAILURE;
    }
    while (pointer)
    {
        if (*pointer == 0) /* end of string */
            break;
        if ( (*pointer > 122 || ( *pointer > 90 && *pointer < 96 )|| (*pointer < 65 && *pointer > 57) || *pointer < 48 ) ) /*if letter is not a number or letter */
        {
            return FAILURE;
        }

        runner++;
        pointer++;
    }
    return SUCCESS;
}

int isWordDirective(char * word)
{
    int runner = 0;

    while(runner < 5)
    {
        if(strcmp(word,directives[runner]) == 0)
        {
            return runner;
        }
        runner++;
    }

    return -1;
}

int isWordOpcode(char * word)
{
    int runner = 0;

    while(runner < 16)
    {
        if(strcmp(word,opcodes[runner]) == 0)
        {
            return runner;
        }
        runner++;
    }

    return -1;
}

int isWordRegister(char * word)
{
    int runner = 0;

    while(runner < 8)
    {
        if(strcmp(word,registers[runner]) == 0)
        {
            return runner;
        }
        runner++;
    }

    return -1;

}

int isFirstWordSymbolConfiguration(char *word)
{
    int i = 0;

    while(word[i] != '\0')
    {
        i++;
    }
    if(i == 0)
    {
        return FAILURE;
    }
    i--;
    if(word[i] == ':')
    {
        return SUCCESS;
    }
    return FAILURE;
}

