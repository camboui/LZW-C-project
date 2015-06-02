/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure_dico.h"
#include <math.h>

/* Fonction d'initialisation du dictionnaire*/
Dictionnaire Init (void)
{
	unsigned int i=0;
	Dictionnaire d;
	d.racine=NULL;
	d.racine=malloc(sizeof(un_noeud));
	d.racine->pere=NULL;
	d.racine->frere=NULL;
	d.racine->car=0;
	d.racine->code=0;

	un_noeud*AC=NULL;
	
	if (d.racine==NULL)
	{
		fprintf (stderr,"Echec de l'allocation de la racine dans Init();\n");
		exit (EXIT_FAILURE);
	}
	
	AC=malloc(sizeof(un_noeud));
	d.racine->fils=AC;
	AC=d.racine->fils;

	
	for(i=0;i<256;i++)
	{

		AC->code = i;
		AC->car  = (Caractere) i;
		AC->pere=NULL;
		AC->fils=NULL;
		AC->frere=malloc(sizeof(un_noeud));
		if (AC->frere==NULL)
		{
			fprintf (stderr,"Echec de l'allocation frere dans Init();\n");
			exit (EXIT_FAILURE);
		}
		AC=AC->frere;
	}
	AC->frere=NULL;
	return d;
}

/* Libere tous les noeuds du dico */
void liberer_noeud(un_noeud * AC)
{
	if(AC!=NULL)
	{
		if(AC->fils!=NULL)
			liberer_noeud(AC->fils);
		if(AC->frere!=NULL)
			liberer_noeud(AC->frere);
		free(AC);
	}
}



/* si renvoit de temp il a trouve le caractere il faut donc regarder le caractere suivant sinon on doit rajouter ce caractere dans le dico */
un_noeud* Est_Dans_Dico (Caractere wc, un_noeud* AC)
{
	un_noeud* temp =NULL;
	temp = AC;
	int cpt=0;

		temp = temp -> fils;
	
	
	if (temp != NULL) {
		while(temp->frere != NULL && temp->car != wc)
		{
			temp=temp->frere;
			cpt++;
		}
		if (temp->car != wc){
			return AC;
		}
		else {
			return temp;
		}
	}
	else {
		return AC;
	}
}


/* Permet d'ajouter un noeud au dictionnaire de compression */
void Ajouter_Noeud_Dico (Code code,char c,un_noeud* Place)
{
	un_noeud *newN = malloc(sizeof(un_noeud));
	if (newN==NULL)
	{
		fprintf (stderr,"Echec de l'allocation du un_noeud dans Ajouter_Noeud_Dico();\n");
		exit (EXIT_FAILURE);
	}
	newN -> code = code ;
	newN -> car = (Caractere) c ;	
	newN -> frere = Place -> fils;
	newN -> fils =  NULL ;

	newN -> pere = Place ;
	Place -> fils =  newN ;
			
}


/* Retourne le premier caractere du mot */

Caractere get_first_letter(un_noeud *node)
{
	un_noeud *AC= node;
	while((AC -> pere) != NULL)
	{
		AC= AC -> pere;
	}
	return AC -> car;
}




/* Permet de nous donner le prochain code à mettre dans le dico */
int parcours_tab_code (un_noeud **tab_code)
{
	int i=0; 
	while(tab_code[i]!=NULL)
	{
		i++;
	}
	return i;
}

/* Retourne le nombre de pere du noeud donné en paramètre */
int nb_pere (un_noeud* n){
	
	un_noeud* temp;
	temp = n;
	int cpt = 0;
	while (temp -> pere != NULL){
		
		temp =temp -> pere;
		cpt++;
	}
	
	return cpt;

}

/* Permet d'ajouter un noeud au dictionnaire de decompression */
void ajout_dico (Code code_actuel,Code code_suivant, un_noeud **tab_code,Dictionnaire d)
{
	int lg_tab = parcours_tab_code (tab_code);
	un_noeud* new_noeud =malloc(sizeof(un_noeud));
	
	un_noeud* noeud_actuel=NULL;


	if(code_actuel >=START) // On regarde si le code actuel est 
	{
			noeud_actuel = tab_code[code_actuel-START];
	}
	else
	{
			noeud_actuel = d.racine->fils; 
			while(noeud_actuel -> frere != NULL && noeud_actuel->code != code_actuel)
			{
				noeud_actuel = noeud_actuel -> frere;
			}
	}

	if(code_suivant<START)
	{
		new_noeud -> car = (Caractere)code_suivant;
	}
	else if( tab_code[code_suivant-START] != NULL)
	{
		new_noeud -> car =  get_first_letter(tab_code[code_suivant-START]);
	}
	else
	{
		if(code_actuel<256)
			new_noeud -> car = noeud_actuel -> car;
		else 
		{
			new_noeud -> car = get_first_letter(tab_code[code_actuel-START]);
		}
	}
	new_noeud -> fils = NULL;
	new_noeud -> code = START + lg_tab;
	new_noeud -> pere = noeud_actuel;
	new_noeud -> frere = noeud_actuel -> fils;
	
	noeud_actuel -> fils = new_noeud;
	tab_code[lg_tab] = new_noeud;
}

