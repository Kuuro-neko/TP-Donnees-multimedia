/**
 * @file YCbCr.cpp
 * @brief Reconstruit une image en couleur à partir de trois images en niveaux de gris pour les composantes de luminance Y et de chrominance Cb et Cr
 * 
 */

#include <math.h>
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

int debordement(int a) {
    return max(0, min(255, a));
}

int main(int argc, char* argv[]) {
    char cNomImgOut[250], cNomImgY[250], cNomImgCr[250], cNomImgCb[250];
    int nTaille, nR, nG, nB, nHY, nWY, nHCb, nWCb, nHCr, nWCr;

    if (argc != 5) {
        printf("Usage: ImageY.pgm ImageCb.pgm ImageCr.pgm nomImgOut.ppm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgY);
    sscanf(argv[2], "%s", cNomImgCb);
    sscanf(argv[3], "%s", cNomImgCr);
    sscanf(argv[4], "%s", cNomImgOut);

    OCTET *ImgOut, *ImgY, *ImgCr, *ImgCb;

    lire_nb_lignes_colonnes_image_pgm(cNomImgY, &nHY, &nWY);
    nTaille = nHY * nWY;

    lire_nb_lignes_colonnes_image_pgm(cNomImgCb, &nHCb, &nWCb);
    lire_nb_lignes_colonnes_image_pgm(cNomImgCr, &nHCr, &nWCr);
    if ((!(nHY == nHCr && nHCr == nHCb)) && (!(nWY == nWCr && nWCr == nWCb))) {
        printf("Les images Y Cr Cb doivent être de même dimensions\n");
        exit(1);
    }

    int nTaille3 = nTaille * 3;

    allocation_tableau(ImgOut, OCTET, nTaille3);

    allocation_tableau(ImgY, OCTET, nTaille);
    allocation_tableau(ImgCr, OCTET, nTaille);
    allocation_tableau(ImgCb, OCTET, nTaille);

    lire_image_pgm(cNomImgY, ImgY, nTaille);
    lire_image_pgm(cNomImgCb, ImgCb, nTaille);
    lire_image_pgm(cNomImgCr, ImgCr, nTaille);

    int ipgm;
    for (int i = 0; i < nTaille3; i += 3) {
        ipgm = i / 3;
        nR = ImgY[ipgm] + 1.40200 * (ImgCr[ipgm] - 128);
        nG = ImgY[ipgm] - 0.34414 * (ImgCb[ipgm] - 128) - 0.7141 * (ImgCr[ipgm] - 128);
        nB = ImgY[ipgm] + 1.77200 * (ImgCb[ipgm] - 128);

        nR = debordement(nR);
        nG = debordement(nG);
        nB = debordement(nB);

        ImgOut[i] = nB;
        ImgOut[i + 1] = nG;
        ImgOut[i + 2] = nR;
    }

    ecrire_image_ppm(cNomImgOut, ImgOut, nHY, nWY);
    free(ImgOut);
    free(ImgY);
    free(ImgCr);
    free(ImgCb);
    return 0;
}
