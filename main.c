
#include "syntexCheck.h"
#include "strSplit.h"
#include "mainHfile.h"
#include "symbollist.h"
#include "dataStorage.h"
#include "SymbolAdditionParse.h"
#include "codeStorage.h"
#include "secondRunFunctions.h"


int main(int argc, char *argv[])
{
	int fileNameIndex = MAIN_ONE;
	int DC;
	int IC;
	char* fileName = NULL;
	FILE * sourceFile = NULL;
	sympointer symhead = NULL;
	stringpointer stringhead = NULL;
	datapointer datahead = NULL;

	if(argc == MAIN_ONE)
	{
        fprintf(stderr,"\nNo file names have been recived.\n");
		return FAILURE;
	}

	/* Parsing each given file */
	while(fileNameIndex < argc)
	{
	    IC = FIRST_ADDRESS;
	    DC = MAIN_ZERO;
		if(checkName(argv[fileNameIndex]) == FAILURE)
		{
			fprintf(stderr,"Given file name \"%s\" is not a valid name.", argv[fileNameIndex]);
			fileNameIndex++;
			continue;
		}

        errorflag = SUCCESS;
		errorfilename = argv[fileNameIndex];


		fileName = (char*)malloc(strlen(argv[fileNameIndex]) + MAIN_FILE_EXTENSION_FOUR);
		if(fileName == NULL)
		{
            fprintf(stderr,"\nPC out of memory.\n");/*$$$$chanfe evey failure to stderr*/
			return FAILURE;
		}
		strcpy(fileName, argv[fileNameIndex]);
		strcat(fileName, ".as");
		sourceFile = fopen(fileName, "r");
		if(sourceFile == NULL)
		{
            fprintf(stderr,"File \"%s\" won't open, please check file permissions and rerun the script", errorfilename);
			free(fileName);
			fileNameIndex++;
			continue;
		}
		symhead = initSymbolList();

		if(FirstRun(sourceFile, &symhead, &stringhead, &datahead, &DC, &IC) == FAILURE)
		{
            fprintf(stderr,"File \"%s\" parsing file fails, please fix errors and rerun the script.", argv[fileNameIndex]);
			fclose(sourceFile);
			free(fileName);
			DC = MAIN_ZERO;
			destroySymList(symhead);
			destroyStringList(stringhead);
			destroyDataList(datahead);
			symhead = NULL;
			stringhead = NULL;
			datahead = NULL;
            fileNameIndex++;
			continue;
		}
		

		if ( fseek(sourceFile, 0L, SEEK_SET) != MAIN_ZERO )
		{

            fprintf(stderr,"File \"%s\" reading failed, please check file permissions and rerun the script", argv[fileNameIndex]);
			fclose(sourceFile);
			free(fileName);
			DC = MAIN_ZERO;
			destroySymList(symhead);
			destroyStringList(stringhead);
			destroyDataList(datahead);
			symhead = NULL;
			stringhead = NULL;
			datahead = NULL;
            fileNameIndex++;
			continue;
		} 
		if(secoundRun(sourceFile, &symhead,&stringhead, &datahead, &DC, argv[fileNameIndex], &IC) == FAILURE)
		{
            fprintf(stderr,"File \"%s\" parsing file fails, please fix errors and rerun the script.", argv[fileNameIndex]);
			fclose(sourceFile);
			free(fileName);
			DC = MAIN_ZERO;
			destroySymList(symhead);
			destroyStringList(stringhead);
			destroyDataList(datahead);
			symhead = NULL;
			stringhead = NULL;
			datahead = NULL;
            fileNameIndex++;
			continue;
		}

		fclose(sourceFile);
		free(fileName);
		DC = MAIN_ZERO;
		destroySymList(symhead);
		destroyStringList(stringhead);
		destroyDataList(datahead);
		symhead = NULL;
		stringhead = NULL;
		datahead = NULL;
        fileNameIndex++;
	}
	return SUCCESS;
}

