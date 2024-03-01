/**
 * @file test_grey_4seuil.cpp
 * @brief Seuille une image en niveau de gris en 4 parties
 *
 */
#include <stdio.h>

#include "../lib/image_ppm.h"

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, S1, S2, S3;

    if (argc != 6) {
        printf("Usage: ImageIn.pgm ImageOut.pgm Seuil1 Seuil2 Seuil3\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &S1);
    sscanf(argv[4], "%d", &S2);
    sscanf(argv[5], "%d", &S3);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            if (ImgIn[i * nW + j] < S1) {
                ImgOut[i * nW + j] = 0;
            } else {
                if (ImgIn[i * nW + j] < S2) {
                    ImgOut[i * nW + j] = 255 / 3;
                } else {
                    if (ImgIn[i * nW + j] < S3)
                        ImgOut[i * nW + j] = 255 / 3 * 2;
                    else
                        ImgOut[i * nW + j] = 255;
                }
            }
        }
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    
    free(ImgIn);
    free(ImgOut);
    return 0;
}
