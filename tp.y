/* attention: NEW est defini dans tp.h Utilisez un autre token */
%token IS CLASS VAR EXTENDS DEF OVERRIDE RETURN AS IF THEN ELSE AFF COMMENT
%token<S> Id Classe ChaineString
%token<I> Cste
%token<C> RelOp


%{
#include "tp.h"
#include "tp_y.h"

extern int yylex();
extern void yyerror(char *);
%}

%%
Prog : classLOpt block
;

classLOpt:
;

block:
;
