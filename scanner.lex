%{
/* Declarations section */
#include <stdio.h>
#include "parser.tab.hpp"

%}

%option yylineno
%option noyywrap


%%					
"true" 				{
					yylval.lineno=yylineno;
					return TRUE;
					}

"false"				{
					yylval.lineno=yylineno;
					return FALSE;
					}
"void" 				{
					yylval.lineno=yylineno;
					return VOID;
					}
"int" 				{
					yylval.lineno=yylineno;
					return INT;
					}
"byte" 				{
					yylval.lineno=yylineno;
					return BYTE;
					}
"b" 				{
					yylval.lineno=yylineno;
					return B;
					}
"bool" 				 {
					yylval.lineno=yylineno;
					return BOOL;
					}
"and" 				 {
					yylval.lineno=yylineno;
					return AND;
					}
"or" 				 {
					yylval.lineno=yylineno;
					return OR;
					}
"not" 				 {
					yylval.lineno=yylineno;
					return NOT;
					}
","					 {
					yylval.lineno=yylineno;
					return COMMA;
					}
":"					{
					yylval.lineno=yylineno;
					return COLON;
					}
";"					{
					yylval.lineno=yylineno;
					return SC;
					}
"="					{
					yylval.lineno=yylineno;
					return ASSIGN;
					}
"return"			{
					yylval.lineno=yylineno;
					return RETURN;
					}
"break"				{
					yylval.lineno=yylineno;
					return BREAK;
					}
"if"				{
					yylval.lineno=yylineno;
					return IF;
					}
"else"				{
					yylval.lineno=yylineno;
					return ELSE;
					}
"while"				{
					yylval.lineno=yylineno;
					return WHILE;
					}
"switch"			{
					yylval.lineno=yylineno;
					return SWITCH;
					}
"case"				{
					yylval.lineno=yylineno;
					return CASE;
					}
[+-] 				{
					yylval.lineno=yylineno;
					return BINOP2;
					}
[*/]				{
					yylval.lineno=yylineno;
					return BINOP1;
					}
0|[1-9][0-9]* 		{	
						yylval.lineno =yylineno;
						yylval.value = atoi(yytext);
						return NUM;
					}
\(					{
					yylval.lineno=yylineno;
					return LPAREN;
					}
\)					{
					yylval.lineno=yylineno;
					return RPAREN;
					}
\{					{
					yylval.lineno=yylineno;
					return LBRACE;
					}
\}					{
					yylval.lineno=yylineno;
					return RBRACE;
					}
[ \t\n\r]			;
"=="|"!="			{
					yylval.lineno=yylineno;
					return RELOP2;
					}
"<"|">"|"<="|">="	{
					yylval.lineno=yylineno;
					return RELOP1;
					}
[a-zA-Z][a-zA-Z0-9]* {
					yylval.idName=malloc(strlen(yytext));
					strcpy(yylval.idName,yytext);
					yylval.lineno=yylineno;
					return ID;
					}
\"([^\n\r\"\\]|\\[rnt"\\])+\" return STRING;
.					errorLex(yylineno);

%%

