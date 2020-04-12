#ifndef SYMBOLLIST_H
#define SYMBOLLIST_H


typedef enum {EXTERN , ENTRY , MACRO , DATA , STRING, CODE } symtype;

typedef struct symnode* sympointer;

typedef struct symnode
{
int data;
symtype type;
char* name;
struct symnode* next;
} symnode;


/*adding and getting symbol list nodes functions*/
sympointer initSymbolList(); /*returns a head node w/ UNINITIALIZED value*/
sympointer newSymbol(int inaddress, symtype symboltype, char* symname ); /*creates a new symbol*/
sympointer getCurrentSymbol(sympointer head);/*get current symbol*/
int addsymbol(char* symname, symtype symboltype, sympointer *head,sympointer *current,int curraddress);/*adds a new symbol to the symbol list*/

/*utils*/
void printSym(sympointer node);/*print symbol*/
const char* getDataName(symtype type);/*translates ENUM values to string values*/
void destroySymList(sympointer head);/*free symbol list*/
void printSymList(sympointer head);/*print symbol list*/
int getValue(sympointer head, char* symname,int* returnValue);/*get value from symbol using ptrs 0 success 1 failure*/

/*check type fun*/
int duplicateCheck(sympointer head, char* name,symtype type);/*checks for duplicate values*/
int isMacro(char* string, sympointer head);
int isData(char* string, sympointer head);
int isString(char* string, sympointer head);
int isExtern(char* string, sympointer head);
int isCode(char* string, sympointer head);
#endif
