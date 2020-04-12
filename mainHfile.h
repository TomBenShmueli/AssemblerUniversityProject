#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DATASTORAGE_H
#include "dataStorage.h"
#endif

#define SUCCESS 0
#define FAILURE 1


#define MAIN_ONE 1
#define MAIN_ZERO 0
#define MAIN_FILE_EXTENSION_FOUR 4
#define MAIN_FILE_EXTENSION_FIVE 5
#define FIRST_ADDRESS 100
#define MAINZEROINDEX 0



int FirstRun(FILE * sourceFile, sympointer * symhead, stringpointer *stringHead, datapointer *dataHead, int *DC, int* IC); /*This function does the first run as wrriten in the book*/
int secoundRun(FILE * sourceFile, sympointer * symhead,stringpointer *stringHead, datapointer *dataHead, int *DC, char * fileName, int* sourceIC);/*This function does the secound run as wrriten in the book*/