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




void Init_tab (un_noeud **tab, int nb_case){
	
	int i=0; 
	while(tab[i] != NULL){
		i++;
	}
	while(i<nb_case){
		tab[i] = NULL;
		i++;
	}
}

void Afficher_chaine_d (un_noeud *lettre){

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

void Decompression (char *nom_fichier){
	/* Ouverture et creation des fichiers*/
	
	FILE *f_entree, *f_sortie;
	f_entree = fopen(nom_fichier,"r");
	int lg = strlen(nom_fichier);
	char nom_fichier_sortie[lg];
	strcpy(nom_fichier_sortie,nom_fichier);
	nom_fichier_sortie[lg-5]= '\0';
	f_sortie = fopen(nom_fichier_sortie,"w+");
	
	
	/*Creation du dico*/
	Dictionnaire dico;
	dico = Init();
	
	/*Creation d'un tableau d'adresse de noeud*/
	un_noeud** tab_code;
	tab_code = malloc(MAX_DICO*sizeof(un_noeud));
	lg=0;
	
	while (lg<MAX_DICO){
		tab_code[lg] = NULL;
		lg++;
	}
	
	

	char *chaine;
	Code code_actuel=0, code_suivant = 0;
	un_noeud* lettre;
	int bit_restant=0,fin_decomp,nb_bit_code,nb_bit_restant=0,i;
	nb_bit_code = 9;
	fin_decomp = 0;
	int cp1,cp2;
	
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
				dico = Init();
				printf("\n Je te réinitialise maggle");
				for(lg=0;lg<MAX_DICO;lg++)
					tab_code[lg]=NULL;
				nb_bit_code=9;
			break;
			default : 
			cp1=bit_restant;
			cp2=nb_bit_restant;

				code_suivant = get_code(f_entree,&cp1,&cp2,nb_bit_code,1);
				
				if(code_suivant==257)
					code_suivant = get_code(f_entree,&cp1,&cp2,nb_bit_code+1,1);
				
				if(code_suivant!=256 && code_suivant!=258)
					ajout_dico (code_actuel,code_suivant,tab_code,dico);
					
				if (code_actuel>=START ){

					lettre = tab_code[code_actuel-START];
					
					lg = nb_pere(lettre);
					
					chaine = malloc(lg*sizeof(char)+1);
					chaine[lg+1]='\0';
					i=lg;
					while (lettre != NULL && i >= 0){
						chaine[i]=lettre->car;
						i--;
						lettre = lettre -> pere;
					}
					for (i=0; i<=lg;i++)
						fprintf(f_sortie,"%c",chaine[i]);
					//free(chaine);
				}
				else {
					fprintf(f_sortie,"%c",code_actuel);
				}
				
				break;
		}
		
	
	}
printf("\n %i ",parcours_tab_code(tab_code));

	fclose(f_sortie);
	fclose(f_entree);





}
