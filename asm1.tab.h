/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VALUE = 258,
     SEMICOLON = 259,
     COMMA = 260,
     REG = 261,
     ID = 262,
     LOAD = 263,
     STORE = 264,
     ADD = 265,
     SUB = 266,
     MUL = 267,
     EQ = 268,
     GT = 269,
     LT = 270,
     CLEAR = 271,
     INCR = 272,
     GOTO = 273,
     MEMLOC = 274,
     MEMLOCS = 275,
     LOC = 276,
     REGVAL = 277,
     END = 278,
     START = 279
   };
#endif
/* Tokens.  */
#define VALUE 258
#define SEMICOLON 259
#define COMMA 260
#define REG 261
#define ID 262
#define LOAD 263
#define STORE 264
#define ADD 265
#define SUB 266
#define MUL 267
#define EQ 268
#define GT 269
#define LT 270
#define CLEAR 271
#define INCR 272
#define GOTO 273
#define MEMLOC 274
#define MEMLOCS 275
#define LOC 276
#define REGVAL 277
#define END 278
#define START 279




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 12 "asm1.y"
{
  int intval;
  char* str;
  struct node* nd;
}
/* Line 1529 of yacc.c.  */
#line 103 "asm1.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