int FirstRun(FILE * sourceFile, sympointer * symhead, stringpointer *stringHead, datapointer *dataHead, int *DC, int* IC)
{
	int dccontrol;
	int checker = MAIN_ZERO;
	char line[82];
	int lineCounter = MAIN_ZERO;
	char **splittedLine = NULL;
	int numOfWords = MAIN_ZERO;
	sympointer currentsym = getCurrentSymbol(*symhead);
	*DC = MAIN_ZERO;

	do
	{
		if((checker = getLine(&sourceFile, &line)) == EOF)
        {
            break;
        }
		lineCounter++;
		numOfWords = strSplit(&splittedLine, line);
		if(numOfWords == -1)
		{
			fprintf(stderr, "PC out of memory at line %d", lineCounter);/*$$$$$ not a valid function*/
			return FAILURE;
		}

		if(SyntaxCheck(splittedLine, numOfWords, lineCounter, checker) == FAILURE)
		{
			freeSplittedLine(&splittedLine,numOfWords);
			continue;
		}

        if(numOfWords < 2)
        {
            freeSplittedLine(&splittedLine,numOfWords);
            continue;
        }

		if(splittedLine[MAINZEROINDEX][MAINZEROINDEX] ==';')
		{
			freeSplittedLine(&splittedLine,numOfWords);
			continue;
		}		
			
		if(symbolMacroAddition(splittedLine, numOfWords, lineCounter, symhead, &currentsym) == SUCCESS)/* should handle sections: 3,4
- if it is a macro with error should return seccess*/
		{
			freeSplittedLine(&splittedLine,numOfWords);
			continue;
		}
        dccontrol = storeString(lineCounter, splittedLine, stringHead, symhead);
        if(dccontrol != STRING_FAILURE)
        {
            *DC += dccontrol;
            freeSplittedLine(&splittedLine,numOfWords);
            continue;
        }


        dccontrol = storeData(lineCounter, numOfWords, splittedLine,  dataHead, symhead);
		if(dccontrol != DATA_FAILURE)
		{
		    *DC += dccontrol;
			freeSplittedLine(&splittedLine,numOfWords);
			continue;
		}


        if(storeEntryExtern(lineCounter,numOfWords,splittedLine, symhead) == SUCCESS)
        {
            freeSplittedLine(&splittedLine,numOfWords);
            continue;
        }
        *IC += storeCode(splittedLine,*IC,numOfWords,symhead);
		freeSplittedLine(&splittedLine,numOfWords);


	}while(checker != EOF);

    errorflag =  updateEntryAddress(symhead,*dataHead,*stringHead,*IC);


    if(checkExistingEntryValue(*symhead) == SUCCESS && errorflag == SUCCESS)
	    return SUCCESS;
    return FAILURE;
}

