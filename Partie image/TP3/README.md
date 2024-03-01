# TP3 Filtre inverse vidéo et floutage d'images

Exemples de résultats des programmes du TP3

## Inverse vidéo

Application de l'inverse vidéo sur une image et tracé du profil de la ligne 128. Le profil de l'image originale est symétrique à celui de l'image inversée par rapport à la droite f(x) = 128 (intensité max / 2)

| Image originale | Image inversée | Profils de la ligne 128 |
| --------------- | -------------- | ----------------------- |
| ![image originale](/Partie%20image/TP3/images/baboon_grey.jpg) | ![image inversée](/Partie%20image/TP3/images/baboon_inverse.jpg) | ![profil ligne 128](/Partie%20image/TP3/images/both.png) |

## Filtre flou par moyenne avec les voisins appliqué 1, 2 et 5 fois

| Image originale | Filtre flou 1 fois | Filtre flou 2 fois | Filtre flou 5 fois |
| --------------- | ------------------ | ------------------ | ------------------ |
| ![image originale](/Partie%20image/TP3/images/baboon_grey.jpg) | ![filtre flou 1 fois](/Partie%20image/TP3/images/baboon-1-flou2.jpg) | ![filtre flou 2 fois](/Partie%20image/TP3/images/baboon-2-flou2.jpg) | ![filtre flou 5 fois](/Partie%20image/TP3/images/baboon-5-flou2.jpg) |

Profils de la ligne 200 et histogrammes accumulés pour les images obtenues après 1, 2 et 5 applications du filtre flou par moyenne avec les voisins. L'application successive de flou lisse les courbes.

| Profils de la ligne 200 | Histogrammes |
| ----------------------- | ------------ |
| ![profil ligne 200](/Partie%20image/TP3/images/profil_flou_200.png) | ![histogrammes](/Partie%20image/TP3/images/histos.png) |

## Floutage d'une image en couleur

| Image originale | Filtre flou couleur |
| --------------- | ------------------- |
| ![image originale](/Partie%20image/TP1/images/baboon.jpg) | ![filtre flou couleur](/Partie%20image/TP3/images/baboon_flou.jpg) |

Le tracé (non inclus ici) du profil d'une ligne et de l'histogramme de l'image floutée en couleur comparé à ceux de l'image originale montre le même phénomène que pour l'image en niveaux de gris, mais pour chaque composante de couleur indépendamment.