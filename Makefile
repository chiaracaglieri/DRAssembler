asm1.tab.c: asm1.y
	bison -d -t asm1.y

asm1.tab.o: asm1.tab.c
	g++ -c asm1.tab.c

lex.yy.c: asm1.l asm1.tab.c
	flex asm1.l

lex.yy.o: lex.yy.c
	g++ -c lex.yy.c

util.o: util.cpp
	g++ -c util.cpp

main.o: main.cpp
	g++ -c main.cpp

parser: asm1.tab.o lex.yy.o main.o util.o
	g++ -o parser asm1.tab.o lex.yy.o main.o util.o

run: parser
	./parser < samplecode.txt

run2: parser
	./parser < smallsmp.txt

clean:
	rm -f *.o
	rm -f asm1.tab.c lex.yy.c asm1.tab.h y.output stack.hh parser.exe assembled.txt