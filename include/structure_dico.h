/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/


#ifndef STRUCTURE_DICO_H
#define STRUCTURE_DICO_H


typedef struct un_noeud un_noeud;
struct un_noeud
{
    int caractere, code;
    struct un_noeud *pere;
    struct un_noeud *frere;
    struct un_noeud *fils;
};
typedef un_noeud* Dictionnaire;


#endif
