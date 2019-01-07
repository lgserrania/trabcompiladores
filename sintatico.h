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

#ifndef YY_YY_SINTATICO_H_INCLUDED
# define YY_YY_SINTATICO_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_PROGRAMA = 258,
    T_INICIO = 259,
    T_FIM = 260,
    T_IDENTIF = 261,
    T_DECLARACAO = 262,
    T_VARIAVEL = 263,
    T_VARIAVEIS = 264,
    T_TIPO = 265,
    T_LISTACOMANDOS = 266,
    T_ENTRADASAIDA = 267,
    T_LEIA = 268,
    T_ESCREVA = 269,
    T_ENQTO = 270,
    T_FACA = 271,
    T_FIMENQTO = 272,
    T_SE = 273,
    T_ENTAO = 274,
    T_SENAO = 275,
    T_FIMSE = 276,
    T_ATRIB = 277,
    T_VEZES = 278,
    T_DIV = 279,
    T_MAIS = 280,
    T_MENOS = 281,
    T_MAIOR = 282,
    T_MENOR = 283,
    T_IGUAL = 284,
    T_E = 285,
    T_OU = 286,
    T_V = 287,
    T_F = 288,
    T_NUMERO = 289,
    T_TEXTO = 290,
    T_NAO = 291,
    T_ABRE = 292,
    T_FECHA = 293,
    T_LOGICO = 294,
    T_INTEIRO = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SINTATICO_H_INCLUDED  */
