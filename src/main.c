/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compression.h"
#include "decompression.h"

int main (int argc, char *argv[])
{

	if (argc == 3 )
	{
		if(strcmp(argv[argc-2],"-c")==0)
		{
			printf("\nCompression ");
			Compression(argv[argc-1]);
		}
		else if(strcmp(argv[argc-2],"-d")==0)
		{
			printf("\nDecompression");
			Decompression(argv[argc-1]);
		}
		else
		{
			printf("\nErreur d'utilisation : ./lzw [-c/-d] [fichier]");
		}
	}
	else
	{
		printf("\nErreur d'utilisation : ./lzw [-c/-d] [fichier]");
	}
	printf("\n\n");
	return 0;
}


