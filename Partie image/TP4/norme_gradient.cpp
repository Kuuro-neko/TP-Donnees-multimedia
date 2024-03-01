/**
 * @file norme_gradient.cpp
 * @brief Calcule la norme du gradient de l'image en niveaux de gris ImageIn.pgm et écrit le résultat dans ImageOut.pgm
 * 
 */
#include <math.h>
#include <stdio.h>

#include "../lib/image_ppm.h"

int grad_ligne(unsigned char *Img, int i, int j, int nW) {
    return (Img[i * nW + j + 1] - Img[i * nW + j]);
}

int grad_colonne(unsigned char *Img, int i, int j, int nW) {
    return (Img[(i + 1) * nW + j] - Img[i * nW + j]);
}

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgOut[250];
    int nH, nW, nTaille, nTailleOut;
    double G;

    if (argc != 3) {
        printf("Usage: ImageIn.pgm ImageOut.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgOut);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    nTailleOut = (nH - 1) * (nW - 1);

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i = 0; i < nH - 1; i++) {
        for (int j = 0; j < nW - 1; j++) {
            G = sqrt(
                pow(grad_ligne(ImgIn, i, j, nW), 2) + pow(grad_colonne(ImgIn, i, j, nW), 2));
            ImgOut[i * nW + j] = G;
        }
    }

    ecrire_image_pgm(cNomImgOut, ImgOut, nW, nH);

    return 0;
}
