/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int rle(char * nom_entree)
{
	//premier permet que le premier caratère lu soit correct
	unsigned char compteur=1;
	int a,b;

	FILE *f_lecture = NULL, *f_ecriture = NULL;

	//Si l'ouverture échoue, une erreur est envoyée
	if ((f_lecture= fopen(nom_entree,"r")) == NULL)
	{
		fprintf(stderr, "Echec de l'ouverture de %s en lecture\n",nom_entree);
		exit(EXIT_FAILURE);
	}

	char nom_sortie[100];
	strcpy(nom_sortie,nom_entree);
	strcat(nom_sortie,".rle");

	if ((f_ecriture = fopen(nom_sortie,"w+")) == NULL)
	{
		fprintf(stderr, "Echec de l'ouverture de %s en ecriture \n",nom_sortie);
		exit(EXIT_FAILURE);
	}

	a=fgetc(f_lecture);
	b=a;
	a=fgetc(f_lecture);

	do
	{	
		if (a!=b && compteur < 2)
		{
			fputc(b,f_ecriture);
			compteur=1;
		}
		if (a!=b && compteur >= 2 && compteur <255)
		{
			fputc(b,f_ecriture);
			fputc(b,f_ecriture);
			fputc(compteur-2,f_ecriture);
			compteur=1;
		}
		else if(a==b)
		{
			if(compteur==255)
			{
				fputc(b,f_ecriture);
				fputc(b,f_ecriture);
				fputc(compteur-2,f_ecriture);
				compteur=0;
			}
			else
				compteur++;
		}
		b=a;

	} 
	while ((a = fgetc(f_lecture)) != EOF);

	if (compteur < 2)
	{
		fputc(b,f_ecriture);
	}
	if ( compteur >= 2)
	{
		fputc(b,f_ecriture);
		fputc(b,f_ecriture);
		fputc(compteur-2,f_ecriture);
	}

	fclose(f_ecriture);
	fclose(f_lecture);
	printf("\n nouveau fichier %s ",nom_sortie);
	return 0;	
}



int decoder_rle(char * nom_entree)
{
	int a,b;
	unsigned char comp,compteur;

	FILE *f_lecture = NULL, *f_ecriture = NULL;
	char nom_sortie[100];
	strcpy(nom_sortie,nom_entree);
	nom_sortie[strlen(nom_entree)-4]='\0';

	//Si l'ouverture échoue, une erreur est envoyée
	if ((f_lecture= fopen(nom_entree,"r")) == NULL)
	{
		fprintf(stderr, "Echec de l'ouverture de %s en lecture\n", nom_entree);
		exit(EXIT_FAILURE);
	}

	if ((f_ecriture = fopen(nom_sortie,"w+")) == NULL)
	{
		fprintf(stderr, "Echec de l'ouverture de %s en ecriture \n",nom_sortie);
		exit(EXIT_FAILURE);
	}
	a=fgetc(f_lecture);
	b=a;
	a= fgetc(f_lecture);
	
	do
	{
		if(a!=b)
		{
			fputc(b,f_ecriture);
		}		
		else
		{
			fputc(b,f_ecriture);
			fputc(b,f_ecriture);
			comp=fgetc(f_lecture);
			for(compteur=0;compteur<comp;compteur++)
			{
				fputc(b,f_ecriture);
			}
			a= fgetc(f_lecture);
		}
		b=a;
		//   printf("valeur de b %c\n",b);
		//printf("valeur de a %c\n",a);
	}while ((a=fgetc(f_lecture)) != EOF);
	
	if(b!=EOF)
	{
		fputc(b,f_ecriture);
	}
	fclose(f_lecture);
	fclose(f_ecriture);
	printf("\n nouveau fichier %s ",nom_sortie);
	return 0;
}
