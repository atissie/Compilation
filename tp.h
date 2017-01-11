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
#define ECLA 7
#define ECLI 8
#define ECLS 9
#define ENCL 10
#define LCO 11
#define EXTO 12
#define ECC 13
#define LDCO 14
#define LDC 15
#define ECHAMP 16
#define VIAA 17
#define EBLOC 18
#define IDAC 19
#define LPARO 20
#define LPAR 21
#define PAR 22
#define AEO 23
#define LDMET 24
#define EDMET1 25
#define EDMET2 26
#define EILEO 27
#define EAPPC 28
#define EOVER 29
#define ECHAIN 30
#define ESEL 31
#define EPEXP 32
#define ECAST 33
#define EINSTA 34
#define EENV 35
#define EOPER 36
#define EEXPO 37
#define LEXP 38
#define ECONST 39
#define ENEW 40
#define EADD 41
#define EMINUS 42
#define EMUL 43
#define EQUO 44
#define CONST 46
#define EAND 47
#define ECAE 48
#define EEXP 49
#define ERET 51
#define EAFF 52
#define ITE 53
#define ILINSTO 54
#define LINST 55
#define EINST 56
#define EIB 57
#define LINSTO 58
#define EIS 59
#define EEXPA 60
#define EEXPI 61
#define IDVAR 62
#define ETHIS 63
#define ESUP 64
#define ERES 65
#define APPC 66

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


/* Prototypes des fonctions (partiellement) mises a disposition */

/* construction pour les AST */
TreeP makeLeafStr(short op, char *str); 	    /* feuille (string) */
TreeP makeLeafInt(short op, int val);	            /* feuille (int) */
TreeP makeTree(short op, int nbChildren, ...);	    /* noeud interne */

/* gestion des declarations et traitement de la portee */
VarDeclP addToScope(VarDeclP list, VarDeclP nouv);
VarDeclP declVar(char *name, TreeP tree, VarDeclP decls);

/* evaluateur d'expressions */
VarDeclP evalDecls (TreeP tree);
int eval(TreeP tree, VarDeclP decls);
int evalMain(TreeP tree, VarDeclP decls);
