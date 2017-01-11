#include <stdlib.h>


#define TRUE 1
#define FALSE 0


typedef unsigned char bool;

#define NEW(howmany, type) (type *) calloc((unsigned) howmany, sizeof(type))
#define NIL(type) (type *) 0

/* Etiquettes additionnelles pour les arbres de syntaxe abstraite.
 * Certains tokens servent directement d'etiquette. Attention ici a ne pas
 * donner des valeurs identiques a celles des tokens.
 */
#define NE	1
#define EQ	2
#define LT	3
#define LE	4
#define GT	5
#define GE	6

/* Codes d'erreurs */
#define NO_ERROR	0
#define USAGE_ERROR	1
#define LEXICAL_ERROR	2
#define SYNTAX_ERROR    3
#define CONTEXT_ERROR	40	/* default value for this stage */
#define DECL_ERROR	41	/* more precise information */
#define TYPE_ERROR	42
#define EVAL_ERROR	50
#define UNEXPECTED	10O

typedef struct _varDecl {
  char *name;
  struct _varDecl *next;
} VarDecl, *VarDeclP;

/* la structure d'un arbre (noeud ou feuille) */
typedef struct _Tree {
  short op;         /* etiquette de l'operateur courant */
  short nbChildren; /* nombre de sous-arbres */
  union {
    char *str;      /* valeur de la feuille si op = Id ou STR */
    int val;        /* valeur de la feuille si op = Cste */
    VarDeclP lvar;
    struct _Tree **children; /* tableau des sous-arbres */
  } u;
} Tree, *TreeP;


typedef union
      { char *S;
        char C;
	int I;
	TreeP pT;
	VarDeclP pV;
} YYSTYPE;

struct _Classe;
typedef struct _Classe Classe, *ClasseP;
struct _Methode;
typedef struct _Methode Methode;
struct _Parametre;
typedef struct _Parametre Parametre;

/* MEMO
 * Classe maClasse; // -> Pointeur
 * Classe monPointeur = &maClasse // -> Objet */

struct _Classe {
	char *nom;					/* Nom de la classe */
	ClasseP herite;		/* Pointeur sur la classe hérité */
	Parametre **parametres;		/* Liste de parametres */
	Methode **methodesClasse;	/* Liste methodes de classe */
	Methode *constructeur; 		/* Constructeur */
	/* ... Pas terminé, possibilité de rajouter des choses */
	
};

struct _Methode {
	char *typeR;				/* Type de retour de la methode */
	char *nom;					/* Nom de la méthode */
	bool surcharge;				/* La méthode est une surcharge d'une autre */
	Parametre **parametre;		/* Liste de parametre de la méthode */
	TreeP *CorpsFonction;		/* Abre d expression representant la fonciton */
	/* ... Pas terminé, possibilité de rajouter des choses */
};


struct _Parametre{
	char *nom;					/* Nom du parametre */
	ClasseP type;				/* Nom du type du parametre */
	TreeP expressions;			/* Expressions du parametre sous forme d'abre */
								/* Ex: x: Integer := y + 5 + 2 ou x: Integer := 2 */
};

#define YYSTYPE YYSTYPE
