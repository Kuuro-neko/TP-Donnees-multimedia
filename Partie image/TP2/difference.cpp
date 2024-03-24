/**
 * @file difference.cpp
 * @brief Creer une image correspondant aux contours de l'image source
 *
 * Afin de permettre la detection des contours de l'image source, on utilise l'image seuillee et l'image dilatee.
 */

#include <stdio.h>
#include "../lib/image_ppm.h"

int main(int argc, char *argv[])
{
	char cNomImgSeuillee[250], cNomImgDilatee[250], cNomImgEcrite[250];
	int nH, nW, nTaille, S;

	if (argc != 4)
	{
		printf("Usage: ImageSeuillee.pgm ImageDilatee.pgm ImageOut.pgm Seuil \n");
		exit(1);
	}

	sscanf(argv[1], "%s", cNomImgSeuillee);
	sscanf(argv[2], "%s", cNomImgDilatee);
	sscanf(argv[3], "%s", cNomImgEcrite);

	OCTET *ImgSeuillee, *ImgDilatee, *ImgOut;

	lire_nb_lignes_colonnes_image_pgm(cNomImgSeuillee, &nH, &nW);
	nTaille = nH * nW;

	allocation_tableau(ImgSeuillee, OCTET, nTaille);
	allocation_tableau(ImgDilatee, OCTET, nTaille);
	lire_image_pgm(cNomImgSeuillee, ImgSeuillee, nH * nW);
	lire_image_pgm(cNomImgDilatee, ImgDilatee, nH * nW);
	allocation_tableau(ImgOut, OCTET, nTaille);

	for (int i = 0; i < nH; i++)
		for (int j = 0; j < nW; j++)
		{
			if (ImgSeuillee[i * nW + j] == ImgDilatee[i * nW + j])
				ImgOut[i * nW + j] = 255;
			else
				ImgOut[i * nW + j] = 0;
		}

	ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
	free(ImgSeuillee);
	free(ImgDilatee);
	free(ImgOut);

	return 0;
}
