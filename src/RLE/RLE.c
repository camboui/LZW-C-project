#include <stdio.h>
#include <stdlib.h>
#define ENTREE "entree"
#define SORTIE "sortie"
#define SORTIE2 "sortiedecoder"
#define SORTIE3 "freq.txt"

int coder_rle(void)
{
  //premier permet que le premier caratère lu soit correct
  unsigned char compteur=0,premier=1;
  unsigned char a,b;

  FILE *f_lecture = NULL, *f_ecriture = NULL;
  
  //Si l'ouverture échoue, une erreur est envoyée
  if ((f_lecture= fopen(ENTREE,"r")) == NULL)
    {
      fprintf(stderr, "Echec de l'ouverture de %s en lecture\n",ENTREE);
      exit(EXIT_FAILURE);
    }
  if ((f_ecriture = fopen(SORTIE,"w+")) == NULL)
    {
      fprintf(stderr, "Echec de l'ouverture de %s en ecriture \n",SORTIE);
      exit(EXIT_FAILURE);
    }

  a=fgetc(f_lecture);
  b=a;
  a=fgetc(f_lecture);

  while (!feof(f_lecture))
    {	
      if (a!=b && compteur < 1)
	{
	  fputc(b,f_ecriture);
	  compteur=0;
	}
      if (a!=b && compteur >= 1)
	{
	  fputc(b,f_ecriture);
	  fputc(b,f_ecriture);
	  fputc(compteur-1,f_ecriture);
	  compteur=0;
	}
      else if(a==b)
	{
	  printf(" ta soeur %d \n",compteur);
	  if(compteur==254)
	    {
	      fputc(b,f_ecriture);
	      fputc(b,f_ecriture);
	      fputc(compteur-1,f_ecriture);
	      compteur=0;
	    }
	  compteur++;
	}
      b=a;
      a=fgetc(f_lecture);
    } 

  if (compteur < 1)
    {
      fputc(b,f_ecriture);
    }
  if ( compteur >= 1)
    {
      fputc(b,f_ecriture);
      fputc(b,f_ecriture);
      fputc(compteur-1,f_ecriture);
    }
  fclose(f_ecriture);
  fclose(f_lecture);
  return 0;	
}



int decoder_rle(void)
{
  unsigned char a,b,comp,compteur;

  FILE *f_lecture = NULL, *f_ecriture = NULL;
  
  //Si l'ouverture échoue, une erreur est envoyée
  if ((f_lecture= fopen(SORTIE,"r")) == NULL)
    {
      fprintf(stderr, "Echec de l'ouverture de %s en lecture\n",ENTREE);
      exit(EXIT_FAILURE);
    }
  if ((f_ecriture = fopen(SORTIE2,"w")) == NULL)
    {
      fprintf(stderr, "Echec de l'ouverture de %s en ecriture \n",SORTIE);
      exit(EXIT_FAILURE);
    }
  a=fgetc(f_lecture);
  b=a;
  a=fgetc(f_lecture);
  while(!feof(f_lecture))
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
      a=fgetc(f_lecture);
    }
 
  return 0;
}


//Fonction permettant d'initialiser un tableau à 2 dimensions
int init_tableau(int tab[2][255])
{
  int i,j;
  for(i=0;i<2;i++)
    {
      for(j=0;j<255;j++)
	{
	  tab[i][j]=0;
	}
    }
  return 0;
}
