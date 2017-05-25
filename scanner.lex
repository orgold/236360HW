%{

/* Declarations section */
#include <stdio.h>
void showToken(char *);

%}

%option yylineno
%option noyywrap
digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n ])

%%

{digit}+          			showToken("number");
{letter}+					showToken("word");
{letter}+@{letter}+\.com		showToken("email address");
{whitespace}				;
.		printf("Lex doesn't know what that is!\n");

%%

void showToken(char * name)
{
        printf("Lex found a %s, the lexeme is %s and its length is %d\n", name, yytext, yyleng);
}