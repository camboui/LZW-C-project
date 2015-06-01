/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/


#ifndef STRUCTURE_DICO_H
#define STRUCTURE_DICO_H

#include <stdint.h>
#define START 259
#define MAX_DICO 4096

typedef unsigned char Caractere;
typedef uint32_t Code;

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
typedef struct Dictionnaire Dictionnaire;
struct Dictionnaire
{
    struct un_noeud *racine;
};

#endif
