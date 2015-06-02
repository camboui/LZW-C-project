#include <stdio.h>
#include <stdlib.h>
#define ENTREE "entree"
#define SORTIE "sortie"
#define SORTIE2 "sortiedecoder"
#define SORTIE3 "freq.txt"

int rle(void)
{
  //premier permet que le premier caratère lu soit correct
  unsigned char compteur=1;
  unsigned int a,b;

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
	  //printf("valeur compteur codage : %d\n",compteur);
	  fputc(compteur-2,f_ecriture);
	  compteur=1;
	}
      else if(a==b)
	{
	  //printf(" ta soeur %d \n",compteur);
	  if(compteur==255)
	    {
	      fputc(b,f_ecriture);
	      fputc(b,f_ecriture);
	      //      printf("valeur compteur codage : %d\n",compteur);
	      fputc(compteur-2,f_ecriture);
	      compteur=0;
	    }
	  if(compteur>255)
	    {
	      printf("erreur	overflow \n");
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
      //printf("valeur %d \n",b);
    }
  if ( compteur >= 2)
    {
      fputc(b,f_ecriture);
      fputc(b,f_ecriture);
      //printf("valeur compteur codage de fin : %d\n",compteur);
      fputc(compteur-2,f_ecriture);
    }
  //fputc(a,f_ecriture);  caractere fin de chaine à impr ?
  fclose(f_ecriture);
  fclose(f_lecture);
  return 0;	
}



int decoder_rle(void)
{
  unsigned int a,b;
    unsigned char comp,compteur;

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
	  //	 printf("valeur compteur decodage %d : \n ",comp);
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
  return 0;
}
