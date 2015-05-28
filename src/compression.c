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
#include "structure_dico.h"

void Compression (char *nom_entree)
{
	Dictionnaire dico = NULL;
	dico = Init ();
	if (dico == NULL)
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
	char wc[100]="\0",w[100]="\0";
	int code = START;

	Un_noeud* Place;
	while (!feof(f_entree))
	{
			c=getchar();
			strcat(wc,c);
			
			Place=Est_Dans_Dico (wc, dico);/*Retourne null si présent dans le dico*/
			if(Place==NULL)
			{
				strcpy(w,wc);
			}
			else
			{
				Ajouter_Noeud_Dico (dico,code,wc,Place);/*Ajoute le Noeud qu'il soit Fils ou Frere*/
				code ++;
				fprintf(f_sortie,"%s",w);
				strcpy(w,c);
			}
	}
	fprintf(f_sortie,"%s",w);
	
	fclose(f_entree);
	fclose(f_sortie);
}
