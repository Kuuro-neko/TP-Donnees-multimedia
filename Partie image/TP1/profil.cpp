/**
 * @file profil.cpp
 * @brief Profil d'une ligne ou d'une colonne d'une image au format pgm
 *
 * Options :
 * -l pour ligne (par défaut)
 * -c pour colonne
 */

#include <stdio.h>
#include <unistd.h>

#include "../lib/image_ppm.h"

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomFichDat[250];
    int nH, nW, nTaille, n;
    int col, opt;

    while ((opt = getopt(argc, argv, "cl")) != -1) {
        switch (opt) {
            case 'c':
                col = 1;
                break;
            case 'l':
            default:
                col = 0;
                break;
        }
    }

    if (argc - optind < 3) {
        printf("Usage: %s [-c|-l] ImageIn.pgm ProfilOut.dat Indice\n", argv[0]);
        exit(1);
    }

    sscanf(argv[2], "%s", cNomImgLue);
    sscanf(argv[3], "%s", cNomFichDat);
    sscanf(argv[4], "%d", &n);

    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    FILE *fptr = fopen(cNomFichDat, "w");

    if (col) {
        int profil[nH];
        for (int i = 0; i < nH; i++) {
            profil[i] = ImgIn[i * nW + n];
        }
        for (int i = 0; i < nH; i++) {
            fprintf(fptr, "%d %d\n", i, profil[i]);
        }
    } else {
        int profil[nW];
        for (int i = 0; i < nW; i++) profil[i] = ImgIn[n * nW + i];

        for (int i = 0; i < nW; i++)
            fprintf(fptr, "%d %d\n", i, profil[i]);
    }

    free(ImgIn);
    return 0;
}