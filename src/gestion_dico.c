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
	un_noeud*AC=NULL;
	d.racine=NULL;
	d.racine=malloc(sizeof(un_noeud));
	if (d.racine==NULL)
	{
		fprintf (stderr,"Echec de l'allocation de la racine dans Init();\n");
		exit (EXIT_FAILURE);
	}
	
	d.racine->pere=NULL;
	d.racine->frere=NULL;
	d.racine->car=0;
	d.racine->code=0;
	
	AC=malloc(sizeof(un_noeud));
	if (AC==NULL)
	{
		fprintf (stderr,"Echec de l'allocation du noeud dans Init();\n");
		exit (EXIT_FAILURE);
	}
	
	d.racine->fils=AC;
	AC=d.racine->fils;

	for(i=0;i<256;i++)/*On ajoute tous les noeuds de premiers 1 étage (=étage de base)*/
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

	if (temp ==NULL || temp -> fils==NULL)
	{
		fprintf (stderr,"Erreur : temp ==NULL || temp -> fils==NULL dans Est_dans_dico();\n");
		exit (EXIT_FAILURE);
	}
	temp = temp -> fils;

	
	if (temp != NULL) {
		while(temp->frere != NULL && temp->car != wc)/*On se place sur le noeud correspondant au caractère*/
		{
			temp=temp->frere;
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


/* Retourne le caractère dont le père le plus éloigné, est un noeud de premier étage*/
Caractere get_first_letter(un_noeud *node)
{
	un_noeud *AC= node;
	while((AC -> pere) != NULL)
	{
		AC= AC -> pere;
	}
	return AC -> car;
}




/* Retourne la taille du tableau de noeuds */
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
	un_noeud* new_noeud = NULL;
	un_noeud* noeud_actuel=NULL;
	new_noeud = malloc(sizeof(un_noeud));
	if (new_noeud==NULL)
	{
		fprintf (stderr,"Echec de l'allocation de new_noeud dans  ajout_dico ();\n");
		exit (EXIT_FAILURE);
	}

	if(code_actuel >=START) /*Si le code n'est pas un code de base, on prend le noeud correspondant dans le tableau*/ 
	{
		noeud_actuel = tab_code[code_actuel-START];
	}
	else	/*Sinon on cherche le noeud correspondant au code*/
	{
		noeud_actuel = d.racine->fils; 
		while(noeud_actuel -> frere != NULL && noeud_actuel->code != code_actuel)
		{
			noeud_actuel = noeud_actuel -> frere;
		}
	}

	if(code_suivant<START) /*Si c'est un caractère basique on l'affecte directement*/
	{
		new_noeud -> car = (Caractere)code_suivant;
	}
	else if( tab_code[code_suivant-START] != NULL)/*Sinon on cherche le caractère qui lui est associé*/
	{
		new_noeud -> car =  get_first_letter(tab_code[code_suivant-START]);
	}
	else/*Si le code n'est pas déjà connu*/
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

