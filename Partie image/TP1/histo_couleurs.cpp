/**
 * @file histo.cpp
 * @brief Calcule l'histogramme d'une image au format ppm
 *
 * Sortie : fichier texte contenant l'histogramme
 * Chaque ligne contient un niveau d'intensité et le nombre de pixels de cette intensité pour chaque couleur (R, G, B)
 */

#include <stdio.h>
#include "../lib/image_ppm.h"

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomFichDat[250];
    int nH, nW, nTaille, nR, nG, nB;

    if (argc != 3) {
        printf("Usage: ImageIn.ppm HistogrammeOut.dat\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomFichDat);

    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    FILE *fptr = fopen(cNomFichDat, "w");

    int histo_R[256] = {0};
    int histo_G[256] = {0};
    int histo_B[256] = {0};
    
    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i];
        nG = ImgIn[i + 1];
        nB = ImgIn[i + 2];
        histo_R[nR]++;
        histo_G[nG]++;
        histo_B[nB]++;
    }

    for (int i = 0; i <= 255; i++)
        fprintf(fptr, "%d %d %d %d\n", i, histo_R[i], histo_G[i], histo_B[i]);

    free(ImgIn);
    return 0;
}
