# TP5 Transformation d'espaces couleur

Exemples de résultats des programmes du TP5

## Obtention d'une image en niveaux de gris à partir d'une image couleur

Calcul effectué par mon programme : Y = 0.299 R + 0.587 G + 0.114 B

| Image couleur | Image en niveaux de gris (gimp) | Image en niveaux de gris (programme) |
| ------------- | ------------------------------- | ----------------------------------- |
| ![image couleur](/Partie%20image/TP1/images/baboon.jpg) | ![image en niveaux de gris gimp](/Partie%20image/TP5/images/baboon_grey_gimp.jpg) | ![image en niveaux de gris programme](/Partie%20image/TP5/images/baboon_grey.jpg) |

```
> ./bin/EQM baboon_gris_gimp.pgm mon_baboon_gris.pgm
Erreur quadratique moyenne : 29.994217
```

L'image obtenue par mon programme n'est pas exactement la même que celle obtenue par Gimp, mais elle est très proche (à 10% d'erreur près environ). Gimp utilise une formule différente pour obtenir l'image en niveaux de gris.

## Transformation de l'espace RGB vers l'espace YCbCr

Calculs effectués par mon programme :
- Y = 0.299 R + 0.587 G + 0.114 B
- Cb = -0.1687 R - 0.3313 G + 0.5 B + 128
- Cr = 0.5 R - 0.4187 G - 0.0813 B + 128

| Image couleur | Composante Y | Composante Cb | Composante Cr |
| ------------- | ------------ | ------------- | ------------- |
| ![image couleur](/Partie%20image/TP5/images/peppers.jpg) | ![composante Y](/Partie%20image/TP5/images/peppers_Y.jpg) | ![composante Cb](/Partie%20image/TP5/images/peppers_Cb.jpg) | ![composante Cr](/Partie%20image/TP5/images/peppers_Cr.jpg) |

Y stocke les détails de l'image, Cb et Cr stockent les couleurs. Cela permet de compresser les images.

## Reconstruction d'une image couleur à partir de ses composantes Y, Cb et Cr

Calculs effectués par mon programme :
- R = Y + 1.402 * (Cr - 128)
- G = Y - 0.34414 * (Cb - 128) - 0.7141 * (Cr - 128)
- B = Y + 1.772 * (Cb - 128)

Les valeurs R, G, B sont tronquées à 0 et 255 si elles dépassent ces valeurs.

| Image reconstruite sans corriger | Image reconstruite | Image originale |
| ------------------------------------------ | ------------------ | --------------- |
| ![image reconstruite sans tronquer](/Partie%20image/TP5/images/peppers_recons_bad.png) | ![image reconstruite](/Partie%20image/TP5/images/peppers_recons.jpg) | ![image originale](/Partie%20image/TP5/images/peppers.jpg) |

## Inversions de composantes à la reconstruction

En mélangeant les valeurs des composantes R, G et B à la reconstruction, on obtient des images avec des couleurs différentes. Voici les 6 combinaisons possibles :

| Image RGB | Image RBG | Image GRB |
| --------- | --------- | --------- |
| ![image RBG](/Partie%20image/TP5/images/peppers_recons.jpg) | ![image RGB](/Partie%20image/TP5/images/outRBG.jpg) | ![image GRB](/Partie%20image/TP5/images/outGRB.jpg) |

| Image GBR | Image BRG | Image BGR |
| --------- | --------- | --------- |
| ![image GBR](/Partie%20image/TP5/images/outGBR.jpg) | ![image BRG](/Partie%20image/TP5/images/outBRG.jpg) | ![image BGR](/Partie%20image/TP5/images/outBGR.jpg) |

## Modification de la luminance de l'image

En modifiant la composante Y de l'image en ajoutant k (tel que -128 < k < 128), on modifie sa luminance. Voici l'image originale et l'image modifiée :

| Image originale | Composante Y avec k = 128 | Composante Y avec k = -128 | Image modifiée avec k = 128 | Image modifiée avec k = -128 |
| --------------- | ------------------------- | -------------------------- | -------------------------- | --------------------------- |
| ![image originale](/Partie%20image/TP5/images/peppers.jpg) | ![composante Y avec k = 128](/Partie%20image/TP5/images/modify128.jpg) | ![composante Y avec k = -128](/Partie%20image/TP5/images/modify-128.jpg) | ![image modifiée avec k = 128](/Partie%20image/TP5/images/peppers_recons_modifY128.jpg) | ![image modifiée avec k = -128](/Partie%20image/TP5/images/peppers_recons_modifY-128.jpg) |