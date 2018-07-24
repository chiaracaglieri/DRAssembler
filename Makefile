lex.yy.c: asm1.l
	lex asm1.l

lex.o: lex.yy.c
	gcc -c -o lex.o lex.yy.c

data_structures.o: data_structures.cpp
	gcc -c -o data_structures.o data_structures.cpp

main.o: main.cpp
	g++ -c -o main.o main.cpp

pass1.o: main.o lex.o data_structures.o
	g++ -o pass1 lex.o main.o data_structures.o

run: pass1.o
	./pass1 < samplecode.txt