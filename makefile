all: play

play: codeStorage.o dataStorage.o enums.o main.o secondRunFunctions.o strsplit.o SymbolAdditionParse.o symbollist.o syntexCheck.o
	gcc -ansi -Wall -pedantic -o play codeStorage.o dataStorage.o enums.o main.o secondRunFunctions.o strsplit.o SymbolAdditionParse.o symbollist.o syntexCheck.o

codeStorage.o: codeStorage.c codeStorage.h symbollist.h flags.h SymbolAdditionParse.h dataStorage.h enums.h
	gcc -ansi -Wall -pedantic -c codeStorage.c -o codeStorage.o

dataStorage.o: dataStorage.c dataStorage.h symbollist.h flags.h secondRunFunctions.h SymbolAdditionParse.h
	gcc -ansi -Wall -pedantic -c dataStorage.c -o dataStorage.o

enums.o: enums.c enums.h
	gcc -ansi -Wall -pedantic -c enums.c -o enums.o

main.o: main.c mainHfile.h dataStorage.h
	gcc -ansi -Wall -pedantic -c main.c -o main.o

secondRunFunctions.o: secondRunFunctions.c secondRunFunctions.h symbollist.h enums.h syntexCheck.h flags.h
	gcc -ansi -Wall -pedantic -c secondRunFunctions.c -o secondRunFunctions.o

strsplit.o: strSplit.c strSplit.h
	gcc -ansi -Wall -pedantic -c strSplit.c -o strsplit.o

SymbolAdditionParse.o: SymbolAdditionParse.c SymbolAdditionParse.h symbollist.h flags.h
	gcc -ansi -Wall -pedantic -c SymbolAdditionParse.c -o SymbolAdditionParse.o

symbollist.o: symbollist.c symbollist.h flags.h syntexCheck.h
	gcc -ansi -Wall -pedantic -c symbollist.c -o symbollist.o

syntexCheck.o: syntexCheck.c syntexCheck.h enums.h flags.h
	gcc -ansi -Wall -pedantic -c syntexCheck.c -o syntexCheck.o

clear:
	rm -rf *.o *~play



