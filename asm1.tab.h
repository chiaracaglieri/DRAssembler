/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_ASM1_TAB_H_INCLUDED
# define YY_YY_ASM1_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    VALUE = 258,
    SEMICOLON = 259,
    COMMA = 260,
    REG = 261,
    ID = 262,
    LOAD = 263,
    STORE = 264,
    MOVE = 265,
    ADD = 266,
    SUB = 267,
    MUL = 268,
    EQ = 269,
    GT = 270,
    LT = 271,
    EQ_0 = 272,
    LT_0 = 273,
    GT_0 = 274,
    GTE = 275,
    LTE = 276,
    GTE_0 = 277,
    LTE_0 = 278,
    CLEAR = 279,
    INCR = 280,
    DECR = 281,
    GOTO = 282,
    NOP = 283,
    MEMLOC = 284,
    MEMLOCS = 285,
    LOC = 286,
    REGVAL = 287,
    END = 288,
    START = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 12 "asm1.y" /* yacc.c:1909  */

  int intval;
  char* str;
  struct node* nd;

#line 95 "asm1.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ASM1_TAB_H_INCLUDED  */
