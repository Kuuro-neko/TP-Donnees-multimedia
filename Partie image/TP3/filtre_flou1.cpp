/**
 * @file filtre_flou1.cpp
 * @brief Filtre l'image en niveaux de gris ImageIn.pgm avec un filtre flou et écrit le résultat dans ImageOut.pgm
 *
 * Flou par moyenne des 4 pixels voisins
 */

#include <stdio.h>

#include "../lib/image_ppm.h"

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    int somme;

    if (argc != 3) {
        printf("Usage: ImageIn.pgm ImageOut.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i = 0; i < nTaille; i++) {
        ImgOut[i] = ImgIn[i];
    }

    for (int i = 1; i < nH - 1; i++) {
        for (int j = 1; j < nW - 1; j++) {
            somme = ImgIn[i * nW + j] + ImgIn[(i + 1) * nW + j] + ImgIn[(i - 1) * nW + j] + ImgIn[i * nW + (j + 1)] + ImgIn[i * nW + (j - 1)];
            ImgOut[i * nW + j] = somme / 5;
        }
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
