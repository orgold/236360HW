%{
/* Declarations section */
#include <stdio.h>
#include "parser.tab.hpp"

%}

%option yylineno
%option noyywrap


%%
"true" 				return TRUE;
"false"				return FALSE;
"void" 				return VOID;
"int" 				return INT;
"byte" 				return BYTE;
"b" 				return B;
"bool" 				return BOOL;
"and" 				return AND;
"or" 				return OR;
"not" 				return NOT;
","					return COMMA;
":"					return COLON;
";"					return SC;
"="					return ASSIGN;
"return"			return RETURN;
"break"				return BREAK;
"if"				return IF;
"else"				return ELSE;
"while"				return WHILE;
"switch"			return SWITCH;
"case"				return CASE;
[+-] 				return BINOP2;
[*/]				return BINOP1;
0|[1-9][0-9]* 		return NUM;
\(					return LPAREN;
\)					return RPAREN;
\{					return LBRACE;
\}					return RBRACE;
[ \t\n\r]			;
"=="|"!="			return RELOP2;
"<"|">"|"<="|">="	return RELOP1;
[a-zA-Z][a-zA-Z0-9]* return ID;
\"([^\n\r\"\\]|\\[rnt"\\])+\" return STRING;
.					printf("Error: can't match %s\n", yytext);

%%

