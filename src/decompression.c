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
	un_noeud* tab_code[512];
	lg=0;
	
	while (lg<512){
		tab_code[lg] = NULL;
		lg++;
	}
	
	
	void *tab_code_re;
	char *chaine;
	Code code = 0;
	un_noeud* lettre;
	int bit_restant=0,fin_decomp,nb_bit_code,nb_bit_restant=0;
	nb_bit_code = 9;
	fin_decomp = 0;
	
	
	while (fin_decomp != 1){
		/*get_code permet de recuperer le code du prochain caractere a decoder et utilise les bit en trop du code precedent*/
		code = get_code(dico,f_entree,&bit_restant,&nb_bit_restant,nb_bit_code,tab_code);
		//printf("\ncode(main) : %i bits rest : %i nb bit : %i\n",code,bit_restant,nb_bit_restant);
		
		/*switch pour differencier les 3 codes rajoutés à la main*/
		switch(code){
			/*fin de fichier*/
			case 256 :
				fin_decomp=1; 
				break;
			/*Separateur bit*/
			case 257 : 
				nb_bit_code++;
				tab_code_re = realloc(*tab_code,pow(2,nb_bit_code)*sizeof(un_noeud));
				if (tab_code_re != NULL){
					*tab_code = tab_code_re;
					Init_tab (tab_code,pow(2,nb_bit_code));
				}
				else {
					printf("Problème mémoire, cause -> realloc");
					exit(EXIT_FAILURE);
				}
				break;
			/*Reinit dico*/
			case 258 : break;
			default : 

				if (code>258){
					
					lettre = tab_code[code-START];
					
					lg = nb_pere(lettre);
					
					chaine = malloc(lg*sizeof(char));
					
					while (lettre != NULL && lg >= 0){
						chaine[lg]=lettre->car;
						lg--;
						lettre = lettre -> pere;
					}
					fprintf(f_sortie,"%s",chaine);
					//free(chaine);
				}
				else {
					fprintf(f_sortie,"%c",code);
				}
				
				break;
		}
		
	
	}
	for (lg = 0; lg<parcours_tab_code (tab_code);lg++){
		printf("\n%i -> ",START + lg);
		Afficher_chaine_d(tab_code[lg]);
	}
	exit(0);
	fclose(f_sortie);
	fclose(f_entree);





}
