/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure_dico.h"

int main (int argc, char *argv[])
{
	if (argc <=3 && argc >=2)
	{
		if(strcmp(argv[argc-1],"-c"))
		{
			printf("\n\nCompression ");
			Compression(argv[argc-1]);
		}
		else if(strcmp(argv[argc-1],"-d"))
		{
			printf("\n\nDecompression");
			Decompression(argv[argc-1]);
		}
	}
	else
	{
		printf("\n\nErreur d'utilisation : ./lzw [-c/-d] [fichier]");
	}
	printf("\n");
	return 0;
}


