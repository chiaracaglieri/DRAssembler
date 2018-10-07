DIR?= "/usr/local/bin"
all:
	bison -d -t asm1.y
	g++ -c asm1.tab.c
	flex asm1.l
	g++ -c lex.yy.c
	g++ -c util.cpp
	g++ -c parser.cpp
	g++ -g -o pars asm1.tab.o lex.yy.o parser.o util.o
	g++ -c state.cpp
	g++ -c interpreter.cpp
	g++ -g -o int interpreter.o state.o
	g++ -c util_dip.cpp
	bison -d -t dip.y
	g++ -c dip.tab.c
	g++ -c analyzer.cpp
	g++ -g -o dep analyzer.o util.o lex.yy.o dip.tab.o util_dip.o

install:
	mkdir -p $(DIR)
	cp pars $(DIR)
	cp int $(DIR)
	cp assembler $(DIR)
	cp interpreter $(DIR)
	chmod +x $(DIR)/assembler
	chmod +x $(DIR)/interpreter

test1: all
	./assembler test/test1.txt < test/test1.txt
	./interpreter
clean:
	rm -f *.o *.tab.c *.yy.c *.tab.h
	rm -f asm1.output stack.hh int pars dep
