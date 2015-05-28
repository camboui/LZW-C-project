/*	POLYTECH Grenoble - RICM3 2015- PLS
	David BUI
	Yohann MATTEO
	Mathias BRANGER
	Florian PIERRE-LOUIS
*/

void Compression (char *nom_fichier)
{
	Dictionnaire dico = NULL;
	dico = Init ();
	if (fichier == NULL)
	{
		fprintf (stderr,"Echec de l'initialisation du dico\n");
		exit (EXIT_FAILURE);
	}
	
	FILE *fichier;
	fichier = fopen (nom_fichier,"r");
	if (fichier == NULL)
	{
		fprintf (stderr,"Echec de l'ouverture de fichier_de_contenu en lecture\n");
		exit (EXIT_FAILURE);
	}
	
	
	
	
	fclose(fichier);
}
