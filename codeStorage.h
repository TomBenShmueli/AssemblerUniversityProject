

#ifndef ADDMACRO_CODESTORAGE_H
#define ADDMACRO_CODESTORAGE_H

#ifndef SYMBOLLIST_H
#include "symbollist.h"
#endif

#ifndef FLAGS_H
#include "flags.h"
#endif

#ifndef SYMBOLADDITIONPARSE_H
#include "SymbolAdditionParse.h"
#endif

#ifndef DATASTORAGE_H
#include "dataStorage.h"
#endif

#ifndef ENUMS_H
#include "enums.h"
#endif

#define ON 1
#define OFF -1
#define ONEINTERVAL 1
#define TWOINTERVAL 2

#define OPEN_BRACKETS 43
#define CLOSED_BRACKETS 44


/*
 * code line handling functions
 */
int storeCode(char** line, int linenum,int numofwords, sympointer *symhead);
/*
 * utilities functions
 */
int registerCheck(char* string);
int valueCheck(char* string,sympointer head); /*checks for expressions of the following type: #number */
int dataCheck(char* string, sympointer head); /*check for data symbols: LIST: .data .... */

#endif
