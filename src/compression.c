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

void Ecrire_Code (FILE *f, Code code, char *bit_restant,int *nb_bit_restant, int nb_bit_code)
{
	unsigned char res=0;
	char c;
	
	int  alire =0,place_res=8;
	
	//printf("\n--------%i---------%i--------",*bit_restant,*nb_bit_restant);
	place_res-= *nb_bit_restant; 
	res = *bit_restant << place_res; /* On récupère les bits du buffer et on fait pour les bits restants (pris dans code) */
	
	alire = nb_bit_code - place_res;
	res = res | (code >>alire);/* Notre premier mot fait 8 bits, on peut ensuite prendre des blocs de 8 bits */
	fprintf(f,"%c",res);
	//printf("\n%i",res);

	while (alire>=8)
	{
			res = 0;
			res = code >> (alire - 8);
			fprintf(f,"%c",res);
			//printf("\n%d",res);
			alire-=8;
	}
	*nb_bit_restant=alire;
	c= code;
	c =c << (nb_bit_code-*nb_bit_restant);/*supprimer toutes les bits inutiles à gauche du code*/
	*bit_restant= c >> (nb_bit_code-*nb_bit_restant);/*On remet la valeur à sa place*/

}


void Affichage_dico (un_noeud* d){

	un_noeud* AC,*AF, *AFF; 
	AC = d;
	
	while (AC -> frere != NULL ){
		printf("--%c--", AC -> code);
		AF = AC -> fils;
	
		while(AF != NULL){
			printf("-%c-", AF -> car);
			AFF = AF -> fils;
			while(AFF != NULL){
				printf(")%c(", AFF -> car);
				AFF = AFF -> frere;
			}
			AF = AF -> frere;
		}
		printf("\n");
		AC = AC -> frere;
	
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

	
	unsigned char c;
	Code code = START;
	int nb_bit_code = 9;
	char bit_restants = 0;
	int nb_bit_restant = 0;

	un_noeud* Place_courant, *New_place;
	
	c=(unsigned char)fgetc(f_entree);
	while (!feof(f_entree))
	{
	
			Place_courant = dico.racine;
			
			New_place = Est_Dans_Dico (c,Place_courant,dico);
			//printf("\navt)))))))))))%c((((((((((",c);
			while ( New_place -> car != Place_courant -> car && New_place -> code != Place_courant -> code ){
				c=(unsigned char)fgetc(f_entree);
				//printf("\napr1)))))))))))%i(((((((((())))))))%i(((((((((",New_place -> car,c);
				Place_courant = New_place;
				//printf("\napr2)))))))))))%i(((((((((())))))))%i(((((((((",New_place -> car,Place_courant -> car);
				New_place = Est_Dans_Dico (c,Place_courant,dico);
				//printf("\napr2)))))))))))%i(((((((((())))))))%i(((((((((",New_place -> car,Place_courant -> car);
			}
			
			Ajouter_Noeud_Dico (code,c,Place_courant);/*Ajoute le Noeud qu'il soit Fils ou Frere*/
			code ++;
			if (code >= pow(2,nb_bit_code)){
			
				nb_bit_code++;
				Ecrire_Code (f_sortie,257,&bit_restants,&nb_bit_restant,nb_bit_code);
			}
			Ecrire_Code (f_sortie,Place_courant -> code,&bit_restants,&nb_bit_restant,nb_bit_code);
			
			
	}
	Ecrire_Code (f_sortie,Place_courant -> code,&bit_restants,&nb_bit_restant,nb_bit_code);
	Ecrire_Code (f_sortie,256,&bit_restants,&nb_bit_restant,nb_bit_code);
	printf("-----------------------------------------");
	Affichage_dico (dico.racine);
	fprintf(f_entree,"%c",(char)0);
	fclose(f_entree);
	fclose(f_sortie);
}