int secoundRun(FILE * sourceFile, sympointer * symhead,stringpointer *stringHead, datapointer *dataHead, int *DC, char * fileName, int* sourceIC)
{
	int IC = FIRST_ADDRESS;
	int checker = MAIN_ZERO;
	int flag1 = MAIN_ZERO;
	int flag2 = MAIN_ZERO;
	char line[82];
	int runner = MAIN_ZERO;
	int lineCounter = MAIN_ZERO;
	char **splittedLine = NULL;
	int numOfWords = MAIN_ZERO;
	int wordToPrint = MAIN_ZERO;
	int  opcodeType;
	char* outputFileOb = NULL;
    char* outputFileEnt = NULL;
    char* outputFileExt = NULL;
	FILE * dstObFile = NULL;
    FILE * dstExtFile = NULL;
    int externFlag = MAIN_ZERO;

    outputFileEnt = (char*)malloc(strlen(fileName) + MAIN_FILE_EXTENSION_FIVE);
    if(outputFileEnt == NULL)
    {
        fprintf(stderr,"File %s: PC out of memory %d\n",errorfilename, lineCounter);
        return FAILURE;
    }
    strcpy(outputFileEnt, fileName);
    strcat(outputFileEnt, ".ent");

    outputFileExt = (char*)malloc(strlen(fileName) + MAIN_FILE_EXTENSION_FIVE);
    if(outputFileExt == NULL)
    {
        fprintf(stderr,"File %s: PC out of memory %d\n",errorfilename, lineCounter);
        free(outputFileEnt);
        return FAILURE;
    }
    strcpy(outputFileExt, fileName);
    strcat(outputFileExt, ".ext");
    dstExtFile = fopen(outputFileExt, "w");
    if(dstExtFile == NULL)
    {
        free(outputFileEnt);
        free(outputFileExt);
        fprintf(stderr,"File %s: can't open output file - check permissions. \n",errorfilename);
        return FAILURE;
    }

	/* Output file openning $$$$ should add closing abd deleting unwanted files */
	outputFileOb = (char*)malloc(strlen(fileName) + MAIN_FILE_EXTENSION_FOUR);
	if(outputFileOb == NULL)
	{
        fprintf(stderr,"File %s: PC out of memory %d\n",errorfilename, lineCounter);
        free(outputFileEnt);
        free(outputFileExt);
        fclose(dstObFile);
        remove(outputFileExt);
		return FAILURE;
	}
	strcpy(outputFileOb, fileName);
	strcat(outputFileOb, ".ob");
	dstObFile = fopen(outputFileOb, "w");
	if(dstObFile == NULL)
	{
		free(outputFileOb);
        free(outputFileEnt);
        free(outputFileExt);
        fclose(dstObFile);
        remove(outputFileExt);
        fprintf(stderr,"File %s: can't open output file - check permissions. \n",errorfilename);
		return FAILURE;
	}


	
	/*--------------------------------*/

	/*Parsing lines output for .ob file*/
    fprintf(dstObFile,"\t%d %d\n",(*sourceIC-FIRST_ADDRESS),*DC);
	do
	{
        if((checker = getLine(&sourceFile, &line)) == EOF)
        {
            break;
        }
		lineCounter++;
		numOfWords = strSplit(&splittedLine, line);
		runner = MAIN_ZERO;
		if(numOfWords == -1)
		{
            fprintf(stderr,"File %s: PC out of memory %d\n",errorfilename, lineCounter);
			freeFile(&dstObFile, &dstExtFile, &outputFileOb, &outputFileEnt, &outputFileExt);
			return FAILURE;
		}
		
		if(numOfWords < 2)
		{
			freeSplittedLine(&splittedLine,numOfWords);
			continue;	
		}

		if(splittedLine[MAINZEROINDEX][MAINZEROINDEX] ==';')
		{
			freeSplittedLine(&splittedLine,numOfWords);
			continue;
		}

		if(strcmp(splittedLine[MAINZEROINDEX],".define") == 0)
		{
			freeSplittedLine(&splittedLine,numOfWords);
			continue;
		}
		
		if(isFirstWordSymbolConfiguration(splittedLine[MAINZEROINDEX]) == SUCCESS)
		{
			runner++;
		}

		if(strcmp(splittedLine[runner],".entry") == 0 || strcmp(splittedLine[runner],".extern") == 0 || strcmp(splittedLine[runner],".data") == 0 || strcmp(splittedLine[runner],".string") == 0)
		{
			freeSplittedLine(&splittedLine,numOfWords);
			continue;
		}
		opcodeType = isWordOpcode(splittedLine[runner]);
		runner++;  
		
		switch (opcodeType)
		{
			case CMP:
			case MOV:
			case ADD:
			case SUB:
			case LEA:
				opcodeToBits(opcodeType, &wordToPrint);
				if(isWordRegister(splittedLine[runner]) != -1)
				{
					sourceOperand(REGISTER, &wordToPrint);
					flag1 = REGISTER;
				}
				else if(splittedLine[runner][MAINZEROINDEX] == '#')
				{
					sourceOperand(INSTINT, &wordToPrint);
					flag1 = INSTINT;
				}
				else if(splittedLine[runner+1][0] == '[')
				{
					sourceOperand(INDEX, &wordToPrint);
					flag1 = INDEX;
				}
				else
				{
					sourceOperand(DIRECT, &wordToPrint);
					flag1 = DIRECT;
				}   
				if(flag1 == INDEX)
				{
					if(isWordRegister(splittedLine[runner+5]) != -1)
					{
						destOperand(REGISTER, &wordToPrint);
						flag2 = REGISTER;
					}
					else if(splittedLine[runner+5][0] == '#')
					{
						destOperand(INSTINT, &wordToPrint);
						flag2 = INSTINT;
					}
					else if((runner+6) < numOfWords && splittedLine[runner+6][0] == '[')
					{
						destOperand(INDEX, &wordToPrint);
						flag2 = INDEX;
					}
					else
					{
						destOperand(DIRECT, &wordToPrint);
						flag2 = DIRECT;
					}
				}
				else
				{
					if(isWordRegister(splittedLine[runner+2]) != -1)
					{
						destOperand(REGISTER, &wordToPrint);
						flag2 = REGISTER;
					}
					else if(splittedLine[runner+2][0] == '#')
					{
						destOperand(INSTINT, &wordToPrint);
						flag2 = INSTINT;
					}
					else if((runner+3) < numOfWords && splittedLine[runner+3][0] == '[')
					{
						destOperand(INDEX, &wordToPrint);
						flag2 = INDEX;
					}
					else
					{
						destOperand(DIRECT, &wordToPrint);
						flag2 = DIRECT;
					}
				}
				writeToObFile(dstObFile, wordToPrint, IC);
				IC++;
				wordToPrint = 0;				
				/*FIRST operand print*/
				if(flag1 == INSTINT)
				{
					if(instintToBits(splittedLine[runner], &wordToPrint, *symhead) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=2;
					}
					else
					{
						fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
						errorflag = FAILURE;
                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag1 == DIRECT)
				{
					if(directToBits(splittedLine[runner], &wordToPrint, *symhead, dstExtFile, IC, &externFlag) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=2;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;
                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag1 == INDEX)
				{
					if(indexToBits(splittedLine[runner],splittedLine[runner+2], &wordToPrint, *symhead, dstExtFile, IC, &externFlag) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						indexValueToBits(splittedLine[runner+2], &wordToPrint, *symhead);
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=5;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;

                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag1 == REGISTER)
				{
					if(registersToBits(splittedLine[runner],splittedLine[runner +2], &wordToPrint) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=2;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal syntax on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;
                        freeFile(&dstObFile, &dstExtFile, &outputFileOb, &outputFileEnt, &outputFileExt);
                        freeSplittedLine(&splittedLine,numOfWords);
                        return FAILURE;
					}
				}
				/*------------------------------*/
				/*SECOND operand print*/
				if(flag2 == INSTINT)
				{
					if(instintToBits(splittedLine[runner], &wordToPrint, *symhead) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=2;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;

                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag2 == DIRECT)
				{
					if(directToBits(splittedLine[runner], &wordToPrint, *symhead, dstExtFile, IC, &externFlag) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=2;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;

                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag2 == INDEX)
				{
					if(indexToBits(splittedLine[runner],splittedLine[runner+2], &wordToPrint,*symhead , dstExtFile, IC, &externFlag) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						indexValueToBits(splittedLine[runner+2], &wordToPrint, *symhead);
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=5;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;

                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag2 == REGISTER)
				{
					if(flag1 == REGISTER)
					{
						break;
					}
					if(registersToBits(NULL,splittedLine[runner], &wordToPrint) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal syntax on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;

                        freeFile(&dstObFile, &dstExtFile, &outputFileOb, &outputFileEnt, &outputFileExt);
                        freeSplittedLine(&splittedLine,numOfWords);
                        return FAILURE;
					}
				}
				/*------------------------------*/	
				break;

			case JMP:
			case BNE:
			case JSR:
			case DEC:
			case RED:
			case INC:
			case NOT:
			case CLR:
			case PRN:
				opcodeToBits(opcodeType, &wordToPrint);
				if(isWordRegister(splittedLine[runner]) != -1)
				{
                    destOperand(REGISTER, &wordToPrint);
					flag1 = REGISTER;
				}
				else if(splittedLine[runner][0] == '#')
				{
                    destOperand(INSTINT, &wordToPrint);
					flag1 = INSTINT;
				}
				else if((runner+1) < numOfWords && splittedLine[runner+1][0] == '[')
				{
                    destOperand(INDEX, &wordToPrint);
					flag1 = INDEX;
				}
				else
				{
                    destOperand(DIRECT, &wordToPrint);
					flag1 = DIRECT;
				}   
				writeToObFile(dstObFile, wordToPrint, IC);
				IC++;
				wordToPrint = 0;				
				/*OPERAND print*/
				if(flag1 == INSTINT)
				{
					if(instintToBits(splittedLine[runner], &wordToPrint,*symhead) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=2;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;

                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag1 == DIRECT)
				{
					if(directToBits(splittedLine[runner], &wordToPrint, *symhead, dstExtFile, IC, &externFlag) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=2;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;
                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag1 == INDEX)
				{
					if(indexToBits(splittedLine[runner],splittedLine[runner+2], &wordToPrint, *symhead , dstExtFile, IC, &externFlag) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						indexValueToBits(splittedLine[runner+2], &wordToPrint, *symhead);
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
						runner +=5;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal label on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;
                        freeSplittedLine(&splittedLine,numOfWords);
                        continue;
					}
				}
				if(flag1 == REGISTER)
				{
					if(registersToBits(splittedLine[runner],NULL, &wordToPrint) == SUCCESS)
					{
						writeToObFile(dstObFile, wordToPrint, IC);
						IC++;
						wordToPrint = 0;
					}
					else
					{
                        fprintf(stderr,"File %s: Illegal syntax on line: %d\n",errorfilename, lineCounter);
                        errorflag = FAILURE;
                        freeFile(&dstObFile, &dstExtFile, &outputFileOb, &outputFileEnt, &outputFileExt);
                        freeSplittedLine(&splittedLine,numOfWords);
                        return FAILURE;
					}
				}
				/*------------------------------*/
				break;

			case RTS:
			case STOP:
				opcodeToBits(opcodeType, &wordToPrint);
                writeToObFile(dstObFile, wordToPrint, IC);
                IC++;
				break;

			default:
            {
                fprintf(stderr,"File %s: Illegal syntax on line: %d\n",errorfilename, lineCounter);
                errorflag = FAILURE;
            }
		}
        freeSplittedLine(&splittedLine,numOfWords);
	}while(checker != EOF);
	
	printDataAndStringtoFile(dstObFile,*dataHead,*stringHead);

	if(errorflag == FAILURE)
    {
        freeFile(&dstObFile, &dstExtFile, &outputFileOb, &outputFileEnt, &outputFileExt);
        return FAILURE;
    }

	printEntryListFile(outputFileEnt,lineCounter,*symhead);



    if(fclose(dstObFile) != 0)
    {
        fprintf(stderr,"File %s: Unable to close file: unexpected behavior may occur on line: %d\n",errorfilename, lineCounter);
    }
    if(fclose(dstExtFile) != 0)
    {
        fprintf(stderr,"File %s: Unable to close file: unexpected behavior may occur on line: %d\n",errorfilename, lineCounter);
    }
    if(*sourceIC == 0 && *DC == 0)
    {
        if(remove(outputFileOb) != 0)
        {
            fprintf(stderr,"File %s: Unable to close file: unexpected behavior may occur on line: %d\n",errorfilename, lineCounter);
        }
    }
    if(externFlag == 0)
    {
        if(remove(outputFileExt) != 0)
        {
            fprintf(stderr,"File %s: Unable to close file: unexpected behavior may occur on line: %d\n",errorfilename, lineCounter);
        }
    }
    free(outputFileOb);
    free(outputFileExt);
    free(outputFileEnt);
	return SUCCESS;
}






