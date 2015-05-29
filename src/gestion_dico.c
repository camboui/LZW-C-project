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

	Code res =0;
	int i = 0, alire =0;
	
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
			*bit_restant = (*bit_restant << alire);
			*bit_restant = (*bit_restant >> alire);
			alire =0;
			printf("\n -------res : %u ----------",res);
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

void Affichage (char *ch, int nb_case){
	int i;
	for(i=0; i<nb_case; i++){
		printf("%i ",ch[i]);
	}
	
	
}

Code get_code(Dictionnaire d, FILE *f, int *bit_restant, int *nb_bit_restant, int nb_bit_code, un_noeud* *tab_code){

	int nb_case, nb_case_sec;

	if(nb_bit_code%8 == 0){
		nb_case = nb_bit_code/8;
	}
	else {
		nb_case = (nb_bit_code - *nb_bit_restant)/8 +1;
	}
	if (nb_case != 1){
		nb_case_sec = nb_case-1;
	}
	else {
		nb_case_sec = nb_case;
	}
	char first_c[nb_case];
	char second_c[nb_case_sec];
	Code code_first, code_second;
	int bit_restant_second,rest_bit,i;
	un_noeud* new_noeud, *noeud_courant;
	for(i=0;i<nb_case;i++){
		first_c[i] = fgetc(f);
	}

	for(i=0;i<nb_case_sec;i++){
		second_c[i] = fgetc(f);
	}

	

	code_first = Recherche_code (bit_restant,nb_bit_restant,nb_bit_code,first_c);
	bit_restant_second = *bit_restant;
	rest_bit = *nb_bit_restant;
	code_second = Recherche_code (&bit_restant_second,&rest_bit,nb_bit_code,second_c);
	printf("\nchaine first : ");
	Affichage(first_c,nb_case);
	printf("\nchaine second : ");
	Affichage(second_c,nb_case_sec);
	printf("\ncode_first : %i, code_sec : %i, bits rest : %i, nb bit : %i, 2br : %i, nb2br : %i\n",code_first,code_second,*bit_restant,*nb_bit_restant,bit_restant_second,rest_bit);
	
	
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
	
	if (nb_case == 1){
		fseek(f,-nb_case, SEEK_CUR);
	}
	else {
		fseek(f,-nb_case+1, SEEK_CUR);
	}
	
	return code_first;
}
