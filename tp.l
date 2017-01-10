Chiffre [0-9]
Entier {Chiffre}+
Lower	[a-z]
Upper	[A-Z]
Lettre	[a-zA-Z]
LC 	({Lettre}|{Chiffre})
NomClasse ({Upper}&{LC}*)
Delim   [ \t\n\r]
Symbole [&+\-*/();:,.{}]
BS      \\.
PasBSG 	[^\\"]
Guill		"\""
InterieurChaine (({PasBSG}|{BS})*)
Star  		"*"
NotStar		[^*]
NotStarPar      [^*/]

%{
#include <string.h>
#include <strings.h>
#include "tp.h"
#include "tp_y.h"

extern char *strdup(char *);
extern YYSTYPE yylval;
extern int yylineno;
extern int yylex();

int keywordOrId() {
	if (! strcmp(yytext, "is")) return IS;
	if (! strcmp(yytext, "class")) return CLASS;
	if (! strcmp(yytext, "var")) return VAR;
	if (! strcmp(yytext, "extends")) return EXTENDS;
	if (! strcmp(yytext, "def")) return DEF;
	if (! strcmp(yytext, "override")) return OVERRIDE;
	if (! strcmp(yytext, "return")) return RETURN;
	if (! strcmp(yytext, "as")) return AS;
  	if (! strcmp(yytext, "if")) return IF;
  	if (! strcmp(yytext, "then")) return THEN;
  	if (! strcmp(yytext, "else")) return ELSE;

	yylval.S = strdup(yytext);
	return(Id);	
}

void lexError(char c) {
	fprintf(stderr, "\tErreur: Caractere invalide '%c' a la ligne %d\n", c, yylineno);
	setError(LEXICAL_ERROR);
}
%}

%%
"/*"({NotStar}|{Star}+{NotStarPar})*{Star}*"*/" { return(COMMENT); }
{Guill}{InterieurChaine}{Guill} { yylval.S = strdup(yytext); return(ChaineString); }
{Entier} 		{ yylval.I = atoi(yytext); return(Cste); }
{Symbole}		{ return(yytext[0]); }
{Lower}{LC}*	{ return keywordOrId(); }
{Upper}{LC}* 	{ yylval.S = strdup(yytext); return (Classe); }
{Delim}+		{ }
":="			{ return(AFF); }
"="				{ yylval.C = EQ; return(RelOp); }
"<>"			{ yylval.C = NE; return(RelOp); }
"<="			{ yylval.C = LE; return(RelOp); }
">="			{ yylval.C = GE; return(RelOp); }
">"				{ yylval.C = GT; return(RelOp); }
"<"				{ yylval.C = LT; return(RelOp); }
.				{ lexError(yytext[0]); }

%%
