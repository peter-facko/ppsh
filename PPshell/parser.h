/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MNT_D_PROGRAMMING_PPSHELL_PPSHELL_PARSER_H_INCLUDED
#define YY_YY_MNT_D_PROGRAMMING_PPSHELL_PPSHELL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "main.y"

#include <stdio.h>
#include "main.h"

#line 54 "/mnt/d/Programming/PPshell/PPshell/parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype
{
	YYEMPTY = -2,
	YYEOF = 0,	   /* "end of file"  */
	YYerror = 256, /* error  */
	YYUNDEF = 257, /* "invalid token"  */
	SEMIC = 258,   /* SEMIC  */
	IDF = 259,	   /* IDF  */
	REDIRI = 260,  /* REDIRI  */
	REDIROT = 261, /* REDIROT  */
	REDIROA = 262, /* REDIROA  */
	PIPE = 263,	   /* PIPE  */
	EOL = 264	   /* EOL  */
};
typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
	command_t command; /* command  */
	command_t
		command_arguments_redirections;	 /* command_arguments_redirections  */
	pipeline_t pipeline;				 /* pipeline  */
	pipelines_t pipelines_pure;			 /* pipelines_pure  */
	redirection_t redirection;			 /* redirection  */
	redirection_type_t redirection_type; /* redirection_type  */
	redirections_t redirections;		 /* redirections  */
	string_t IDF;						 /* IDF  */
	string_t argument;					 /* argument  */
	string_t command_path;				 /* command_path  */

#line 91 "/mnt/d/Programming/PPshell/PPshell/parser.h"
};
typedef union YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

int yyparse(void);

#endif /* !YY_YY_MNT_D_PROGRAMMING_PPSHELL_PPSHELL_PARSER_H_INCLUDED  */
