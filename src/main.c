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
#include "RLE.h"


int main (int argc, char *argv[])
{

	if (argc == 3 )
	{
		if(strcmp(argv[argc-2],"-c")==0)
		{
			printf("\nCompression ");
			Compression(argv[argc-1]);
			printf("\n --- FIN --- ");
		}
		else if(strcmp(argv[argc-2],"-d")==0)
		{
			printf("\nDecompression");
			Decompression(argv[argc-1]);
			printf("\n --- FIN --- ");
		}
		else if(strcmp(argv[argc-2],"-rc")==0)
		{
			printf("\nCompression RLE");
			rle(argv[argc-1]);
			printf("\n --- FIN --- ");
		}
		else if(strcmp(argv[argc-2],"-rd")==0)
		{
			printf("\nDecompression RLE");
			decoder_rle(argv[argc-1]);
			printf("\n --- FIN --- ");
		}
		else
		{
			printf("\nErreur d'utilisation : ./lzw [-c/-d/-rc/-rd] [fichier]");
		}
	}
	else
	{
		printf("\nErreur d'utilisation : ./lzw [-c/-d/-rc/-rd] [fichier]");
	}
	printf("\n\n");
	return 0;
}


