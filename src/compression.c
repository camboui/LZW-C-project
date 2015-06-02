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


/*
	Compression : permet de compresser un fichier via l'algorithme de Lempel-Ziv-Welch
*/
void Compression (char *nom_entree)
{
	FILE *f_entree,*f_sortie;
	Dictionnaire dico;
	char nom_sortie[strlen(nom_entree)+4];
	Code code = START;
	int nb_bit_code = 9,nb_bit_restant = 0;
	Caractere bit_restants = 0 , c ;
	un_noeud* Place_courant, *New_place;
	
	dico = Init ();
	if (dico.racine == NULL)
	{
		fprintf (stderr,"Echec de l'initialisation du dictionnaire\n");
		exit (EXIT_FAILURE);
	}

	/*Initialisation des fichiers et vérification du retour des fonctions */
	f_entree = fopen (nom_entree,"r");
	if (f_entree == NULL)
	{
		fprintf (stderr,"Echec de l'ouverture de f_entree en lecture\n");
		exit (EXIT_FAILURE);
	}
	
	strcpy(nom_sortie,nom_entree);/*On change le nom de fichier pour ajouter une extension*/
	strcat(nom_sortie,".lzw");
	f_sortie = fopen (nom_sortie,"w+");
	if (f_sortie == NULL)
	{
		fprintf (stderr,"Echec de l'ouverture de f_sortie en écriture\n");
		exit (EXIT_FAILURE);
	}

	/*boucle principale de l'algorithme*/
	c=(Caractere)fgetc(f_entree); /*On lit d'abord un premier caractère*/

	while (!feof(f_entree))
	{
			Place_courant = dico.racine;

			New_place = Est_Dans_Dico (c,Place_courant);/*On se position sur le noeud correspondant au caractère*/

			/*Tant qu'on a pas un nouveau mot à écrire dans le dictionnaire, on lit un nouveau caractère*/
			while ( (New_place != Place_courant || New_place== dico.racine )&& !feof(f_entree)){
				c=(Caractere)fgetc(f_entree);
				Place_courant = New_place;
				New_place = Est_Dans_Dico (c,Place_courant);
			}
			
			Ajouter_Noeud_Dico (code,c,Place_courant);/*Ajoute le Noeud (encore INEXISTANT) au dictionnaire*/

	
			Ecrire_Code (f_sortie,Place_courant -> code,&bit_restants,&nb_bit_restant,nb_bit_code);	/*Ecrit dans le fichier de sortie le dernier code qu'on connait*/
			code ++;
	
			if (code >= pow(2,nb_bit_code)){/*Si on passe une puissance de 2 ...*/
				if(code >=MAX_DICO)/*... Et si on dépasse une certaine taille, on imprime le caractère de réinitialisation du dico*/
				{
					Ecrire_Code (f_sortie,258,&bit_restants,&nb_bit_restant,nb_bit_code);
					nb_bit_code=9;	
					liberer_noeud(dico.racine);
					dico = Init ();
					code=START;	
				}
				else/*... Sinon, on imprime le caractère d'augmentation du nombre de bit qu'il faut lire*/
				{
					Ecrire_Code (f_sortie,257,&bit_restants,&nb_bit_restant,nb_bit_code);
					nb_bit_code++;
				}
			}
			
	}
	
	Ecrire_Code (f_sortie,256,&bit_restants,&nb_bit_restant,nb_bit_code);/*On écrit la fin de fichier*/
	if (nb_bit_restant != 0){/*S'il reste des bits qui correspondent au fin de fichier, on bourre avec des 0*/
		Ecrire_Code (f_sortie,(Caractere)0,&bit_restants,&nb_bit_restant,8);
	}
	
	/*On libère toute notre mémoire */
	liberer_noeud(dico.racine);
	fclose(f_entree);
	fclose(f_sortie);
}




