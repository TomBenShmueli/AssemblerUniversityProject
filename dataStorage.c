#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dataStorage.h"
#include "syntexCheck.h"

/*get line from strSplit ==> a line with spaces between each word and legal commands,returns STRINGADDED if successful else FAILURE*/
int storeString(int linenum,char** line,stringpointer *currentstring,sympointer *headsymbol)
{
    /*
     * dec
     */
    char** wordarg;
    char* findvalue;
    char* labelname;
    sympointer currentsym;
    stringpointer tmp;
    int symcontrol;

    /*is this a .string line check*/
    if(isLabel(*line) == SUCCESS)
    {
        if(strcmp(*(line+DATAINDEXONE),".string") != 0) /*check if first word is .string*/
        {
            return STRING_FAILURE;
        }
    }
    else
    {
        if(strcmp(*(line),".string") != 0) /*check if first word is .string*/
        {
            return STRING_FAILURE;
        }
    }

    wordarg = line; /*for sending value variables to functions*/
    findvalue = *line; /*comparision var */
    labelname = NULL; /*will store label name if found*/

    currentsym = getCurrentSymbol(*headsymbol);
    tmp = NULL;
    if(isLabel(findvalue) == SUCCESS) /*case LABEL: .string "word" : first word is a label, adding to symbol list and pointing ptr on string*/
    {
        labelname = (char*)malloc(strlen(*line));
        if (extractLabelName(findvalue, &labelname) == FAILURE)
        {
            free(labelname);
            return STRING_FAILURE;
        }
        else symcontrol = addsymbol(labelname, STRING, headsymbol, &currentsym, linenum);
        free(labelname);
        if (symcontrol == FAILURE)
        {
            fprintf(stderr,"File %s: Unable to add string symbol at line %d\n",errorfilename ,linenum);
        }
        else {

                tmp = addStringNode(linenum, (wordarg)[DATAINDEXTHREE], currentstring);
                if(tmp == NULL)
                    return STRING_FAILURE;
                else {
                    return (strlen((wordarg)[DATAINDEXTHREE])+DATAINDEXONE);
                }
            }
    }
           /* *wordarg->.string *wordarg+1-> "string" */
            tmp = addStringNode(linenum, wordarg[DATAINDEXTWO], currentstring);
                if(tmp == NULL)
                    return STRING_FAILURE;
                else {
                    return (strlen(*(wordarg+DATAINDEXONE))+DATAINDEXONE);
                }

}

/*
 * create and link  string node to list
 */
stringpointer addStringNode(int inaddress, char* stringname, stringpointer* strhead) {

    stringpointer additionptr = *strhead;
    stringpointer tmp = (stringpointer) malloc(sizeof(stringnode));/*mem aloc*/
    if (tmp == NULL)
    {
            fprintf(stderr, "File %s: Unable to create a new string node at line %d.\n",errorfilename ,inaddress);
            return NULL;
    }
    tmp->string = (char*)malloc(strlen(stringname) +ONECHAR);
    if(tmp->string == NULL)
    {
        fprintf(stderr, "File %s: Unable to create a new string node at line %d.\n",errorfilename ,inaddress);
        return NULL;
    }
    strcpy(tmp->string, stringname);/*update new node*/
    tmp->address = inaddress;
    tmp->next = NULL;

    if ((*strhead) == NULL) /*strhead is null,create new node*/
    {
        (*strhead) = tmp;
        return tmp;
    }
    while (additionptr ->next!= NULL)     /*get current node*/
    {
        additionptr = additionptr->next;
    }
    additionptr->next = tmp;
    return additionptr;
}

/*free string list*/
void destroyStringList(stringpointer head)
{
    if(head == NULL)
        return;

    destroyStringList(head->next);
    free(head->string);
    free(head);
}
/*print string list*/
void printStringList(stringpointer head)
{
    if(head == NULL)
        return;
    printf("%s\n",head->string);
    printStringList(head->next);
}

/*
 * store data in list and returns number of added values , failure -1.
 */
