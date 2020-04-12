#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#ifndef SYMBOLLIST_H
#include "symbollist.h"
#endif


#ifndef FLAGS_H
#include "flags.h"
#endif

#ifndef SYMBOLADDITIONPARSE_H
#include "SymbolAdditionParse.h"
#endif

#ifndef SECONDRUNFUNCTIONS_H
#include "secondRunFunctions.h"
#endif

#define DATAINDEXZERO 0
#define DATAINDEXONE 1
#define DATAINDEXTWO 2
#define DATAINDEXTHREE 3

#define SCANCOMPLETE 1
#define INCOMPLETE -1
#define SIGNFLAGON 1
#define SIGNFLAGOFF -1
#define NEGATIVESIGN -1

#define DATASENTANCELENLONG 4
#define DATASETNANCELENSHORT 3

#define DATAFLAGOFF -1
#define DATAFLAGON 1

#define ADDZEROLIMIT 999

#define ONECHAR 1
#define ADDTEN 10

typedef struct stringnode* stringpointer;
typedef struct stringnode
{
    int address;
    char* string;
    stringpointer next;
} stringnode;

typedef struct datanode* datapointer;
typedef struct datanode
{
    int address;
    int* array;
    int arraylen;
    datapointer next;
} datanode;

#define DATA_FAILURE -1
#define STRING_FAILURE -1
#define COMMA_FOUND -1


/*string fun*/

int storeString(int linenum,char** line,stringpointer *current,sympointer *headsymbol ); /*interface function*/
stringpointer addStringNode(int inaddress, char* stringname, stringpointer* strhead); /*util - add and link string node*/
void destroyStringList(stringpointer head); /*interface - print list*/
void printStringList(stringpointer head); /*interface - print list*/

/*data fun*/
int storeData(int linenum,int numofwords ,char** line,datapointer *current, sympointer *headSymbol); /*interface function for storing data, returns the number of elements added to the node */
datapointer addDataNode(int inaddress,char** data, int arraylen, datapointer* datahead,sympointer symhead);
int checkNum(int* num,char* string,sympointer head);
void destroyDataList(datapointer head);
void printDataList(datapointer head);


/*entry / extern fun*/
int storeEntryExtern(int linenum,int numofwords,char** line, sympointer* headsymbol);
void printExternList(sympointer headsymbol);
void printEntryList(sympointer headsymbol);
int printEntryListFile(char* filename,int linenum ,sympointer headsymbol);
int updateEntryAddress(sympointer* headsymbol, datapointer datahead, stringpointer stringhead, int IC);

/*print to file*/
int printDataAndStringtoFile(FILE* fileptr,datapointer datahead,stringpointer stringhead);


#endif
