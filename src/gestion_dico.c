/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/
/*
FONCTIONS A AJOUTER :

	Retourne null si présent dans le dico, sinon retourne le un_noeud où il faut ajouter le un_noeud
	Est_Dans_Dico (wc, dico);
	
	Ajoute le Noeud qu'il soit Fils ou Frere
	Ajouter_Noeud_Dico (dico,code,wc,Place);
	
	get_code permet de recuperer le code du prochain caractere a decoder et utilise les bit en trop du code precedent
	Il rajoutera aussi les nouveaux codes
	code = get_code(*f_entree,*bit_restant,nb_bit_code, *tab_code);
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure_dico.h"
#include <math.h>

Dictionnaire Init (void)
{
	int i=0;
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

	
	for(i=0;i<START;i++)
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







/* si renvoit de temp il a trouve le caractere il faut donc regarder le caractere suivant sinon on doit rajouter ce caractere dans le dico*/
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


/*
  this function gives the higher node in a dictionnary
  param : a node
  return a caractere
*/

Caractere get_first_letter(un_noeud *node)
{
	un_noeud *AC= node;
		while((AC -> pere) != NULL)
	      	{
			AC= AC -> pere;
	      	}
	    	return AC -> car;
}

Code Recherche_code (int *bit_restant, int *nb_bit_restant, int nb_bit_code, int *chaine){


	Code res =0;
	int i = 0, alire =0;
	Caractere c;
	
	if(*bit_restant>=pow(2,*nb_bit_restant))
		{
			printf("\n NON");
			exit(0);
		}
	
	alire = nb_bit_code - *nb_bit_restant;
	
	res = *bit_restant << alire;
	
	while (alire>0){
		*bit_restant = chaine[i];
		i++;
		if(alire>=8){
			alire-=8;
			res = res | (*bit_restant << alire);
			*bit_restant = 0;
			*nb_bit_restant = 0;
		}
		else {
			*nb_bit_restant = 8 - alire;
			res = res | (*bit_restant >> (8-alire));
			c=*bit_restant;
			c = (c << alire);
			*bit_restant = (c >> alire);
			if (*bit_restant < 0){
				*bit_restant = -*bit_restant;
			}
			alire =0;
		}
		
	}
	return res;

}


int parcours_tab_code (un_noeud **tab_code)
{
	int i=0; 
	while(tab_code[i]!=NULL)
	{
		i++;
	}
	return i;
}

void Affichage (int *ch, int nb_case){
	int i;
	for(i=0; i<nb_case; i++){
		printf("\n%i ",ch[i]);
	}
	
	
}



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

void Afficher_chaine_de (un_noeud *lettre){

	int lg;
	char* chaine;
	
	lg = nb_pere(lettre);
					
	chaine = malloc(lg*sizeof(char));
	
	while (lettre != NULL && lg >= 0){
		chaine[lg]=lettre->car;
		lg--;
		lettre = lettre -> pere;
	}
	printf("-%s-\n",chaine);


}

Code get_code (FILE *f, int *bit_restant, int *nb_bit_restant, int nb_bit_code,int executer_retour)
{
	int nb_case = 1,alire = (nb_bit_code - *nb_bit_restant),i;
	while (alire>8){
		alire-=8;
		nb_case++;
	}
	
	int car_lus[nb_case];
	
	for(i=0;i<nb_case;i++){
		car_lus[i] = fgetc(f);
	}

	if(executer_retour==1)
	{
		fseek(f,-nb_case, SEEK_CUR);
	}
	return Recherche_code (bit_restant,nb_bit_restant,nb_bit_code,car_lus);
}

void ajout_dico (Code code_actuel,Code code_suivant, un_noeud **tab_code,Dictionnaire d)
{
	int lg_tab = parcours_tab_code (tab_code);
	un_noeud* new_noeud =malloc(sizeof(un_noeud));
	
	un_noeud* noeud_actuel=NULL;

//printf("\n act : %i  suiv : %i",code_actuel, code_suivant);
	if(code_actuel >=START)
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
		new_noeud -> car = code_suivant;
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


