/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

#ifndef GESTION_DICO_H
#define GESTION_DICO_H
#include "structure_dico.h"

/* Fonction d'initialisation du dictionnaire*/
Dictionnaire Init (void);


/* Libere tous les noeuds du dico */
void liberer_noeud(un_noeud * AC);


/* si renvoit de temp il a trouve le caractere il faut donc regarder le caractere suivant sinon on doit rajouter ce caractere dans le dico */
un_noeud* Est_Dans_Dico (Caractere wc, un_noeud* AC);


/* Permet d'ajouter un noeud au dictionnaire de compression */
void Ajouter_Noeud_Dico (Code code,char c,un_noeud* Place);


/* Retourne le caractère dont le père le plus éloigné, est un noeud de premier étage*/
Caractere get_first_letter(un_noeud *node);


/* Retourne la taille du tableau de noeuds */
int parcours_tab_code (un_noeud **tab_code);


/* Retourne le nombre de pere du noeud donné en paramètre */
int nb_pere (un_noeud* n);


/* Permet d'ajouter un noeud au dictionnaire de decompression */
void ajout_dico (Code code_actuel,Code code_suivant, un_noeud **tab_code,Dictionnaire d);

#endif
