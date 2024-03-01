/**
 * @file filtre_flou_couleur.cpp
 * @brief Filtre l'image en couleur ImageIn.ppm avec un filtre flou et écrit le résultat dans ImageOut.ppm
 * 
 * Flou par moyenne des 8 pixels voisins
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

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille * 3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille * 3);

    for (int i = 0; i < nTaille * 3; i++) {
        ImgOut[i] = ImgIn[i];
    }

    for (int i = 1; i < nH - 1; i++) {
        for (int j = 3; j < nW * 3 - 3; j += 3) {
            // pour chaque couleur
            for (int c = 0; c <= 2; c++) {
                somme = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -3; dj <= 3; dj += 3) {
                        somme += ImgIn[(i + di) * (nW * 3) + (j + dj) + c];
                    }
                }
                ImgOut[i * (nW * 3) + j + c] = somme / 9;
            }
        }
    }

    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
