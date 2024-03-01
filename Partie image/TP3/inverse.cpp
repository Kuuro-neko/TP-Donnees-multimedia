/**
 * @file inverse.cpp
 * @brief Inverse les couleurs d'une image en niveaux de gris
 *
 */

#include <stdio.h>

#include "../lib/image_ppm.h"

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgOut[250];
    int nH, nW, nTaille;

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgOut);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i = 0; i < nTaille; i++) {
        ImgOut[i] = 255 - ImgIn[i];
    }

    ecrire_image_pgm(cNomImgOut, ImgOut, nW, nH);

    return 0;
}
