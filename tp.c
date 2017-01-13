#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "tp.h"
#include "tp_y.h"

extern int yyparse();
extern int yylineno;

/* Niveau de 'verbosite'.
 * Par defaut, n'imprime que le resultat et les messages d'erreur
 */
bool verbose = FALSE;

/* Generation de code ou pas. Par defaut, on produit le code */
bool noCode = FALSE;

/* Pose de points d'arret ou pas dans le code produit */
bool debug = FALSE;

/* code d'erreur a retourner */
int errorCode = NO_ERROR;

FILE *out; /* fichier de sortie pour le code engendre */

void printEt(short op); /* prototype du switch des etiquettes AST */


int main(int argc, char **argv) {
  int fi;
  int i, res;

  out = stdout;
  for(i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'd': case 'D':
	debug = TRUE; continue;
      case 'v': case 'V':
	verbose = TRUE; continue;
      case 'e': case 'E':
	noCode = TRUE; continue;
      case '?': case 'h': case 'H':
	fprintf(stderr, "Appel: tp -v -e -d -o file.out programme.txt\n");
	exit(USAGE_ERROR);
      case'o':
	  if ((out= fopen(argv[++i], "w")) == NULL) {
	    fprintf(stderr, "erreur: Cannot open %s\n", argv[i]);
	    exit(USAGE_ERROR);
	  }
	break;
      default:
	fprintf(stderr, "Option inconnue: %c\n", argv[i][1]);
	exit(USAGE_ERROR);
      }
    } else break;
  }

  if (i == argc) {
    fprintf(stderr, "Fichier programme manquant\n");
    exit(USAGE_ERROR);
  }

  if ((fi = open(argv[i++], O_RDONLY)) == -1) {
    fprintf(stderr, "erreur: Cannot open %s\n", argv[i-1]);
    exit(USAGE_ERROR);
  }

  /* redirige l'entree standard sur le fichier... */
  close(0); dup(fi); close(fi);

  res = yyparse();
  if (out != NIL(FILE) && out != stdout) fclose(out);
  return res ? SYNTAX_ERROR : errorCode;
}


void setError(int code) {
  errorCode = code;
  if (code != NO_ERROR) { noCode = TRUE; /*  abort(); */}
}


/* yyerror:  fonction importee par Bison et a fournir explicitement. Elle
 * est appelee quand Bison detecte une erreur syntaxique.
 * Ici on se contente d'un message minimal.
 */
void yyerror(char *ignore) {
  printf("erreur de syntaxe: Ligne %d\n", yylineno);
  setError(SYNTAX_ERROR);
}


/* Tronc commun pour la construction d'arbre */
TreeP makeNode(int nbChildren, short op) {
  TreeP tree = NEW(1, Tree);
  tree->op = op;
  tree->nbChildren = nbChildren;
  tree->u.children = nbChildren > 0 ? NEW(nbChildren, TreeP) : NIL(TreeP);
  return(tree);
}


/* Construction d'un arbre a nbChildren branches, passees en parametres */
TreeP makeTree(short op, int nbChildren, ...) {
  va_list args;
  int i;
  TreeP tree = makeNode(nbChildren, op); 
  va_start(args, nbChildren);
  for (i = 0; i < nbChildren; i++) { 
    tree->u.children[i] = va_arg(args, TreeP);
  }
  va_end(args);
  return(tree);
}

/*MethodeP makeMethode() {
	

}*/


void printTree(TreeP my_tree, int prof) {
	int i;
	for (i=0; i < prof; i++) {
        fputs("|_ ", stdout);
    }
	
	printEt(my_tree->op);
	printf("\n");
	for(i=0; i<my_tree->nbChildren; i++) {
		if (my_tree->u.children[i] != NIL(Tree))
			printTree(my_tree->u.children[i], prof+1);
	}
}


/* Retourne le rankieme fils d'un arbre (de 0 a n-1) */
TreeP getChild(TreeP tree, int rank) {
  if (tree->nbChildren < rank -1) { 
    fprintf(stderr, "Incorrect rank in getChild: %d\n", rank);
    abort();
  }
  return tree->u.children[rank];
}


void setChild(TreeP tree, int rank, TreeP arg) {
  if (tree->nbChildren < rank -1) { 
    fprintf(stderr, "Incorrect rank in getChild: %d\n", rank);
    abort();
  }
  tree->u.children[rank] = arg;
}


/* Constructeur de feuille dont la valeur est une chaine de caracteres */
TreeP makeLeafStr(short op, char *str) {
  TreeP tree = makeNode(0, op);
  tree->u.str = str;
  return tree;
}


