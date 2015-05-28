/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/
/*
FONCTIONS A AJOUTER :

	Retourne null si présent dans le dico, sinon retourne le noeud où il faut ajouter le noeud
	Est_Dans_Dico (wc, dico);
	
	Ajoute le Noeud qu'il soit Fils ou Frere
	Ajouter_Noeud_Dico (dico,code,wc,Place);
	
	get_code permet de recuperer le code du prochain caractere a decoder et utilise les bit en trop du code precedent
	Il rajoutera aussi les nouveaux codes
	code = get_code(*f_entree,*bit_restant,nb_bit, *tab_code);
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure_dico.h"


Dictionnaire Init (void)
{
	int i=0;
	Dictionnaire d;
	d.racine=NULL;
	d.racine=malloc(sizeof(un_noeud));
	un_noeud*AC=NULL;
	
	if (d.racine==NULL)
	{
		fprintf (stderr,"Echec de l'allocation de la racine dans Init();\n");
		exit (EXIT_FAILURE);
	}
	
	AC=d.racine;
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


void Lire_Dico(Dictionnaire d)
{
	un_noeud*AC=d.racine;
	if (AC!=NULL)
	{
		while (AC->frere !=NULL)
		{
			printf("\n%i %c",AC->code, AC->car);
			AC = AC->frere;
		}
	}
}

un_noeud* Est_Dans_Dico (char *wc, Dictionnaire d)
{
	un_noeud*AC=d.racine;
	int i=0, taille = strlen(wc)-1;
	while (i != taille)
	{
		while(AC->frere !=NULL && AC->car!=wc[i])
		{
			AC=AC->frere;
		}
		if(AC->fils !=NULL  && AC->car==wc[i])
		{
			AC=AC->fils;
		}
		else
		{
			break;
		}
		i++;
	}
	
	if(AC->car==wc[i-1])/*Si on l'a trouvé, on renvoit null*/
		AC=NULL;

	return AC;
}

void Ajouter_Noeud_Dico (dico,code,wc,Place)
{

}

/*
  this function gives the higher node in a dictionnary
  param : a node
  return a caractere
*/
Caractere get_first_letter(un_noeud node)
{
    un_noeud*AC=node;
    while((AC -> pere) != NULL)
      {
	AC= AC -> pere;
      }
    return AC -> car;
}

Code Recherche_code (int *bit_restant, int *nb_bit_restant, int nb_bit, char *chaine){

	Code code, temp;
	char c;
	int rest_bit;
	
	/*Recherche de code*/
	code = *bit_restant;
	rest_bit = nb_bit-(*nb_bit_restant);
	code = code_first << (rest_bit);
	
	/*permet de remplir le temp si on a des grosses longueur de bit>8*/
	temp=0;
	i=0;
	while(rest_bit>8){
		temp = temp << 8;
		temp = temp | chaine[i];
		i++;
		rest_bit-8;
	}
	
	/*Finition sur le code*/
	if(rest_bit == 0){
		code = code_first | temp;
		*nb_bit_restant = 0;
		*bit_restant = 0;
	}
	else {
		c=chaine[nb_case-1];
		*nb_bit_restant = nb_bit-rest_bit;
		c = c >> (*nb_bit_restant);
		temp = temp | c;
		code = code_first | temp;
	}
	
	return code;


}

int parcours_tab_code (*noeud *tab_code){
	int i=0; 
	while(tab_code[i]!=NULL){
		i++;
	}
	return i;
}

void Ajout_Dico_Decomp (Code code_second,*noeud *tab_code,un_noeud *new_noeud,un_noeud *noeud_courant){
	
	/* Cas si le suivant est un mot*/
	if(code_second>258){
		c = get_first_letter(tab_code[code_second-259]);
		*new_noeud -> car = c;
		*new_noeud -> code = 259 + parcours_tab_code (tab_code);
		*new_noeud -> pere = *noeud_courant;
		*new_noeud -> frere = *noeud_courant -> fils;
		*new_noeud -> fils = NULL;
		
		*noeud_courant -> fils = *new_noeud;
	}
	/*Cas si le suivant est caractere special*/
	else if (code_second>=256 || code_second<=258){
	
	}
	/*Cas si le suivant est caractere basique*/
	else {
		*new_noeud -> car = *code_second;
		*new_noeud -> code = 259 + parcours_tab_code (tab_code);
		*new_noeud -> pere = *noeud_courant;
		*new_noeud -> frere = *noeud_courant -> fils;
		*new_noeud -> fils = NULL;
			
		*noeud_courant -> fils = *new_noeud;
	}


}

Code get_code(Dictionnaire d, FILE *f, int *bit_restant, int *nb_bit_restant, int nb_bit, *noeud *tab_code){

	int nb_case = nb_bit%8+1;
	char first_c[nb_case];
	char second_c[nb_case];
	Code code_first, code_second, temp;
	int bit_restant_second,rest_bit,i;
	char c;
	un_noeud new_noeud, noeud_courant;
	
	for(i=0;i<nb_case;i++){
		first_c[i] = fgetc(f);
	}
	for(i=0;i<nb_case;i++){
		second_c[i] = fgetc(f);
	}
	
	code_first = Recherche_code (*bit_restant,*nb_bit_restant,nb_bit,first_c);
	bit_restant_second = *bit_restant;
	rest_bit = *nb_bit_restant;
	code_second = Recherche_code (&bit_restant_second,&rest_bit,nb_bit,second_c);
	
	
	/*Cas si le code_first est un mot*/
	if(code_first>258){
		new_noeud = malloc(sizeof(un_noeud));
		noeud_courant = tab_code[code_first-259]; 
		
		Ajout_Dico_Decomp (code_second,tab_code,&new_noeud,&noeud_courant);
		
	}
	/*Cas si le code_first est caractere special*/
	else if (code_first>=256 || code_first<=258){
		
	}
	/*Cas si le code_first est caractere basique*/
	else {
		/*On cree le nouveau noeud et on cherche l'endroit ou l'ajouter par rapport au code_first*/
		new_noeud = malloc(sizeof(un_noeud));
		noeud_courant = d.racine; 
		while(noeud_courant -> frere != NULL && noeud_courant->code != code_first){
			noeud_courant = noeud_courant -> frere;
		}
		
		Ajout_Dico_Decomp (code_second,tab_code,&new_noeud,&noeud_courant);
	}
	
	fseek(f,-nb_case, SEEK_CUR);
	return code_first;
}
