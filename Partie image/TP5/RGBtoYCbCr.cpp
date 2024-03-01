/**
 * @file RGBtoYCbCr.cpp
 * @brief Convertit une image en couleur en une image en niveaux de gris et deux images en niveaux de gris pour les composantes de chrominance Cb et Cr
 * 
 */

#include <stdio.h>
#include <string.h>

#include "../lib/image_ppm.h"

int min(int a, int b) {
    if (a > b) return b;
    return a;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

void writeOutNames(char *cNomImgLue, char *cNomImgY, char *cNomImgCr, char *cNomImgCb) {
    strncpy(cNomImgY, cNomImgLue, strlen(cNomImgLue) - 4);
    strncpy(cNomImgCr, cNomImgLue, strlen(cNomImgLue) - 4);
    strncpy(cNomImgCb, cNomImgLue, strlen(cNomImgLue) - 4);
    strcat(cNomImgY, "_Y.pgm");
    strcat(cNomImgCr, "_Cr.pgm");
    strcat(cNomImgCb, "_Cb.pgm");
}

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgY[250], cNomImgCr[250], cNomImgCb[250];
    int nH, nW, nTaille, nR, nG, nB;
    double Y;

    if (argc != 2) {
        printf("Usage: ImageIn.ppm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);

    OCTET *ImgIn, *ImgY, *ImgCr, *ImgCb;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgY, OCTET, nTaille);
    allocation_tableau(ImgCr, OCTET, nTaille);
    allocation_tableau(ImgCb, OCTET, nTaille);

    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i];
        nG = ImgIn[i + 1];
        nB = ImgIn[i + 2];
        Y = 0.299 * nR + 0.587 * nG + 0.114 * nB;
        ImgY[i / 3] = int(Y);
        ImgCb[i / 3] = int(-0.1687 * nR - 0.3313 * nG + 0.5 * nB + 128);
        ImgCr[i / 3] = int(0.5 * nR - 0.4187 * nG - 0.0813 * nB + 128);
        ImgY[i / 3] = max(0, min(255, ImgY[i / 3]));
        ImgCb[i / 3] = max(0, min(255, ImgCb[i / 3]));
        ImgCr[i / 3] = max(0, min(255, ImgCr[i / 3]));
    }

    writeOutNames(cNomImgLue, cNomImgY, cNomImgCr, cNomImgCb);
    printf("Création des fichiers :\n%s\n%s\n%s\n", cNomImgY, cNomImgCr, cNomImgCb);
    ecrire_image_pgm(cNomImgY, ImgY, nH, nW);
    ecrire_image_pgm(cNomImgCr, ImgCr, nH, nW);
    ecrire_image_pgm(cNomImgCb, ImgCb, nH, nW);
    free(ImgIn);
    free(ImgY);
    free(ImgCr);
    free(ImgCb);
    return 0;
}