/* Constructeur de feuille dont la valeur est un entier */
TreeP makeLeafInt(short op, int val) {
  TreeP tree = makeNode(0, op); 
  tree->u.val = val;
  return(tree);
}

TreeP makeLeafLVar(short op, VarDeclP lvar) {
  TreeP tree = makeNode(0, op); 
  tree->u.lvar = lvar;
  return(tree);
}



/* Switch d'affichage des elements pour l'AST - Voir son utilite */
void printEt(short op) {
	switch (op) {
		case 1:
			printf ("NE");
			break;
		case 2: 
			printf("EQ");
			break;
		case 3:
			printf("LT");
			break;
		case 4:
			printf("LE");
			break;
		case 5:
			printf("GT");
			break;
		case 6:
			printf("GE");
			break;
		case 7:
			printf("ECLA");
			break;
		case 8:
			printf("ECLI");
			break;
		case 9:
			printf("ECLS");
			break;
		case 10:
			printf("ESUP");
			break;
		case 11:
			printf("LCO");
			break;
		case 12:	
			printf("EXTO");
			break;
		case 13:
			printf("ECC");
			break;
		case 14:
			printf("LDCO");
			break;
		case 15:
			printf("LDC");
			break;
		case 16:
			printf("ECHAMP");
			break;
		case 17:
			printf("VIAA");
			break;
		case 18:
			printf("EBLOC");
			break;
		case 19:
			printf("IDAC");
			break;
		case 20:
			printf("LPARO");
			break;
		case 21:
			printf("LPAR");
			break;
		case 22:
			printf("PAR");
			break;
		case 23:
			printf("AEO");
			break;
		case 24:
			printf("LDMET");
			break;
		case 25:
			printf("EDMET1");
			break;
		case 26:
			printf("EDMET2");
			break;
		case 27:
			printf("EILEO");
			break;
		case 28:
			printf("EAPPC");
			break;
		case 29:
			printf("EOVER");
			break;
		case 30:
			printf("ECHAIN");
			break;
		case 31:
			printf("ESEL");
			break;
		case 32:
			printf("EPEXP");
			break;
		case 33:
			printf("ECAST");
			break;
		case 34:
			printf("EINSTA");
			break;
		case 35:
			printf("EENV");
			break;
		case 36:
			printf("EOPER");
			break;
		case 37:
			printf("EEXPO");
			break;
		case 38:
			printf("LEXP");
			break;
		case 39:
			printf("ECONST");
			break;
		case 40:
			printf("ENEW");
			break;
		case 41:
			printf("EADD");
			break;
		case 42:
			printf("EMINUS");
			break;
		case 43:
			printf("EMUL");
			break;
		case 44:
			printf("EQUO");
			break;
		case 45:
			printf("APPC");
			break;
		case 46:
			printf("CONST");
			break;
		case 47:	
			printf("EAND");
			break;
		case 48:
			printf("ECAE");
			break;
		case 49:
			printf("EEXP");
			break;
		case 50:
			printf("ERES");
			break;
		case 51:
			printf("ERET");
			break;
		case 52:
			printf("EAFF");
			break;
		case 53:
			printf("ITE");
			break;
		case 54:
			printf("ILINSTO");
			break;
		case 55:
			printf("LINST");
			break;
		case 56:
			printf("EINST");
			break;
		case 57:
			printf("EIB");
			break;
		case 58:
			printf("LINSTO");
			break;
		case 59:
			printf("EIS");
			break;
		case 60:
			printf("EEXPA");
			break;
		case 61:
			printf("EEXPI");
			break;
		case 62:
			printf("IDVAR");
			break;
		case 63:
			printf("ETHIS");
			break;
		default:
			printf("ETIQUETTE NON EXISTANTE !");
			break;
	}
}





/**
**
**
* FONCTION A VERIFIER ET/OU COMPLETER POUR VERIFIER LES ELEMENTS DANS L ARBRE
* CF TP PARTIE 2
**
**/

/* Verifie que nouv n'apparait pas deja dans list. l'ajoute en tete et
 * renvoie la nouvelle liste
 */
/*
VarDeclP addToScope(VarDeclP list, VarDeclP nouv) {
}
*/

/* Avant evaluation, verifie si tout identificateur qui apparait dans tree a
 * bien ete declare (dans ce cas il doit etre dans la liste lvar).
 * On impose que ca soit le cas y compris si on n'a pas besoin de cet
 * identificateur pour l'evaluation, comme par exemple x dans
 * begin if 1 = 1 then 1 else x end
 * Le champ 'val' de la structure VarDecl n'est pas significatif
 * puisqu'on n'a encore rien evalue.
 */
/*
bool checkScope(TreeP tree, VarDeclP lvar) {
}
*/



