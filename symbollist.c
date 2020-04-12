/*
 * This file will include the functions related to the symbol list.
 */

#include "symbollist.h"
#include "flags.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syntexCheck.h"


/*initializes a symbol list - returns a head node with UNDEFINED */
sympointer initSymbolList()
{
sympointer tmp=NULL;
    tmp= (sympointer)malloc(sizeof(symnode));
    if(tmp== NULL)
    {
        fprintf(stderr, "Out of memory\n");
        return NULL;
    }
    tmp->data= UNINITIALIZED;
    tmp->type = MACRO;
    tmp->name = "HEAD";
    tmp->next = NULL;
    return tmp;
}
/*inits a new symbol*/
sympointer newSymbol(int inaddress, symtype symboltype, char* symname) {


    sympointer tmp=NULL;
    tmp= (sympointer)malloc(sizeof(symnode));
    if(tmp== NULL)    {

            fprintf(stderr,"Out of memory at line %d.\n", inaddress);
        return NULL;
    }
    tmp->name =(char*)malloc(strlen(symname)+1);
    if(tmp->name == NULL)
    {
        fprintf(stderr,"Out of memory at line %d.\n", inaddress);
        return NULL;
    }
    switch (symboltype)
    {
        case EXTERN: /* no value, line num  */
            tmp->data=inaddress;
            tmp->type = symboltype;
            strcpy(tmp->name,symname);
            tmp->next = NULL;
            break;
        case ENTRY: /* no valuem, line num*/
            tmp->data = inaddress;
            tmp->type = symboltype;
            strcpy(tmp->name,symname);
            tmp->next = NULL;
            break;
        case MACRO: /* inaddress is the value parameter */
            tmp->data = inaddress;
            tmp->type = symboltype;
            strcpy(tmp->name,symname);
            tmp->next = NULL;
            break;
        case CODE: /* no value, line num */
            tmp->data = inaddress;
            tmp->type = symboltype;
            strcpy(tmp->name,symname);
            tmp->next = NULL;
            break;
        case DATA: /* no value, line num */
            tmp->data=  inaddress;
            tmp->type = symboltype;
            strcpy(tmp->name,symname);
            tmp->next = NULL;
            break;
        case STRING: /* no value, line num */
            tmp->data = inaddress;
            tmp->type = symboltype;
            strcpy(tmp->name,symname);
            tmp->next = NULL;

            break;
        default:
                    fprintf(stderr,"No matching symbol type found at line %d.\n", inaddress);
    }
    return tmp;
}
/*get current node*/
sympointer getCurrentSymbol(sympointer head)
{
    sympointer ptr = head;
    while(ptr->next != NULL)
    {
        ptr = ptr ->next;
    }
    return ptr;
}

/*
 * Adds a symbol to the symbol list and reports via defined values about the process. Since newsymbol takes care of the memory allocation it is the function responsible for error handling.
 */

int addsymbol(char* symname, symtype symboltype, sympointer *head,sympointer *current,int currdata)
{
    sympointer tmp = newSymbol(currdata , symboltype, symname);
    sympointer freeptr = *head;
    if(duplicateCheck(*head,symname,symboltype) == FAILURE)/*dup symcheck*/
    {
       fprintf(stderr,"Duplicate symbol name at line %d\n",currdata);
        return FAILURE;
    }
    if( tmp == NULL )
    {
        fprintf(stderr,"Memory allocation failed at line %d.\n",currdata);
        return FAILURE;
    }
    else if((*head)->data == UNINITIALIZED) /*no nodes added*/
    {
        (*head) = tmp;
        (*current) = (*head);
        free(freeptr);
    }
    else /*add to tail*/
    {
        (*current)->next = tmp;
        (*current) = (*current)->next;
    }
    return SUCCESS;
}

/*
 * Print symbol data
 */
void printSym(sympointer node)
{
        printf("data:%d\nType:%s\nName:%s\n\n",node->data,getDataName(node->type),node->name);
}

/*
 * ENUM parser
 * EXTERN , ENTRY , MACRO ,CODE , DATA , STRING , LABEL
 */
const char* getDataName(symtype type)
{
    switch (type)
    {
        case EXTERN: return "EXTERN";
        case ENTRY: return "ENTRY";
        case MACRO: return "MACRO";
        case CODE: return "CODE";
        case DATA: return "DATA";
        case STRING: return "STRING";
        default: return NULL;

    }
}

/*
 * Recursively frees the linked list
 */

void destroySymList(symnode* head)
{
    if(head == NULL)
        return;
    destroySymList(head->next);
    free(head->name);
    free(head);
}
/*
 * Print symbol list
 */
void printSymList(sympointer head)
{
    if(head == NULL)
        return;
    printSym(head);
    printSymList(head->next);
}

/*
 * returns data field for symbol using pointers 
*/
int getValue(sympointer head, char* symname,int* returnValue)
{
		while(head != NULL)
		{
			if(strcmp(head->name,symname) == 0)
			{
				*returnValue = head->data;
				return SUCCESS;
			}
			head = head->next;
		}		
	return FAILURE;
}

/*duplicate symbol check*/
int duplicateCheck(sympointer head, char* name,symtype type)
{
    while(head)
    {
        if(strcmp(head->name,name) == 0 && head->type == type && type != ENTRY)/*ENTRY values can be duplicate for the same symbol name*/
        {
            return FAILURE;
        }
        head = head->next;
    }
    return SUCCESS;
}

/*
 * checks is a symbol is a defined macro
 */
int isMacro(char* string, sympointer head)
{
    if(string == NULL)
        return FAILURE;
    while(head)
    {
        if(strcmp(string,head->name) ==0 && head->type == MACRO)
        {
            return SUCCESS;
        }
        head = head->next;
    }
    return FAILURE;
}
/*
 * checks is a symbol is a defined data
 */
int isData(char* string, sympointer head)
{
    while(head)
    {
        if(strcmp(string,head->name) ==0 && head->type == DATA)
        {
            return SUCCESS;
        }
        head = head->next;
    }
    return FAILURE;
}
/*
 * checks is a symbol is a defined string
 */
int isString(char* string, sympointer head)
{
    while(head)
    {
        if(strcmp(string,head->name) ==0 && head->type == STRING)
        {
            return SUCCESS;
        }
        head = head->next;
    }
    return FAILURE;
}
/*
 * checks is a symbol is a defined extern
 */
int isExtern(char* string, sympointer head)
{
    while(head)
    {
        if(strcmp(string,head->name) ==0 && head->type == EXTERN)
        {
            return SUCCESS;
        }
        head = head->next;
    }
    return FAILURE;
}
/*
 * checks is a symbol is a defined code symbol
 */
int isCode(char* string, sympointer head)
{
    while(head)
    {
        if(strcmp(string,head->name) ==0 && head->type == CODE)
        {
            return SUCCESS;
        }
        head = head->next;
    }
    return FAILURE;
}











