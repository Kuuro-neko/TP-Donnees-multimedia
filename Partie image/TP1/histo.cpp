/**
 * @file histo.cpp
 * @brief Calcule l'histogramme d'une image au format pgm
 *
 * Sortie : fichier texte contenant l'histogramme
 * Chaque ligne contient un niveau de gris et le nombre de pixels de cette intensité
 */

#include <stdio.h>
#include "../lib/image_ppm.h"

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomFichDat[250];
    int nH, nW, nTaille;

    if (argc != 3) {
        printf("Usage: ImageIn.pgm HistogrammeOut.dat\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomFichDat);

    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    FILE *fptr = fopen(cNomFichDat, "w");
    
    int hist[256] = {0};
    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++)
            hist[ImgIn[i * nW + j]]++;
    for (int i = 0; i <= 255; i++)
        fprintf(fptr, "%d %d\n", i, hist[i]);

    free(ImgIn);
    return 0;
}