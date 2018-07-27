asm1.tab.c asm1.tab.h: asm1.y
	bison -d --debug -v asm1.y

lex.yy.c: asm1.l asm1.tab.h
	flex asm1.l

lex.o: lex.yy.c asm1.tab.h
	g++ -c -o lex.o lex.yy.c

yacc.o: asm1.tab.c
	g++ -c -o yacc.o asm1.tab.c

data_structures.o: data_structures.cpp
	g++ -c -o data_structures.o data_structures.cpp

main.o: main.cpp
	g++ -o main.o main.cpp

pass1.o: main.o lex.o yacc.o data_structures.o
	g++ -o pass1 lex.o yacc.o main.o data_structures.o

run: pass1.o
	./pass1 < samplecode.txt

clean:
	rm -f *.o
	rm -f asm1.tab.cc lex.yy.cc asm1.tab.hh asm1.output pass1.exe stack.hh