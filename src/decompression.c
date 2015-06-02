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



/* Initialisation du tableau d'adresse */
void Init_tab (un_noeud **tab){
	
	int i=0; 
	for (i=0; i<MAX_DICO;i++)
		tab[i] = NULL;
}

/* Permet de decoder les caractere lu precedemment */
Code Recherche_code (Caractere *bit_restant, unsigned int  *nb_bit_restant, int nb_bit_code, Caractere *chaine){


	Code res = 0;
	int i = 0, alire =0;
	Caractere c;

	alire = nb_bit_code - *nb_bit_restant;
	
	res = *bit_restant << alire;

	while (alire>0)
		{
		*bit_restant = chaine[i];
		i++;
		if(alire>=8){
			alire-=8;
			res = res | (*bit_restant << alire);
			*bit_restant = 0;
			*nb_bit_restant = 0;

		}
		else 
		{
			*nb_bit_restant = 8 - alire;
			res = res | (*bit_restant >> (8-alire));
			c=0;
			c=*bit_restant;
			c = (c << alire);
			*bit_restant = (c >> alire);
			alire =0;
		}
	}
	return res;
}

/*Fonction qui permet de recuperer les prochains caractere a decoder et retourne son code*/
Code get_code (FILE *f, Caractere *bit_restant, unsigned int  *nb_bit_restant, int nb_bit_code,int executer_retour)
{
	int nb_case = 1,alire = (nb_bit_code - *nb_bit_restant),i;
	Code code;
	while (alire>8){
		alire-=8;
		nb_case++;
	}
	
	Caractere car_lus[nb_case];

	for(i=0;i<nb_case;i++){
		car_lus[i] = (Caractere)fgetc(f);
		
	}
	code = Recherche_code (bit_restant,nb_bit_restant,nb_bit_code,car_lus);
	if(executer_retour==1 && code == 257) 
	{
		Caractere tmp1=*bit_restant;
		unsigned int tmp2=*nb_bit_restant;
	
		code = get_code (f,&tmp1,&tmp2,nb_bit_code+1,1);
		fseek(f,-nb_case, SEEK_CUR);
	}
	else if(executer_retour==1)
		fseek(f,-nb_case, SEEK_CUR);
	
	return code;
}




/*Fonction principal de decompression */
void Decompression (char *nom_fichier){
	
	Dictionnaire dico;
	char nom_fichier_sortie[lg];
	/*Creation d'un tableau d'adresse de noeud*/
	
	Caractere *chaine;
	Code code_actuel=0, code_suivant = 0;
	un_noeud* lettre=NULL;
	int fin_decomp,nb_bit_code,i,lg = strlen(nom_fichier);;
	unsigned int nb_bit_restant=0,cp2;
	Caractere bit_restant=0,cp1;
	
	un_noeud*tab_code[MAX_DICO];
	Init_tab (tab_code);
	
	
	strcpy(nom_fichier_sortie,nom_fichier);
	nom_fichier_sortie[lg-4]= '\0';
	strcat(nom_fichier_sortie,"_res");
	/* Ouverture et creation des fichiers*/
	FILE *f_entree, *f_sortie;
	
	f_entree = fopen(nom_fichier,"r");
	if (f_entree == NULL)
	{
		fprintf (stderr,"Echec de l'ouverture de f_entree en lecture\n");
		exit (EXIT_FAILURE);
	}
	
	f_sortie = fopen(nom_fichier_sortie,"w+");
	if (f_sortie == NULL)
	{
		fprintf (stderr,"Echec de l'ouverture de f_sortie en écriture\n");
		exit (EXIT_FAILURE);
	}
	
	/*Creation du dico*/
	dico = Init();
	if (dico.racine == NULL)
	{
		fprintf (stderr,"Echec de l'initialisation du dictionnaire\n");
		exit (EXIT_FAILURE);
	}

	nb_bit_code = 9;
	fin_decomp = 0;
	
	while (fin_decomp != 1){
		/*get_code permet de recuperer le code du prochain caractere a decoder et utilise les bit en trop du code precedent*/
		code_actuel  = get_code(f_entree,&bit_restant,&nb_bit_restant,nb_bit_code,0);

		/*switch pour differencier les 3 codes rajoutés à la main*/
		switch(code_actuel){
			/*fin de fichier*/
			case 256 :
				fin_decomp=1; 
				break;
			/*Separateur bit*/
			case 257 : 
				nb_bit_code++;
				break;
			/*Reinit dico*/
			case 258 : 
				liberer_noeud(dico.racine);
				dico = Init();
				Init_tab (tab_code);
				nb_bit_code=9;
			break;
			default : 
				cp1=bit_restant;
				cp2=nb_bit_restant;
				code_suivant = get_code(f_entree,&cp1,&cp2,nb_bit_code,1);
	
				if(code_suivant!=256 && code_suivant!=258 &&  code_suivant!=257)/*On n'ajoute rien au dico si on a un caractère spécial*/
					ajout_dico (code_actuel,code_suivant,tab_code,dico);
					
				if (code_actuel>=START){/*Si le code n'est un code basique*/

					lettre = tab_code[code_actuel-START];
					lg = nb_pere(lettre)+1;
					chaine = malloc((lg+1)*sizeof(Caractere));
					i=lg-1;
					
					while (lettre != NULL && i >= 0 && !feof(f_entree)){/*On récupère les caractère dans l'ordre inverse à la racine*/
						chaine[i]=lettre->car;
						i--;
						lettre = lettre -> pere;
					}
					
					for (i=0; i<lg;i++)/*On les écrit dans l'ordre partant de la racine*/
					{
						fputc(chaine[i],f_sortie);
					}
					
					free(chaine);
				}
				else {
					/*Si le caractère est basique, on peut l'afficher directement*/
					fputc((Caractere)code_actuel,f_sortie);
		
				}
				
				break;
		}
		
	
	}
	
	/*On libère toute notre mémoire*/
	liberer_noeud(dico.racine);
	fclose(f_sortie);
	fclose(f_entree);

}
