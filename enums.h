#ifndef ENUMS_H
#define ENUMS_H

#define NUMBER_OF_REGISTERS 8
#define NUMBER_OF_OPCODES 16

typedef enum { INSTINT, DIRECT, INDEX, REGISTER} opcodeType;
typedef enum { R0, R1, R2, R3, R4, R5, R6, R7 } reg;
typedef enum { MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP} opcode;

extern char* registers[];
extern char* opcodes[];
extern char* directives[];
extern int errorflag;
extern char* errorfilename;

#endif

