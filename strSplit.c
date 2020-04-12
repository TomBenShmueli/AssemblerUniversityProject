#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strSplit.h"


/* 
This function takes an address of a pointer to a file, and and address to a list of chars,
and copys a line from the file to the list of chars.

input: address to a pointer of a file, address to a list of chars the size of 82 atleast.
output: 0 - ended succesfully, -1 - got to EOF, 1 - got to maximum size of a line without new line.


 */
int getLine(FILE **f, char (*line)[])
{
    int i = 0;
    int c = 0;

    while(i <80 && (c = getc(*f)) != EOF)
    {
        (*line)[i] = c;
        if((char)c == '\n')
        {
            i = i+1;
            break;
        }
        else
        {
            i = i+1;
        }
    }
    (*line)[(i)] = '\0';
    if(c == EOF)
    {
        return -1;
    }
    if(i == 80)
    {
        if((char)c == '\n')
        {
            return 0;
        }
        else if((c = getc(*f)) == EOF)
        {
            return -1;
        }
        else if((char)c == '\n')
        {
            (*line)[(i)] = '\n';
            i = i+1;
            (*line)[(i)] = '\0';

            if((c = getc(*f)) == EOF)
            {
                return -1;
            }
            else
            {
                c = ungetc(c,*f);
            }
            return 0;
        }
        else
        {
            c = ungetc(c,*f);
        }

        return 1;
    }
    return 0;
}


/*
This function takes an address to a list of strings, and and a list of chars,
splits the list of chars by given rules and copys the words to the list of strings (using malloc).

input: address to a list of strings - should be NULL, address to a list of chars containing a line from a file.
output: -1 - got denied requesting storage, 0 or above - number of words created.


 */
