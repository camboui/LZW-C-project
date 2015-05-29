/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

/*Bourrer les avec des 0 quand trop grand.
ex : 12 bits à lire et fin de fichier, bourrer 3 bits à 0*/

/*
FONCTIONS NECESSAIRES :

	Est_Dans_Dico (wc, dico);
	Ajouter_Noeud_Dico (dico,code,wc,Place);

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gestion_dico.h"

void Ecrire_Code (FILE *f, Code code, char *bit_restants,int *nb_bit_restant, int nb_bit_code)
{
	int nb_case = (nb_bit_code+*nb_bit_restant)/8, i=0;
	uint8_t chaine [nb_case];
	int nb; int temp = nb_bit_code;

	if (*nb_bit_restant != 0){
		
		chaine[i] = ((code >> (nb_bit_code - (nb_case-i)*8) + *nb_bit_restant) + *bit_restants);
		nb= 255;
		nb = nb << (nb_bit_code - 8 + *nb_bit_restant);
		nb = ~nb;
		code = nb & code;
		temp = temp - 8 + *nb_bit_restant;
		i++;
		
	}
	while (i < nb_case){
		chaine[i] = code >> (nb_bit_code - ((nb_case-i)*8));
		nb= 255;
		nb = nb << (nb_bit_code - 8);
		nb = ~nb;
		code = nb & code;
		temp = temp - 8;
		i++;
	}
	*nb_bit_restant = temp;
	/*Les bits restants sont mis à gauche*/
	*bit_restants = code << (8 - temp);
	for (i=0; i<nb_case; i++){
		fprintf(f,"%c",chaine[i]);
	}
}

void Compression (char *nom_entree)
{
	Dictionnaire dico;
	dico = Init ();
	if (dico.racine == NULL)
	{
		fprintf (stderr,"Echec de l'initialisation du dictionnaire\n");
		exit (EXIT_FAILURE);
	}
	
	FILE *f_entree,*f_sortie;
	f_entree = fopen (nom_entree,"r");
	if (f_entree == NULL)
	{
		fprintf (stderr,"Echec de l'ouverture de f_entree en lecture\n");
		exit (EXIT_FAILURE);
	}
	
	char nom_sortie[strlen(nom_entree)+4];
	strcpy(nom_sortie,nom_entree);
	strcat(nom_sortie,".lzw");
	f_sortie = fopen (nom_sortie,"w+");
	if (f_sortie == NULL)
	{
		fprintf (stderr,"Echec de l'ouverture de f_sortie en écriture\n");
		exit (EXIT_FAILURE);
	}
	
	
	char c;
	int code = START;
	int nb_bit_code = 9;
	char bit_restants = 0;
	int nb_bit_restant = 0;

	un_noeud* Place_courant, *New_place;
	c=getchar();
	while (!feof(f_entree))
	{
			Place_courant = dico.racine;
			New_place = Est_Dans_Dico (c,Place_courant);
			while ( New_place != Place_courant){
				c=getchar();
				Place_courant = New_place;
				New_place = Est_Dans_Dico (c,Place_courant);
			}
			Ajouter_Noeud_Dico (code,c,Place_courant);/*Ajoute le Noeud qu'il soit Fils ou Frere*/
			code ++;
			if (code => pow(2,nb_bit_code)){
				nb_bit_code++;
			}
			Ecrire_Code (f_sortie,Place_courant -> code,&bit_restants,&nb_bit_restant,nb_bit_code)
	}
	fprintf(f_sortie,"%s",c);
	
	fclose(f_entree);
	fclose(f_sortie);
}

char *Ecrire_Code (Code c, int *bit_restants)
{
	int nb_case = c%8, i=0;
	char chaine [nb_case];
	while (i < nb_case){
		chaine[i] = code >> (c%2 - 8);
	}
	return chaine;
}



