#ifndef SECONDRUNFUNCTIONS_H
#define SECONDRUNFUNCTIONS_H

int registersToBits(char* register1, char* register2, int* output);
void indexValueToBits(char* value, int* output,sympointer symhead);
int indexToBits(char * index, char * value,int * output, sympointer head, FILE *outFile, int IC,  int *externFlag);
int instintToBits(char* input, int* output, sympointer head);
int directToBits(char * input, int* output, sympointer head, FILE *outFile, int IC,  int *externFlag);
void sourceOperand(int sourceOperand, int *output);
void destOperand(int destOperand, int* output);
void opcodeToBits(int opcodeType, int* output);
void freeFile(FILE ** f1, FILE ** f2, char** n1, char** n2, char** n3);
void writeToObFile(FILE* file, int numToConvert, int lineNumber);
int checkExistingEntryValue(sympointer head);

#endif