int strSplit(char ***newLine, char *line)
{
    int i = 0, j = 0, m =0, t = 0;
    int numOfWords = 0;
    char word[82];
    int insideWord = 1;

    /*-----------------FIND NUMBER OF WORDS IN A LINE----------------------*/
    if(line[i] == ' ' || line[i] == '\t' || line[i] == ',' || line[i] == '[' || line[i] == ']' || line[i] == '=' || line[i] == '"' || line[i] == '\n')
    {
        insideWord = 0;
    }
    else
    {
        numOfWords++;
    }

    for(; i < 82 && line[i] != '\0'; i++)
    {
        if(insideWord == 0)
        {
            if(line[i] != ' ' && line[i] != '\t' && line[i] != ',' && line[i] != '[' && line[i] != ']' && line[i] != '=' && line[i] != '"' && line[i] != '\n')
            {
                insideWord = 1;
                numOfWords++;
            }
        }
        else
        {
            if(line[i] == ' ' || line[i] == '\t' || line[i] == ',' || line[i] == '[' || line[i] == ']' || line[i] == '=' || line[i] == '"' || line[i] == '\n')
            {
                insideWord = 0;
            }
        }
        if(line[i] == ',' || line[i] == '[' || line[i] == ']' || line[i] == '=' || line[i] == '"' || line[i] == '\n')
        {
            numOfWords++;
            if(line[i] == '"')
            {
                t = findLastQuotationMarks(line);
                if(t == i)
                {
                    numOfWords++;
                    while(line[i] != '\0' && line[i] != '\n')
                    {
                        i++;
                    }
                    if(line[i] == '\n')
                    {
                        numOfWords++;
                    }
                    break;
                }
                else
                {
                    if((t -i) == 1)
                    {
                        numOfWords++;
                        i++;
                    }
                    else
                    {
                        numOfWords +=2;
                        i = i + (t -i);
                    }
                }
            }
        }
    }

    *newLine = (char **)malloc(sizeof(char *)*numOfWords);
    if(*newLine == NULL)
    {
        return -1;
    }

    /*-----------------CREATE A LIST OF WORDS FROM THE LINE----------------------*/
    i = 0;
    j = 0;
    for(; i < numOfWords; i++)
    {
        while((line[j] == '\t' || line[j] == ' ')&& line[j] != '\0')
        {
            j++;
        }
        for(m = 0; j < 82 && line[j] != '\0'; j++)
        {
            if(line[j] != '\t' && line[j] != ' ')
            {
                if(line[j] == ',' || line[j] == '[' || line[j] == ']' || line[j] == '=' || line[j] == '"' || line[j] == '\n')
                {
                    break;
                }
                word[m] = line[j];
                m++;
            }
            else
            {
                break;
            }
        }
        word[m] = '\0';

        while((line[j] == '\t' || line[j] == ' ')&& line[j] != '\0')
        {
            j++;
        }

        if(m != 0)
        {
            (*newLine)[i] = (char *)malloc(sizeof(char)*(m+1));
            if((*newLine)[i] == NULL)
            {
                freeSplittedLine(newLine, i);
                return -1;
            }
            strncpy((*newLine)[i], word,m);
            (*newLine)[i][m] = '\0';
        }

        if(line[j] == ',' || line[j] == '[' || line[j] == ']' || line[j] == '=' || line[j] == '"' || line[j] == '\n')
        {
            if(m != 0)
            {
                i++;
                m = 0;
            }

            while(line[j] == ',' || line[j] == '[' || line[j] == ']' || line[j] == '=' || line[j] == '"' || line[j] == '\n')
            {
                word[0] = line[j];
                word[1] = '\0';
                (*newLine)[i] = (char *)malloc(sizeof(char)*2);
                if((*newLine)[i] == NULL)
                {
                    freeSplittedLine(newLine, i);
                    return -1;
                }
                strncpy((*newLine)[i], word,2);
                if(line[j] == '"')
                {
                    t = findLastQuotationMarks(line);

                    if(t == j)
                    {
                        j++;
                        i++;
                        while(line[j] != '\0' && line[j] != '\n')
                        {
                            word[m] = line[j];
                            j++;
                            m++;
                        }
                        word[m] = '\0';
                        if(m != 0)
                        {
                            (*newLine)[i] = (char *)malloc(sizeof(char)*(m+1));
                            if((*newLine)[i] == NULL)
                            {
                                freeSplittedLine(newLine, i);
                                return -1;
                            }
                            strncpy((*newLine)[i], word,m);
                            (*newLine)[i][m] = '\0';
                            i++;
                        }
                        if(line[j] == '\n')
                        {
                            (*newLine)[i] = (char *)malloc(sizeof(char)*2);
                            if((*newLine)[i] == NULL)
                            {
                                freeSplittedLine(newLine, i);
                                return -1;
                            }
                            (*newLine)[i][0] = '\n';
                            (*newLine)[i][1] = '\0';
                        }
                        return numOfWords;
                    }

                    j++;
                    for(; j < t; m++, j++)
                    {
                        word[m] = line[j];
                    }
                    word[m] = '\0';
                    if(m != 0)
                    {
                        i++;
                        (*newLine)[i] = (char *)malloc(sizeof(char)*(m+1));
                        if((*newLine)[i] == NULL)
                        {
                            freeSplittedLine(newLine, i);
                            return -1;
                        }
                        strncpy((*newLine)[i], word,m);
                        (*newLine)[i][m] = '\0';
                    }
                    i++;
                    word[0] = line[j];
                    word[1] = '\0';
                    (*newLine)[i] = (char *)malloc(sizeof(char)*2);
                    if((*newLine)[i] == NULL)
                    {
                        freeSplittedLine(newLine, i);
                        return -1;
                    }
                    strncpy((*newLine)[i], word,2);
                    j++;

                }
                else
                {
                    j++;
                }

                while((line[j] == '\t' || line[j] == ' ' )&& line[j] != '\0')
                {
                    j++;
                }
                i++;
            }
            i--;
        }
    }

    return numOfWords;
}

int findLastQuotationMarks(char line[])
{
    int i = 0, cellNum = 0;

    while(line[i] != '\0')
    {
        if(line[i] == '"')
        {
            cellNum = i;
        }
        i++;
    }
    return cellNum;

}

/*
This function gets a list of pointers to strings and frees the active pointers by given int.

input: list of pointers that needs to get free, num of pointers that go used in the list.

*/
void freeSplittedLine(char ***line, int numOfCells)
{
    int i = 0;

    for(; i<numOfCells; i++)
    {
        if((*line)[i] != NULL)
        {
            free((*line)[i]);
            (*line)[i] = NULL;
        }
    }

    free((*line));
    (*line) = NULL;
}

