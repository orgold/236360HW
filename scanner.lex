%{
/* Declarations section */
#include <stdio.h>
#include "hw4_table_utils.h"
#include "parser.tab.hpp"

int handle_default();
void allocate_value();
%}

%option yylineno
%option noyywrap


%%					
"true" 				return handle_default();
"false"				return handle_default();
"void" 				return handle_default();
"int" 				return handle_default();
"byte" 				return handle_default();
"b" 				return handle_default();
"bool" 				return handle_default();
"and" 				return handle_default();
"or" 				return handle_default();
"not" 				return handle_default();
","					return handle_default();
":"					return handle_default();
";"					return handle_default();
"="					return handle_default();
"return"			return handle_default();
"break"				return handle_default();
"if"				return handle_default();
"else"				return handle_default();
"while"				return handle_default();
"switch"			return handle_default();
"case"				return handle_default();
[+-] 				{
						yylval.lineno=yylineno;
						allocate_value();
						return BINOP2;
					}
[*/]				{
						yylval.lineno=yylineno;
						allocate_value();
						return BINOP1;
					}
0|[1-9][0-9]* 		{	
						yylval.lineno =yylineno;
						allocate_value();
						return NUM;
					}
\(					return handle_default();
\)					return handle_default();
\{					return handle_default();
\}					return handle_default();
[ \t\n\r]			;
"=="|"!="			{
						allocate_value();
						yylval.lineno=yylineno;
						return RELOP2;
					}
"<"|">"|"<="|">="	{
						allocate_value();
						yylval.lineno=yylineno;
						return RELOP1;
					}
[a-zA-Z][a-zA-Z0-9]* {
						yylval.idName=(char*)malloc(strlen(yytext));
						strcpy(yylval.idName,yytext);
						yylval.lineno=yylineno;
						return ID;
					}
\"([^\n\r\"\\]|\\[rnt"\\])+\" {
								allocate_value();
								return STRING;
							}


\/\/[^\r\n]*[\r|\n|\r\n]?         ;
.					{
					errorLex(yylineno);
					exit(1);
					}

%%
int handle_default()
{
	//default behavior
	yylval.lineno=yylineno;
	
	//different returned tokens
	if(0==strcmp(yytext,"true"))
		return TRUE;
	if(0==strcmp(yytext,"false"))
		return FALSE;
	if(0==strcmp(yytext,"void"))
		return VOID;
	if(0==strcmp(yytext,"int"))
		return INT;
	if(0==strcmp(yytext,"byte"))
		return BYTE;
	if(0==strcmp(yytext,"b"))
		return B;
	if(0==strcmp(yytext,"bool"))
		return BOOL;
	if(0==strcmp(yytext,"and"))
		return AND;
	if(0==strcmp(yytext,"or"))
		return OR;
	if(0==strcmp(yytext,"not"))
		return NOT;
	if(0==strcmp(yytext,","))
		return COMMA;
	if(0==strcmp(yytext,":"))
		return COLON;
	if(0==strcmp(yytext,";"))
		return SC;
	if(0==strcmp(yytext,"="))
		return ASSIGN;
	if(0==strcmp(yytext,"return"))
		return RETURN;
	if(0==strcmp(yytext,"break"))
		return BREAK;
	if(0==strcmp(yytext,"if"))
		return IF;
	if(0==strcmp(yytext,"else"))
		return ELSE;
	if(0==strcmp(yytext,"while"))
		return WHILE;
	if(0==strcmp(yytext,"switch"))
		return SWITCH;
	if(0==strcmp(yytext,"case"))
		return CASE;
	if(0==strcmp(yytext,"("))
		return LPAREN;
	if(0==strcmp(yytext,")"))
		return RPAREN;
	if(0==strcmp(yytext,"{"))
		return LBRACE;
	if(0==strcmp(yytext,"}"))
		return RBRACE;
}


void allocate_value()
{
	yylval.value=(char*)malloc(strlen(yytext));
	strcpy(yylval.value,yytext);
}