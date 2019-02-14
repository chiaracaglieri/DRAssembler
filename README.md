# Tool for D-RISC assembler compilation,execution and analysis

This tool is concerned with the compilation of D-RISC assembler, its execution and the analysis of performance degradation when ran on a type of pipeline processor with parallelization of the EU.

## Getting Started

Unzip the DRAssembler.zip folder and open it in terminal.

### Prerequisites

Prior to executing the different commands within the tool, make sure you have Flex and Bison installed on your machine.

```
sudo apt-get update
sudo apt-get install flex
sudo apt-get install bison
```

### Installing

Compile the files within the main folder

```
make
```

Get the executables ready via the command

```
sudo make install
```

if you wish to install the commands to usr/local/bin

If you wish to specify the installation path just use:

```
make install DIR=myPath
```

## Running the tests

After installing it, the tool will provide you with three different commands: assembler, interpreter and analyzer. 
Run the assembler module on a .drisc file via the following command:

```
assembler -f test.drisc
```
Run the interpreter on a .o file via the following command, only after compiling it through the execution of the assembler module:

```
interpreter -f test.o
```

Run the analyzer on a .drisc file specifying the number of stages you wish to have for the parallel EU with the -s option:
```
analyzer -f test.drisc -s 2
```

## Built With

* [Flex](https://github.com/westes/flex) - The lexical analyzer
* [Bison](https://www.gnu.org/software/bison) - The parser generator
* [Doxygen](http://www.doxygen.nl/) - Used to generate the documentation

## Authors

* **Chiara Caglieri** 

