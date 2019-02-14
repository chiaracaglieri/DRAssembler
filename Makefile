DIR?= "/usr/local/bin"
all:
	bison -d -t asm.y
	g++ -c asm.tab.c
	flex asm.l
	g++ -c lex.yy.c
	g++ -c parse_util.cpp
	g++ -c parser.cpp
	g++ -g -o pars asm.tab.o lex.yy.o parser.o parse_util.o
	g++ -c int_util.cpp
	g++ -c interpreter.cpp
	g++ -g -o int interpreter.o int_util.o
	g++ -c dep_util.cpp
	bison -d -t dep.y
	g++ -c dep.tab.c
	g++ -c dependencies.cpp
	g++ -g -o dep dependencies.o parse_util.o lex.yy.o dep.tab.o dep_util.o

install:
	mkdir -p $(DIR)
	cp pars $(DIR)
	cp int $(DIR)
	cp dep $(DIR)
	cp assembler $(DIR)
	cp interpreter $(DIR)
	cp analyzer $(DIR)
	chmod +x $(DIR)/assembler
	chmod +x $(DIR)/interpreter
	chmod +x $(DIR)/analyzer

setup: all install

clean:
	rm -f *.o *.tab.c *.yy.c *.tab.h
	rm -f asm1.output stack.hh int pars dep
	cd test; rm -f *.o
