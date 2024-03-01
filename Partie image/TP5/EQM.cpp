/**
 * @file EQM.cpp
 * @brief Calcule l'erreur quadratique moyenne entre les pixels deux images en niveaux de gris
 * 
 */

#include <math.h>
#include <stdio.h>

#include "../lib/image_ppm.h"

int main(int argc, char* argv[]) {
    char cNomImg1[250], cNomImg2[250];
    int nH1, nW1, nH2, nW2, nTaille;

    if (argc != 3) {
        printf("Usage: Image1.pgm Image2.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImg1);
    sscanf(argv[2], "%s", cNomImg2);

    OCTET *Img1, *Img2;

    lire_nb_lignes_colonnes_image_pgm(cNomImg1, &nH1, &nW1);
    nTaille = nH1 * nW1;
    lire_nb_lignes_colonnes_image_pgm(cNomImg2, &nH2, &nW2);
    if (nH1 != nH2 && nW1 != nW2) {
        printf("Les deux images doivent avoir les mêmes dimensions \n");
        exit(1);
    }
    allocation_tableau(Img1, OCTET, nTaille);
    allocation_tableau(Img2, OCTET, nTaille);
    lire_image_pgm(cNomImg1, Img1, nTaille);
    lire_image_pgm(cNomImg2, Img2, nTaille);

    int somme = 0;
    int v1, v2;

    for (int i = 0; i < nTaille; i++) {
        v1 = Img1[i];
        v2 = Img2[i];
        somme += pow((v1 - v2), 2);
    }

    double res = double(somme) / nTaille;

    printf("EQM : %f\n", res);
    free(Img1);
    free(Img2);
    return 0;
}
