/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

#include <stdio.h>
#include <stdlib.h>


int main (int argc, char *argv[])
{

	if (argc <=3)
	{
		if(strcmp(argv[],"-c"))
		{
			printf("\n\nCompression ");
		}
		else if(strcmp(argv[],"-d"))
		{
			printf("\n\nDecompression");
		}
	}
	else
	{
		printf("\n\nErreur d'utilisation : ./lzw [-c/-d] [fichier]");
	}
	
	return 0;
}


