/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/


#ifndef STRUCTURE_DICO_H
#define STRUCTURE_DICO_H


#include <stdint.h>

typedef uint8_t Caractere;
typedef int Code;

/*Un noeud est défini par 2 variables et 3 pointeurs de noeud*/
typedef struct un_noeud un_noeud;
struct un_noeud
{
    Caractere car;
    Code code;
    struct un_noeud *pere;
    struct un_noeud *frere; /*Un frere possède le même préfixe mais pas le même suffixe par rapport à ses autres frères*/
    struct un_noeud *fils;
};
/*Définition d'une liste de noeuds*/
typedef un_noeud* Dictionnaire;

/*Initialisation d'un dictionnaire*/
Dictionnaire Init (void);

/*Ajout d'un frère en queue*/
Dictionnaire Ajout_Frere (Dictionnaire d);

/*Ajout d'un fils*/
Dictionnaire Ajout_Fils (Dictionnaire d);

/*Parcours du dictionnaire.
  Renvoie Null si le caractère est dans la liste
  Renvoie la Queue de cette liste sinon*/
Dictionnaire Est_Dans_Dico (Caractere car, Dictionnaire d);

/*Renvoie le code associé à sa séquence*/
Code SequenceVersCode (Caractere *sequence, int longueur);

/*renvoie séquence et affecte la longueur associée à code*/
/*Code *CodeVersChaine (Code code, int *longueur);*/

/*Compresse le fichier dont le nom est donné en paramètre*/
void Compression (char *nom_fichier);

/*Compresse le fichier dont le nom est donné en paramètre*/
void Decompression (char *nom_fichier);

#endif
