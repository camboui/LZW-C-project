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
un_noeud* Est_Dans_Dico (unsigned char wc, un_noeud* AC, Dictionnaire d)
{
	un_noeud* temp =NULL;
	temp = AC;
	int cpt=0;
	if(temp != d.racine){
		temp = temp -> fils;
	}
	
	if (temp != NULL) {
		while(temp->frere != NULL && temp->car != wc)
		{
			cpt++;
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


void Ajout_Dico_Decomp (Code code_second,Code code_first,un_noeud **tab_code,un_noeud *noeud_courant){
	
	char c;
	un_noeud *new_noeud;
	new_noeud = malloc(sizeof(un_noeud));
	
	
	
	/* Cas si le suivant est un mot*/
	if(code_second>=START && tab_code[code_second-START]!=NULL){
		c = get_first_letter(tab_code[code_second-START]);
		
		new_noeud -> car = c;
		new_noeud -> code = START + parcours_tab_code (tab_code);
		new_noeud -> pere = noeud_courant;
		new_noeud -> frere = noeud_courant -> fils;
		new_noeud -> fils = NULL;
		
		noeud_courant -> fils =new_noeud;
		tab_code[parcours_tab_code (tab_code)] = new_noeud;
	}
	else if (code_second>=START && tab_code[code_second-START]==NULL){
		if (code_first >=START){
			code_first = get_first_letter(tab_code[code_first-START]);
		}
		new_noeud -> car = code_first;
		new_noeud -> code = START + parcours_tab_code (tab_code);
		new_noeud -> pere = noeud_courant;
		new_noeud -> frere = noeud_courant -> fils;
		new_noeud -> fils = NULL;
		
		noeud_courant -> fils =new_noeud;
		tab_code[parcours_tab_code (tab_code)] = new_noeud;
	
	
	}
	/*Cas si le suivant est caractere special*/
	else if (code_second>=256 && code_second<START){

	}
	/*Cas si le suivant est caractere basique*/
	else {

		new_noeud -> car = code_second;
		new_noeud -> code = START + parcours_tab_code (tab_code);
		new_noeud -> pere = noeud_courant;
		new_noeud -> frere = noeud_courant -> fils;
		new_noeud -> fils = NULL;
			
		noeud_courant -> fils = new_noeud;
		tab_code[parcours_tab_code (tab_code)] = new_noeud;

	}
	

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


Code get_code(Dictionnaire d, FILE *f, int *bit_restant, int *nb_bit_restant, int nb_bit_code, un_noeud* *tab_code){

	int nb_case, nb_case_sec;
	
	
	if ((nb_bit_code - *nb_bit_restant) <= 8){
		nb_case = nb_bit_code/8;
		nb_case_sec = nb_case +1;
	}
	else {
		nb_case = (nb_bit_code - *nb_bit_restant)/8 +1;
		nb_case_sec = nb_case -1;
	}
	/*if(nb_bit_code%8 == 0 || *nb_bit_restant == 1){
		printf("\ntamere %i\n", *nb_bit_restant);
		nb_case = nb_bit_code/8;
	}
	else {
		
		nb_case = (nb_bit_code - *nb_bit_restant)/8 +1;
	}*/
	
	
	/*if (*nb_bit_restant == 1){
		nb_case_sec = nb_case+1;
	}
	else if (nb_case != 1){
		nb_case_sec = nb_case-1;
	}
	else{
		nb_case_sec = nb_case;
	}*/
	
	
	
	int first_c[nb_case];
	int second_c[nb_case_sec];
	Code code_first, code_second;
	int bit_restant_second,rest_bit,i;
	un_noeud *noeud_courant;
	
	for(i=0;i<nb_case;i++){
		first_c[i] = fgetc(f);
	}


	for(i=0;i<nb_case_sec;i++){
		second_c[i] = fgetc(f);
	}

	
	printf("\n\n-------%i-------",*nb_bit_restant);
	
	code_first = Recherche_code (bit_restant,nb_bit_restant,nb_bit_code,first_c);
	bit_restant_second = *bit_restant;
	rest_bit = *nb_bit_restant;
	code_second = Recherche_code (&bit_restant_second,&rest_bit,nb_bit_code,second_c);
	printf("\n______%i",*nb_bit_restant);
	printf("\n--%d--))%d((\n",nb_case,nb_case_sec);
	printf("~~~~~~~%i~~~~~~~%i\n",code_first,code_second);
	
	
	/*Cas si le code_first est un mot*/
	if(code_first>=START){
		noeud_courant = tab_code[code_first-START]; 
		Ajout_Dico_Decomp (code_second,code_first,tab_code,noeud_courant);
	}
	/*Cas si le code_first est caractere special*/
	else if (code_first>=256 && code_first<=258){
	
	}
	/*Cas si le code_first est caractere basique*/
	else {
		/*On cree le nouveau un_noeud et on cherche l'endroit ou l'ajouter par rapport au code_first*/
		noeud_courant = d.racine; 
		while(noeud_courant -> frere != NULL && noeud_courant->code != code_first){
			noeud_courant = noeud_courant -> frere;
		}
		Ajout_Dico_Decomp (code_second,code_first,tab_code,noeud_courant);

	}
	/*for (i = 0; i<parcours_tab_code (tab_code);i++){
		printf("\n%i -> ",START + i);
		Afficher_chaine_de(tab_code[i]);
	}*/
	fseek(f,-nb_case_sec, SEEK_CUR);

	
	return code_first;
}
