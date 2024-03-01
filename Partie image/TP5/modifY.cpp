/**
 * @file modifY.cpp
 * @brief Ajoute une constante -128 < k < 128 à chaque pixel de l'image en niveaux de gris ImageIn.pgm et écrit le résultat dans ImageIn_modifY.pgm
 *
 */

#include <stdio.h>

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

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, k;

    if (argc != 4) {
        printf("Usage: ImageIn.pgm ImageOut.pgm k \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &k);

    if (k < -128 || k > 128) {
        printf("k doit être compris entre -128 et 128. valeur de k : %d\n", k);
        exit(1);
    }

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i = 0; i < nTaille; i++) {
        ImgOut[i] = debordement(ImgIn[i] + k);
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

    free(ImgIn);
    free(ImgOut);
    return 0;
}
