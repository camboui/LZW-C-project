/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

#ifndef GESTION_DICO_H
#define GESTION_DICO_H
#include "structure_dico.h"

Dictionnaire Init (void);

/* si renvoit de temp il a trouve le caractere il faut donc regarder le caractere suivant sinon on doit rajouter ce caractere dans le dico*/
un_noeud* Est_Dans_Dico (char wc, un_noeud* AC, Dictionnaire d);


void Ajouter_Noeud_Dico (Code code,char c,un_noeud* Place);


/*
  this function gives the higher node in a dictionnary
  param : a node
  return a caractere
*/
Caractere get_first_letter(un_noeud *node);


Code Recherche_code (int *bit_restant, int *nb_bit_restant, int nb_bit, char *chaine);


int parcours_tab_code (un_noeud **tab_code);


void Ajout_Dico_Decomp (Code code_second,un_noeud **tab_code,un_noeud *noeud_courant);

int nb_pere (un_noeud* n);

Code get_code(Dictionnaire d, FILE *f, int *bit_restant, int *nb_bit_restant, int nb_bit, un_noeud**tab_code);

#endif
