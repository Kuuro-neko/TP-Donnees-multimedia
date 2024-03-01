/**
 * @file RGBtoY.cpp
 * @brief Convertit une image en couleur en une image en niveaux de gris
 * 
 */

#include <stdio.h>

#include "../lib/image_ppm.h"

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgOut[250];
    int nH, nW, nTaille, nTaille3, nR, nG, nB;

    if (argc != 3) {
        printf("Usage: ImageIn.ppm ImageOut.pgm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgOut);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i];
        nG = ImgIn[i + 1];
        nB = ImgIn[i + 2];
        ImgOut[i / 3] = int(0.299 * nR + 0.587 * nG + 0.114 * nB);
    }

    ecrire_image_pgm(cNomImgOut, ImgOut, nW, nH);

    return 0;
}
