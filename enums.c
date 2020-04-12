#include "enums.h"
char* registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
char* opcodes[] = {"mov","cmp","add","sub","not", "clr","lea","inc", "dec","jmp","bne","red","prn","jsr","rts","stop"};
char* directives[] = {"extern","entry","define", "data","string"};
int errorflag;
char* errorfilename;