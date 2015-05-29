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

/* si renvoit de temp il a trouve le caractere il faut donc regarder le caractere suivant sinon on doit rajouter ce caractere dans le dico*/
un_noeud* Est_Dans_Dico (char wc, un_noeud* AC)
{
	un_noeud* temp =NULL;
	temp = AC;
	if(temp->fils !=NULL)
		temp = temp -> fils;
	
	
	
	while(temp->frere != NULL && temp->car != wc)
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

Code Recherche_code (int *bit_restant, int *nb_bit_restant, int nb_bit_code, char *chaine){

	Code code, temp;
	char c, masque = 255;
	int rest_bit,i;

	/*Recherche de code*/
	code = *bit_restant;
	rest_bit = nb_bit_code-(*nb_bit_restant);
	code = code << (rest_bit);


	
	/*permet de remplir le temp si on a des grosses longueur de bit>8*/
	temp=0;
	i=0;
	while(rest_bit>=8){
		temp = temp << 8;
		temp = temp | chaine[i];
		i++;
		rest_bit-=8;
	}
	temp = temp << rest_bit;
	
	/*Finition sur le code*/
	if(rest_bit == 0){
		code = code | temp;
		*nb_bit_restant = 0;
		*bit_restant = 0;
	}
	else {
		c=chaine[strlen(chaine)-1];
		*nb_bit_restant = 8-rest_bit;
		masque = masque >> rest_bit;
		*bit_restant = masque & c;
		c = c >> (*nb_bit_restant);
		temp = temp | c;
		code = code | temp;
	}
	
	return code;


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


void Ajout_Dico_Decomp (Code code_second,un_noeud **tab_code,un_noeud *new_noeud,un_noeud *noeud_courant){
	
	char c;
	/* Cas si le suivant est un mot*/
	if(code_second>=START){
		c = get_first_letter(tab_code[code_second-START]);
		new_noeud -> car = c;
		new_noeud -> code = START + parcours_tab_code (tab_code);
		new_noeud -> pere = noeud_courant;
		new_noeud -> frere = noeud_courant -> fils;
		new_noeud -> fils = NULL;
		
		noeud_courant -> fils =new_noeud;
	}
	/*Cas si le suivant est caractere special*/
	else if (code_second>=256 || code_second<START){
	
	}
	/*Cas si le suivant est caractere basique*/
	else {
		new_noeud -> car = code_second;
		new_noeud -> code = START + parcours_tab_code (tab_code);
		new_noeud -> pere = noeud_courant;
		new_noeud -> frere = noeud_courant -> fils;
		new_noeud -> fils = NULL;
			
		noeud_courant -> fils = new_noeud;
	}


}


Code get_code(Dictionnaire d, FILE *f, int *bit_restant, int *nb_bit_restant, int nb_bit_code, un_noeud* *tab_code){

	int nb_case;

	if(nb_bit_code%8 == 0){
		nb_case = nb_bit_code/8;
	}
	else {
		nb_case = nb_bit_code/8 +1;
	}
	
	char first_c[nb_case];
	char second_c[nb_case];
	Code code_first, code_second;
	int bit_restant_second,rest_bit,i;
	un_noeud* new_noeud, *noeud_courant;
	
	for(i=0;i<nb_case;i++){
		first_c[i] = fgetc(f);
	}

	for(i=0;i<nb_case-1;i++){
		second_c[i] = fgetc(f);
	}
	
	code_first = Recherche_code (bit_restant,nb_bit_restant,nb_bit_code,first_c);
	bit_restant_second = *bit_restant;
	rest_bit = *nb_bit_restant;
	
	code_second = Recherche_code (&bit_restant_second,&rest_bit,nb_bit_code,second_c);
	printf("\nchaine first : %s chaine second : %s\n",first_c,second_c);
	printf("\ncode : %i bits rest : %i nb bit : %i\n",code_second,*bit_restant,*nb_bit_restant);
	
	
	/*Cas si le code_first est un mot*/
	if(code_first>258){
		new_noeud = malloc(sizeof(un_noeud));
		noeud_courant = tab_code[code_first-259]; 
		
		Ajout_Dico_Decomp (code_second,tab_code,new_noeud,noeud_courant);
		
	}
	/*Cas si le code_first est caractere special*/
	else if (code_first>=256 || code_first<=258){
		
	}
	/*Cas si le code_first est caractere basique*/
	else {
		/*On cree le nouveau un_noeud et on cherche l'endroit ou l'ajouter par rapport au code_first*/
		new_noeud = malloc(sizeof(un_noeud));
		noeud_courant = d.racine; 
		while(noeud_courant -> frere != NULL && noeud_courant->code != code_first){
			noeud_courant = noeud_courant -> frere;
		}
		
		Ajout_Dico_Decomp (code_second,tab_code,new_noeud,noeud_courant);
	}
	
	fseek(f,-nb_case+1, SEEK_CUR);
	return code_first;
}
