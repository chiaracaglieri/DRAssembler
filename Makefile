lex.yy.c: asm1.l
	lex asm1.l

lex.o: lex.yy.c
	gcc -c -o lex.o lex.yy.c

y.tab.c: asm1.y
	yacc asm1.y

yacc.o: y.tab.c
	gcc -c -o yacc.o y.tab.c

data_structures.o: data_structures.cpp
	gcc -c -o data_structures.o data_structures.cpp

main.o: main.cpp
	g++ -c -o main.o main.cpp

pass1.o: main.o lex.o yacc.o data_structures.o
	g++ -o pass1 lex.o yacc.o main.o data_structures.o

run: pass1.o
	./pass1 < samplecode.txt