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

%type <pT> Classe AppelClasse ListClasseOpt ExtendsOpt ContenuClasse ListDeclChampOpt ListDeclChamp DeclChamp BlocOpt Param ListParamOpt ListParam AffExpressionOpt ListDeclMethodeOpt DeclMethode AppelMethode AppelClasseOpt Expression ListExpressionOpt ListExpression Instanciation ExpressionOperateur Cast Instruction ListInstructionOpt ListInstruction Bloc InterieurBloc Envoi Selection
%type <I> OverrideOpt

%{
#include "tp.h"
#include "tp_y.h"

extern int yylex();
extern void yyerror(char *);
%}

%%

Programme : ListClasseOpt Bloc  
{
    /* cf le tp corrigé
    il faut récupérer la liste des classes déclarés 
    Si cette evauation réussit, on evalue le Bloc en envoant a la fonction la 	liste de classe 
    
    exemple :
    ClasseDeclP lclasse = evalDecls($1);
    evalMain($3, lclasse);
    */
    
    printTree($2, 0);
}
;


/* Classe */

Classe : CLASS NOMCLASSE '(' ListParamOpt ')' ExtendsOpt BlocOpt IS '{' ContenuClasse '}'       
                            { $$ = makeTree(ECLA, 5, makeLeafStr(IDVAR, $2), $4, $6, $7, $10); }
;

AppelClasse : CLASSEINT     { $$ = makeLeafInt(APPC, ECLI); }
| CLASSESTRING              { $$ = makeLeafInt(APPC, ECLS); }
| NOMCLASSE                 { $$ = makeLeafStr(APPC, $1); }
;

ListClasseOpt : Classe ListClasseOpt    { $$ = makeTree(LCO, 2, $1, $2); }
|                                       { $$ = NIL(Tree); }
;

// on passe des expressions
ExtendsOpt : EXTENDS AppelClasse '(' ListExpressionOpt ')'   
                            { $$ = makeTree(EXTO, 2, $2, $4); }
|                           { $$ = NIL(Tree); }
;

ContenuClasse : ListDeclChampOpt ListDeclMethodeOpt
                                        { $$ = makeTree(ECC, 2, $1, $2); }
;

ListDeclChampOpt : DeclChamp ListDeclChampOpt  { $$ = makeTree(LDCO, 2, $1, $2); }
|                               { $$ = NIL(Tree); }
;

ListDeclChamp : DeclChamp ListDeclChamp { $$ = makeTree(LDC, 2, $1, $2); }
| DeclChamp                           {$$ = makeTree(ECHAMP, 2, $1, NIL(Tree)); }
;

DeclChamp : VAR ID ':' AppelClasse AffExpressionOpt ';'
                            { $$ = makeTree(VIAA, 3, makeLeafStr(IDVAR, $2), $4, $5); }
;

BlocOpt : Bloc  { $$ = makeTree(EBLOC, 2, NIL(Tree), $1); }
|               { $$ = NIL(Tree); }
;


/* Param */

Param : ID ':' AppelClasse          { $$ = makeTree(IDAC, 2, makeLeafStr(IDVAR, $1), $3); }
;

ListParamOpt : ListParam            { $$ = makeTree(LPARO, 2, NIL(Tree), $1); }
|                                   { $$ = NIL(Tree); }
;

ListParam : Param ',' ListParam     { $$ = makeTree(LPAR, 2, $1, $3); }
| Param                             { $$ = makeTree(PAR, 2, $1, NIL(Tree)); }
;

AffExpressionOpt : AFF Expression   { $$ = makeTree(AEO, 2, NIL(Tree), $2); }
|                                   { $$ = NIL(Tree); }
;


/* Méthode */


ListDeclMethodeOpt : DeclMethode ListDeclMethodeOpt 
                            { $$ = makeTree(LDMET, 2, $1, $2); }
|                           { $$ = NIL(Tree); }
;

DeclMethode : OverrideOpt DEF ID '(' ListParamOpt ')' ':' AppelClasse AFF Expression
        { $$ = makeTree(EDMET1, 5, $1, makeLeafStr(IDVAR, $3), $5, $8, $10); }
| OverrideOpt DEF ID '(' ListParamOpt ')' AppelClasseOpt IS Bloc
        { $$ = makeTree(EDMET2, 5, $1, makeLeafStr(IDVAR, $3), $5, $7, $9); }
;

AppelMethode : ID '(' ListExpressionOpt ')' 
                            { $$ = makeTree(EILEO, 2, makeLeafStr(IDVAR, $1), $3); }