int storeData(int linenum,int numofwords ,char** line,datapointer *currentdata, sympointer *headSymbol)
{
    char** wordarg = line; /*for sending value variables to functions*/
    char* findvalue = *line; /*comparision var */
    char* labelname = NULL; /*will store label name if found*/
    sympointer currentsym = getCurrentSymbol(*headSymbol);
    datapointer tmp = NULL;
    int symcontrol;

    /*is this a .data line check*/
    if(isLabel(*line) == SUCCESS)
    {
        if(strcmp(*(line+DATAINDEXONE),".data") != 0) /*check if first word is .data*/
        {
            return DATA_FAILURE;
        }
    }
    else
    {
        if(strcmp(*(line),".data") != 0) /*check if first word is .data*/
        {
            return DATA_FAILURE;
        }
    }

    if(isLabel(findvalue) == SUCCESS) /*case LABEL: .data "v" "v" "v" : first word is a label, add to symlist and extract numeric values*/
    {
        labelname = (char*)malloc(strlen(wordarg[DATAINDEXZERO])+DATAINDEXONE);
        if (extractLabelName(findvalue, &labelname) == FAILURE)
        {
            free(labelname);
            return DATA_FAILURE;
        }

        symcontrol = addsymbol(labelname,DATA, headSymbol, &currentsym, linenum);/*add symbol*/
        free(labelname);

        if (symcontrol == FAILURE)
        {
            fprintf(stderr,"File %s: Unable to add data symbol at line %d\n",errorfilename ,linenum);
        }
        else
        {
            if(*(currentdata) == NULL)/* null head */
            {
                (*currentdata) = addDataNode(linenum, line+DATAINDEXTWO, numofwords ,currentdata,*headSymbol);
                return (*currentdata)->arraylen;
            }
            tmp = addDataNode(linenum, line+DATAINDEXTWO ,numofwords,currentdata,*headSymbol);/*add to tail*/
            if (tmp == NULL)
                return DATA_FAILURE;
            else
                    return (tmp->next)->arraylen;
        }
    }

    else/*case no LABEL found, ditto as above.*/
        {
            if(*(currentdata) == NULL)
            {
                (*currentdata) = addDataNode(linenum, line+DATAINDEXTWO,numofwords,currentdata,*headSymbol);
                if(*(currentdata) == NULL)
                {
                    return FAILURE;
                }
                return (*currentdata)->arraylen;
            }
            tmp = addDataNode(linenum, line+DATAINDEXTWO,numofwords,currentdata,*headSymbol);
            if (tmp == NULL)
                return FAILURE;
            else return (tmp->next)->arraylen;
        }
    return DATA_FAILURE;
}

/*
 * create and link new node
 */
datapointer addDataNode(int inaddress,char** data,int arraylen, datapointer* datahead,sympointer symhead)
{
     /* control and looping variables*/
    int len = 0, index = 0;
    int numcheck;
    char** numbercounter = data;

    datapointer additionptr = *datahead;
    datapointer tmp = (datapointer)malloc(sizeof(datanode));/*mem aloc*/

    for(; index < arraylen - DATAINDEXTWO; numbercounter++ )
    {
        if(checkNumber(*numbercounter) == SUCCESS || isMacro(*numbercounter,symhead) == SUCCESS)
        {
            len++;
        }
        index++;
    }


    if (tmp == NULL) {
        fprintf(stderr, "File %s: Unable to create a new string node at line %d.\n", errorfilename , inaddress);
        return NULL;
    }
    tmp->array = (int*)malloc((len*sizeof(int)));
    for(numbercounter = data,index = 0;  index < arraylen/DATAINDEXTWO &&( (strcmp(*numbercounter,"\n") != 0) && *numbercounter !=NULL) ; numbercounter++ )
    {
        numcheck = checkNum(&(tmp->array)[index],*numbercounter, symhead );
        if(numcheck == SUCCESS)
        {
            index++;
        }
    }

    tmp->address = inaddress;
    tmp->arraylen = len;
    tmp->next = NULL;

    if ((*datahead) == NULL) /*strhead is null,create new node*/
    {
        (*datahead) = tmp;
        return tmp;
    }
    while (additionptr->next != NULL)     /*get current node*/
    {
        additionptr = additionptr->next;
    }
    additionptr->next = tmp;
    return additionptr;
}

/*
 * my own version of atoi w/ error control.
 */
int checkNum(int* num,char* string,sympointer symhead)
{
    /*
     * flags for checks
     */
    int negetiveflag = SIGNFLAGOFF;
    int scanscomplete = INCOMPLETE;
    int getValControl;
    int returnnum = 0;
    char* ptr = string;
    /*
     * null check
     */
    if(string == NULL)
        return FAILURE;
    if(*ptr == '-')
    {
        negetiveflag=SIGNFLAGON;
        ptr++; /* move past - sign*/
    }
    else if (*ptr == '+')
    {
        ptr++; /* move past + sign */
    }
    /*
     * string to int w/ error control unlike atoi.
     */
    do
    {
        if(isMacro(string,symhead) == SUCCESS)/*get value from macro if success*/
        {
            getValControl = getValue(symhead,string, num);
            return getValControl;
        }
        if(*ptr == ',')/*skip if comma*/
        {
            return COMMA_FOUND;
        }
        if (*ptr > 47 && *ptr < 58)
        {
            returnnum += *ptr - 48; /* '1' - 48 = 1 and so on */
            ptr++;

            if(*(ptr) != '\0')/*num has more digits, add 10 and continue*/
            {
                returnnum *= ADDTEN;
                continue;
            }
            else scanscomplete = SCANCOMPLETE;
        }
        else return FAILURE;
    } while(scanscomplete != SCANCOMPLETE);/*until scan complete flag is raised*/

    if(negetiveflag == SIGNFLAGON)
    {
        *num = NEGATIVESIGN*returnnum;
        return SUCCESS;
    }
    else

        *num = returnnum;
    return SUCCESS;
}

