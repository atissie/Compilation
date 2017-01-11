/* attention: NEW est defini dans tp.h Utilisez un autre token */
%token IS CLASS VAR EXTENDS DEF OVERRIDE RETURN AS IF THEN ELSE AFF NEWC CLASSEINT CLASSESTRING THIS SUPER RESULT
%token<S> ID NOMCLASSE ChaineString
%token<I> Cste
%token<C> RelOp

%right AFF
%nonassoc RelOp
%left '+' '-' '&'
%left '*' '/'
%right '.'
%nonassoc UNARY

%{
#include "tp.h"
#include "tp_y.h"

extern int yylex();
extern void yyerror(char *);
%}

%%

Programme : ListClasseOpt Bloc
;

/* Classe */

Classe : CLASS NOMCLASSE '(' ListParamOpt ')' ExtendsOpt BlocOpt IS '{' ContenuClasse '}'
;

AppelClasse : CLASSEINT
| CLASSESTRING
| NOMCLASSE
;

ListClasseOpt : ListClasse
| 
;

ListClasse : Classe ListClasse
| Classe
;

ExtendsOpt : EXTENDS AppelClasse '(' ListExpressionOpt ')' // on passe des expressions
|
;

ContenuClasse : ListDeclChampOpt ListDeclMethodeOpt 
;

ListDeclChampOpt : ListDeclChamp
|
;

ListDeclChamp : DeclChamp ListDeclChamp
| DeclChamp
;

DeclChamp : VAR ID ':' AppelClasse AffExpressionOpt ';'
;

BlocOpt : Bloc
|
;


/* Param */

Param : ID ':' AppelClasse
;

ListParamOpt : ListParam
|
;

ListParam : Param ',' ListParam
| Param
;

AffExpressionOpt : AFF Expression 
|
;


/* Méthode : */


ListDeclMethodeOpt : ListDeclMethode
|
;

ListDeclMethode : DeclMethode ListDeclMethode
| DeclMethode
;

DeclMethode : OverrideOpt DEF ID '(' ListParamOpt ')' ':' AppelClasse AFF Expression
| OverrideOpt DEF ID '(' ListParamOpt ')' AppelClasseOpt IS Bloc
;

AppelMethode : ID '(' ListExpressionOpt ')'
;

AppelClasseOpt : ':' AppelClasse
|
;

OverrideOpt : OVERRIDE
|
;


/* Expression */

Expression : Cste | ChaineString
| Selection
| '(' Expression ')'
| Cast
| Instanciation
| Envoi
| ExpressionOperateur
;

ListExpressionOpt : ListExpression
|
;

ListExpression : Expression ',' ListExpression
| Expression
;

Instanciation : NEWC AppelClasse '(' ListExpressionOpt ')'
;

ExpressionOperateur : Expression '+' Expression 	
| Expression '-' Expression 
| Expression '*' Expression      
| Expression '/' Expression
| '-' Expression %prec UNARY
| '+' Expression %prec UNARY
| Expression '&' Expression
| Expression RelOp Expression
;

Cast : '(' AS AppelClasse ':' Expression ')'
;


/* Instructions */

Instruction : Expression ';'
| Bloc
| RETURN ';'
| Selection AFF Expression ';'
| IF Expression THEN Instruction ELSE Instruction
;

ListInstructionOpt : ListInstruction
|
;

ListInstruction : Instruction ListInstruction
| Instruction
;


/* Bloc */

Bloc : '{' InterieurBloc '}'
;

InterieurBloc : ListInstructionOpt
| ListDeclChamp IS ListInstruction
;


/* Envoi et sélection */

Envoi : Expression '.' AppelMethode
;

Selection : Expression '.' ID
| ID | THIS | SUPER | RESULT
;


