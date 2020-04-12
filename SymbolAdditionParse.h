
#ifndef SYMBOLADDITIONPARSE_H
#define SYMBOLADDITIONPARSE_H

#include <string.h>

#ifndef SYMBOLLIST_H
#include "symbollist.h"
#endif

#ifndef FLAGS_H
#include "flags.h"
#endif

int symbolMacroAddition(char** line,int numOfWords ,int linenum,sympointer* headsym,sympointer* currentsym);/*gets a splitted line from StrSplit()*/
int isLabel(char* line); /* success if LABEL , failure if not */
int extractLabelName(char* label,char** returnValue);

#define LONGMACRO 5
#define SHORTMACRO 4

#define MACROFLAGOFF -1
#define MACROFLAGON 1

#define MACROINDEXONE 1

#define MACROZERO 0

#endif
