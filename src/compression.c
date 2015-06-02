/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gestion_dico.h"



/*
	Ecrire code : 
	écrit sous forme d'octets les bits correspondants au code dans un fichier.
	Les bits étants écrits 8 par 8, il peux y avoir un reste qui est stocké dans les paramètres :
		bit_restant 
		nb_bit_restant
		nb_bit_code

*/
void Ecrire_Code (FILE *f, Code code, Caractere *bit_restant,int *nb_bit_restant, int nb_bit_code)
{
	Caractere res=0;
	Caractere c;
	
	int  alire =0,place_res=8;
	
	place_res-= *nb_bit_restant;
	res = *bit_restant << place_res; /* On récupère les bits du buffer et on fait pour les bits restants (pris dans code) */
	alire = nb_bit_code - place_res;
	res = res | (code >>alire);/* Notre premier mot fait 8 bits, on peut ensuite prendre des blocs de 8 bits */
	fprintf(f,"%c",res);

	while (alire>=8)/*Tant qu'on peut écrire des octets on le fait*/
	{
			res = 0;
			res = code >> (alire - 8);
			fprintf(f,"%c",res);
			alire-=8;
	}
	*nb_bit_restant=alire;
	if(alire >0 ){ /*S'il reste des bits à lire, on actualise le nouveau buffer*/
		*bit_restant =0;
		c= (Caractere)code;
		c =c << (8-*nb_bit_restant);/*supprimer toutes les bits inutiles à gauche du code*/
		c= c >> (8-*nb_bit_restant);/*On remet la valeur à sa place*/
		*bit_restant = c;
	}
	else {
		*bit_restant = 0;
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

	
	Caractere c;
	Code code = START;
	int nb_bit_code = 9;
	Caractere bit_restants = 0;
	int nb_bit_restant = 0;
	un_noeud* Place_courant, *New_place;
	
	c=(Caractere)fgetc(f_entree);

	while (!feof(f_entree))
	{
			Place_courant = dico.racine;

			New_place = Est_Dans_Dico (c,Place_courant);

			while ( (New_place != Place_courant || New_place== dico.racine )&& !feof(f_entree)){
				c=(Caractere)fgetc(f_entree);
				Place_courant = New_place;
				New_place = Est_Dans_Dico (c,Place_courant);
			}

			Ajouter_Noeud_Dico (code,c,Place_courant);/*Ajoute le Noeud qu'il soit Fils ou Frere*/

			Ecrire_Code (f_sortie,Place_courant -> code,&bit_restants,&nb_bit_restant,nb_bit_code);
			code ++;
	
			if (code >= pow(2,nb_bit_code)){
				if(code >=MAX_DICO)
				{
					Ecrire_Code (f_sortie,258,&bit_restants,&nb_bit_restant,nb_bit_code);
					nb_bit_code=9;	
					liberer_noeud(dico.racine);
					dico = Init ();
					code=START;	
				}
				else
				{
					Ecrire_Code (f_sortie,257,&bit_restants,&nb_bit_restant,nb_bit_code);
					nb_bit_code++;
				}
			}
			
	}
	Ecrire_Code (f_sortie,256,&bit_restants,&nb_bit_restant,nb_bit_code);
	if (nb_bit_restant != 0){
		Ecrire_Code (f_sortie,(Caractere)0,&bit_restants,&nb_bit_restant,8);
	}
	
	liberer_noeud(dico.racine);
	fclose(f_entree);
	fclose(f_sortie);
}


/*
void Afficher_chaine (un_noeud *lettre){

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
}*/



/*
void Affichage_dico (un_noeud* d, int etage){

	un_noeud* AC,*AF; 
	AC = d;
	
	while (AC  != NULL ){
	printf("\nFRERE");
		printf("--%i-- ETAGE : %i", AC -> code,etage);
		AF = AC -> fils;
		
	
		while(AF != NULL){
		etage++;
			printf("\nFILS");
			printf(")%i(", AF -> code);
			printf("\n");
			Affichage_dico (AF,etage);
			AF = AF -> frere;

		}	
		AC = AC -> frere;

	}
}*/