;

AppelClasseOpt : ':' AppelClasse    { $$ = makeTree(EAPPC, 2, NIL(Tree), $2); }
|                                   { $$ = NIL(Tree); }
;

OverrideOpt : OVERRIDE  { $$ = TRUE; }
|                       { $$ = FALSE; }             
;


/* Expression */

Expression : Cste                   { $$ = makeLeafInt(ECONST, $1); }
| ChaineString                      { $$ = makeLeafStr(ECHAIN, $1); }
| Selection                         { $$ = makeTree(ESEL, 2, NIL(Tree), $1); }
| '(' Expression ')'                { $$ = makeTree(EPEXP, 2, NIL(Tree), $2); }
| Cast                              { $$ = makeTree(ECAST, 2, NIL(Tree), $1); }
| Instanciation                     { $$ = makeTree(EINSTA, 2, NIL(Tree), $1); }
| Envoi                             { $$ = makeTree(EENV, 2, NIL(Tree), $1); }
| ExpressionOperateur               { $$ = makeTree(EOPER, 2, NIL(Tree), $1); }
;

ListExpressionOpt : ListExpression  { $$ = makeTree(EEXPO, 2, NIL(Tree), $1); }
|                                   { $$ = NIL(Tree); } 
;

ListExpression : Expression ',' ListExpression
                                    { $$ = makeTree(LEXP, 2, $1, $3); }
| Expression                        { $$ = makeTree(EEXP, 2, NIL(Tree), $1); }
;

Instanciation : NEWC AppelClasse '(' ListExpressionOpt ')'
                                    { $$ = makeTree(ENEW, 2, $2, $4); }
;

ExpressionOperateur : Expression '+' Expression 
                                    { $$ = makeTree(EADD, 2, $1, $3); }
| Expression '-' Expression         { $$ = makeTree(EMINUS, 2, $1, $3); }
| Expression '*' Expression         { $$ = makeTree(EMUL, 2, $1, $3); }
| Expression '/' Expression         { $$ = makeTree(EQUO, 2, $1, $3); }
| '-' Expression %prec UNARY        { $$ = $2; }
| '+' Expression %prec UNARY
                    { $$ = makeTree(EMINUS, 2, makeLeafInt(CONST, 0), $2); }
| Expression '&' Expression         { $$ = makeTree(EAND, 2, $1, $3); } 
| Expression RelOp Expression       { $$ = makeTree($2, 2, $1, $3); } 
;

Cast : '(' AS AppelClasse ':' Expression ')' { $$ = makeTree(ECAE, 2, $3, $5);}
;


/* Instructions */

Instruction : Expression ';'           { $$ = makeTree(EEXP, 2, NIL(Tree), $1); }
| Bloc                                 {$$ = makeTree(EBLOC, 2, NIL(Tree), $1); }
| RETURN ';'                            { $$ = makeLeafStr(ERET, NIL(char)); }
| Selection AFF Expression ';'          { $$ = makeTree(EAFF, 2, $1, $3); }
| IF Expression THEN Instruction ELSE Instruction 
                                        { $$ = makeTree(ITE, 3, $2, $4, $6); }
;

ListInstructionOpt : Instruction ListInstructionOpt     {$$=makeTree(ILINSTO, 2, $1, $2);}
|                                                       { $$ = NIL(Tree); }
;

ListInstruction : Instruction ListInstruction  {$$ = makeTree(LINST, 2, $1, $2);}
| Instruction                           {$$ = makeTree(EINST, 2, $1, NIL(Tree)); }
;

/* Bloc */

Bloc : '{' InterieurBloc '}'    { $$ = makeTree(EIB, 2, NIL(Tree), $2); }
;

InterieurBloc : ListInstructionOpt  { $$ = makeTree(LINSTO, 2, NIL(Tree), $1); }
| ListDeclChamp IS ListInstruction  { $$ = makeTree(EIS, 2, $1, $3); }
;


/* Envoi et sélection */

Envoi : Expression '.' AppelMethode { $$ = makeTree(EEXPA, 2, $1, $3); }
;

Selection : Expression '.' ID   { $$ = makeTree(EEXPI, 2, $1, makeLeafStr(IDVAR, $3)); }
| ID               				{ $$ = makeLeafStr(IDVAR, $1); }
| THIS                          { $$ = makeLeafInt(IDVAR, ETHIS); }
| SUPER                         { $$ = makeLeafInt(IDVAR, ESUP); }
| RESULT                        { $$ = makeLeafInt(IDVAR, ERES); }
;