/*
 * free data list
 */
void destroyDataList(datapointer head)
{
    if(head == NULL)
        return;

    destroyDataList(head->next);
    free(head->array);
    free(head);
}
/*
 * print data list
 */
void printDataList(datapointer head)
{
    int i;
    int len;
    if(head == NULL)
    {
        printf("\n");
        return;
    }
    i = 0;
    len = head->arraylen;
    for(; i < len ; i++)
    {
        printf("%d\t",head->array[i]);
    }
    printf("\n");
    printDataList(head->next);
}

/*
 * store entry/extern symbols
 */
int storeEntryExtern(int linenum,int numOfWords, char** line, sympointer* headsymbol) {
    sympointer currentsym = getCurrentSymbol(*headsymbol);
    char **lineptr = line;
    int control;


    if (*line == NULL)/*NULL check for the first word of line[][]*/
    {
        fprintf(stderr, "File %s: Null pointer at %d\n", errorfilename ,linenum);
        return FAILURE;
    }
    if(isLabel(*line) == SUCCESS)
    {
        if(numOfWords == DATASENTANCELENLONG)/*valid but has no meaning whatsoever*/
        {
            if (strcmp(*(lineptr+DATAINDEXONE), ".extern") == 0) /*second word is a .extern*/
            {
                    control = addsymbol(*(lineptr + DATAINDEXTWO), EXTERN, headsymbol, &currentsym, linenum);
                    if (control == FAILURE) {
                        return FAILURE;
                    } else
                        return SUCCESS;
            }
            if (strcmp(*(lineptr+DATAINDEXONE), ".entry") == 0) /*second word is a .entry*/
            {
                    control = addsymbol(*(lineptr + DATAINDEXTWO), ENTRY, headsymbol, &currentsym, linenum);
                    if (control == FAILURE) {
                        return FAILURE;
                    } else
                        return SUCCESS;
            }
        }
        return FAILURE;
    }
    else
        if (strcmp(*lineptr, ".extern") == 0) /*first word is a .extern*/
    {
        if (numOfWords == DATAINDEXTHREE) /*.extern VAR + check for corrent number of words*/
        {
            control = addsymbol(*(lineptr + DATAINDEXONE), EXTERN, headsymbol, &currentsym, linenum);
            if (control == FAILURE) {
                return FAILURE;
            } else
                return SUCCESS;
        }
    }
    if (strcmp(*lineptr, ".entry") == 0) /*first word is a .entry*/
    {
        if (numOfWords == DATAINDEXTHREE) /*.entry VAR + check for corrent number of words*/
        {
            control = addsymbol(*(lineptr + DATAINDEXONE), ENTRY, headsymbol, &currentsym, linenum);
            if (control == FAILURE) {
                return FAILURE;
            } else
                return SUCCESS;
        }
    }
    else
       return FAILURE;
    return FAILURE;
}
/*
 * print extern list
 */
void printExternList(sympointer headsymbol)
{

        if(headsymbol == NULL)
            return;
        if(headsymbol->type == EXTERN)
        {
            printSym(headsymbol);
        }
        printExternList(headsymbol->next);

}/*
 * print entry list
 */
void printEntryList(sympointer headsymbol)
{

    if(headsymbol == NULL)
        return;
    if(headsymbol->type == ENTRY)
    {
        printSym(headsymbol);
    }
    printEntryList(headsymbol->next);
}

/*
 * print entry list to file
 */
