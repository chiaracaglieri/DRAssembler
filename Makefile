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
	g++ -g -o parser asm1.tab.o lex.yy.o main.o util.o

parse: parser
	./parser < samplecode.txt

state.o: state.cpp
	g++ -c state.cpp

interpreter.o: interpreter.cpp
	g++ -c interpreter.cpp

interpreter: interpreter.o state.o
	g++ -g -o interpreter interpreter.o state.o -lsfml-graphics -lsfml-window -lsfml-system

run: interpreter
	./interpreter

valgrind: parser
	valgrind --leak-check=full ./parser < samplecode.txt

clean:
	rm -f *.o
	rm -f asm1.tab.c lex.yy.c asm1.tab.h y.output stack.hh assembled.txt memory.txt registers.txt

