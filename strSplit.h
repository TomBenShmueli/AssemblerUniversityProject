#ifndef STRSPLIT_H
#define STRSPLIT_H

#ifndef _IO_STDIO
#include <stdio.h>
#endif

int getLine(FILE **f, char (*line)[]);
void freeSplittedLine(char ***line, int numOfCells);
int strSplit(char ***newLine, char *line);
int findLastQuotationMarks(char line[]);
int SyntaxCheck(char **splittedLine, int numOfWords, int lineCounter, int lineExtractResualt);

#endif