int printEntryListFile(char* filename,int linenum ,sympointer headsymbol) {
/*
 * creating file at local directory
 */

    FILE *fptr = NULL;
    sympointer  outerindex = headsymbol;
    sympointer indexsymbol = headsymbol;
    int printflag = DATAFLAGOFF;

    if (outerindex == NULL) /*symlist check*/
    {
        fprintf(stderr, "File %s: NULL symbol list head, at line %d", errorfilename ,linenum);
        return FAILURE;
    }
    fptr = fopen(filename, "w"); /*fopen+w flag allows for creating of new files*/
    if (fptr == NULL)
    {
        fprintf(stderr, "File %s: Unable to open file, at line %d", errorfilename,  linenum);
        return FAILURE;
    }
    while(outerindex) /*write to file*/
    {
        if(outerindex->type == ENTRY)/*find entry*/
        {
            while(indexsymbol) /*index to find match*/
            {
                if(strcmp(indexsymbol->name,outerindex->name) == 0 && indexsymbol->type != ENTRY) /*match found*/
                {
                    if(indexsymbol->data < 999) /*print match*/
                    {
                        fprintf(fptr, "%s 0%d\n", indexsymbol->name, indexsymbol->data);
                    }
                    else fprintf(fptr, "%s %d\n", indexsymbol->name, indexsymbol->data);
                    printflag = DATAFLAGON;
                    break;
                }
                indexsymbol = indexsymbol->next;
            }
        }
        indexsymbol = headsymbol;
        outerindex = outerindex->next;
    }
    if(printflag != DATAFLAGON)/*remove if no match found*/
    {
        fclose(fptr);
        remove(filename);
        return FAILURE;
    }
    fclose(fptr);
    return SUCCESS;
}
/*
 * prints data and string to ob file ordered by address
 *
 */
int printDataAndStringtoFile(FILE* fileptr,datapointer datahead,stringpointer stringhead)
{
    int nulldataflag = DATAFLAGOFF;
    int nullstringflag = DATAFLAGOFF;
    int i = 0;
    if(stringhead == NULL)
        nullstringflag = DATAFLAGON;
    if(datahead == NULL)
        nulldataflag = DATAFLAGON;
    while(datahead != NULL || stringhead != NULL)
    {
        if(nulldataflag == DATAFLAGOFF && nullstringflag == DATAFLAGOFF && datahead->address <= stringhead->address)
        {
            for(i = 0 ; i < datahead->arraylen ; i++)
            {
                writeToObFile(fileptr,(datahead->array)[i],datahead->address+i);
            }
            datahead = datahead->next;
            if(datahead == NULL)
            {
                nulldataflag = DATAFLAGON;
            }
            continue;
        }
        if(nullstringflag == DATAFLAGOFF && nulldataflag == DATAFLAGOFF && datahead->address >= stringhead->address)
        {
            for (i = 0; i < strlen(stringhead->string); i++) {
                writeToObFile(fileptr,(stringhead->string)[i], stringhead->address+i);
            }
            writeToObFile(fileptr, '\0', stringhead->address+i);
            stringhead = stringhead->next;
            if (stringhead == NULL) {
                nullstringflag = DATAFLAGON;
            }
            continue;
        }
        if(nulldataflag == DATAFLAGON)
        {
            while(stringhead != NULL)
            {
                for (i = 0; i < strlen(stringhead->string); i++) {
                    writeToObFile(fileptr,(stringhead->string)[i], stringhead->address+i);
                }
                stringhead = stringhead->next;
            }
            return SUCCESS;
        }
        if(nullstringflag == DATAFLAGON)
        {
            while(datahead != NULL)
            {
                for(i = 0 ; i < datahead->arraylen ; i++)
                {
                    writeToObFile(fileptr,(datahead->array)[i],datahead->address+i);
                }
                datahead = datahead->next;
            }
            return SUCCESS;
        }

    }
    return FAILURE;
}

int updateEntryAddress(sympointer* headsymbol, datapointer datahead, stringpointer stringhead, int IC)
{
/*
 * vars
 */
int dc = 0;

sympointer  symmatch = *headsymbol;
stringpointer stringmatch = stringhead;
datapointer  datamatch =datahead;

while(symmatch != NULL)
{
    if(stringhead != NULL && symmatch->type == STRING)
    {
        while(stringmatch != NULL)
        {
            if(symmatch->data == stringmatch->address)
                {
                    symmatch->data = IC + dc;
                    stringmatch->address = symmatch->data;
                    dc += strlen(stringmatch->string)+DATAINDEXONE;
                    break;
                }
            stringmatch = stringmatch->next;
        }
        if(stringmatch == NULL)
        {
            return FAILURE;
        }
        stringmatch = stringhead;
    }
    if(datahead != NULL && symmatch->type == DATA)
    {
        while(datamatch != NULL)
        {
            if(symmatch->data == datamatch->address)
                {
                    symmatch->data = IC + dc;
                    datamatch->address = symmatch->data;
                    dc += datamatch->arraylen;
                    break;
                }
            datamatch = datamatch->next;
        }
        if(datamatch == NULL)
        {
            return FAILURE;
        }
        datamatch = datahead;
    }
    symmatch = symmatch->next;
}
return SUCCESS;
}


