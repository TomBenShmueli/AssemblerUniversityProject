#ifndef SYNTEXCHECK_H
#define SYNTEXCHECK_H

#ifndef FLAGS_H
#include "flags.h"
#endif

#ifndef ENUMS_H
#include "enums.h"
#endif

#define CHECKZERO 0
#define CHECKZEROINDEX 0
#define CHECKONEINDEX 1

#define CHECKONE 1
#define CHECKTWO 2

#define OK 101

#define STEPONE 1
#define STEPTWO 2
#define STEPTHREE 3
#define STEPFOUR 4

int checkOpcodeDiracetAddressing(char* name);
int checkNumber(char* number);
int checkConfigName(char* name);
int checkName(char* name);
int isWordDirective(char * word);
int isWordOpcode(char * word);
int isWordRegister(char * word);
int isFirstWordSymbolConfiguration(char * word);

#endif
