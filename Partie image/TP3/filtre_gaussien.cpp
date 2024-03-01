/**
 * @file filtre_gaussien.cpp
 * @brief Filtre l'image en niveaux de gris ImageIn.pgm avec un filtre flou et écrit le résultat dans ImageOut.pgm
 *
 * Flou par gaussienne des 8 pixels voisins
 */

#include <stdio.h>

#include "../lib/image_ppm.h"

int alpha(int di, int dj) {
    if (di == 0 && dj == 0) return 4;     // Case du milieu, di et dj = 0
    if (di != 0 && dj != 0) return 1;     // Coins di et dj != 0
    if (!(di != 0 && dj != 0)) return 2;  // Cotés di ou dj != 0 mais pas les 2
    return -1;
}

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    int somme, alpha_somme;

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
            somme = 0;
            alpha_somme = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (alpha(di, dj) == -1) {
                        printf("erreur alpha");
                        exit(1);
                    }
                    somme += ImgIn[(i + di) * nW + (j + dj)] * alpha(di, dj);
                    alpha_somme += alpha(di, dj);
                }
            }
            ImgOut[i * nW + j] = somme / alpha_somme;
        }
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
