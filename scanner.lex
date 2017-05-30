%{
/* Declarations section */
#include <stdio.h>
#include "parser.tab.hpp"

%}

%option yylineno
%option noyywrap


%%
"true" 	return TRUE;
"false"	return FALSE;
"void" 	return VOID;
"int" 	return INT;
"byte" 	return BYTE;
"b" 		return B;
"bool" 	return BOOL;
"and" 	return AND;
"or" 		return OR;
"not" 	return NOT;
[+-] 	return BINOP2;
[*/]	return BINOP1;
 [1-9][0-9]* return NUM;
\(		return LPAREN;
\)		return RPAREN;
[ \t\n\r]						;
.		printf("error");

%%

