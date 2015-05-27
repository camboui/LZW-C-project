/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/


#ifndef STRUCTURE_DICO_H
#define STRUCTURE_DICO_H



typedef struct {

	int ligne, colonne;
	
}emplacement_t;



typedef struct un_maillon un_maillon;
struct un_maillon
{
    maillon_t m;
    struct un_maillon *suc;
};
typedef un_maillon* liste_maillon;


#endif
