/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/



void Decompression (char *nom_fichier){

	/* Ouverture et creation des fichiers*/
	
	FILE *f_entree, *f_sortie;
	f_entree = fopen(*nom_fichier,"r");
	int lg = strlen(*nom_fichier);
	char *nom_fichier_sortie = *nom_fichier;
	nom_fichier_sortie[lg-5]= '\0';
	f_sortie = fopen(nom_fichier_sortie,"w");
	
	/*Creation du dico*/
	Dictionnaire dico;
	dico = Init();
	
	char c[10];
	char chaine[100];
	int nb_bit = 9;
	int bit_restant;
	Code code = 0;
	un_noeud lettre;
	char decode; 
	int i,j;
	int fin_decomp = 0;
	while (fin_decomp != 1){
		/*get_code permet de recuperer le code du prochain caractere a decoder et utilise les bit en trop du code precedent*/
		code = get_code(*f_entree,*bit_restant,nb_bit);
		
		lettre = CodeVersChaine (code);
		
		switch(code){
			/*fin de fichier*/
			case 256 :
				fin_decomp=1; 
				break;
			/*Separateur bit*/
			case 257 : 
				nb_bit++;
				break;
			/*Reinit dico*/
			case 258 : break;
			default : 
				i=0;
				chaine[0]='\0';
				while (lettre.parent != NULL){
			
					lg = strlen(chaine);
					for(i=lg;i>0;i--){
						chaine[i+1]='\0';
						chaine[i]=chaine[i-1];
					}
					chaine[0]=lettre.car;
					lettre = lettre.parent;
			
				}
				fprintf(f_sortie,"%s",chaine);
				break;
		}
	
	
	}
	fclose(f_sortie);
	fclose(f_entree);





}
