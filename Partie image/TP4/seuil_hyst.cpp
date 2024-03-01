/**
 * @file seuil_hyst.cpp
 * @brief Seuille par hystérésis une image en niveaux de gris ImageIn.pgm avec un seuil bas Sb et un seuil haut Sh et écrit le résultat dans ImageOut.pgm
 *
 */

#include <stdio.h>

#include "../lib/image_ppm.h"

int aVoisinsContour(unsigned char *ImgIn, int i, int j, int nW) {
    return ImgIn[(i + 1) * nW + j] + ImgIn[(i - 1) * nW + j] + ImgIn[i * nW + j + 1] + ImgIn[i * nW + j - 1];
}

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, Sb, Sh;

    if (argc != 5) {
        printf("Usage: ImageIn.pgm ImageOut.pgm Seuil_bas Seuil_haut \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &Sb);
    sscanf(argv[4], "%d", &Sh);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Seuil normal pour les bords
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            if (ImgIn[i * nW + j] <= (Sb + Sh) / 2)
                ImgOut[i * nW + j] = 0;
            else
                ImgOut[i * nW + j] = 255;
        }
    }

    // 1ere lecture, valeurs non comprises entre Sb et Sh
    for (int i = 1; i < nH - 1; i++) {
        for (int j = 1; j < nW - 1; j++) {
            if (ImgIn[i * nW + j] <= Sb) ImgOut[i * nW + j] = 0;
            if (ImgIn[i * nW + j] >= Sh) ImgOut[i * nW + j] = 255;
        }
    }

    // 2e lecture, valeurs comprises entre Sb et Sh
    for (int i = 1; i < nH - 1; i++) {
        for (int j = 1; j < nW - 1; j++) {
            if (ImgIn[i * nW + j] > Sb && ImgIn[i * nW + j] < Sh) {
                if (aVoisinsContour(ImgIn, i, j, nW))
                    ImgOut[i * nW + j] = 255;
                else
                    ImgOut[i * nW + j] = 0;
            }
        }
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);

    return 0;
}
