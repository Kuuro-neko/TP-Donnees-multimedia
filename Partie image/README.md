# Partie image

*Les images dans les README des TP sont converties en jpg pour être affichées, mais elles sont en réalité au format ppm ou pgm*

*Les courbes des profils et histogrammes sont tracées avec GNUPlot*

*Les README des TP contiennent des exemples de résultats des programmes et suivent la structure de mes rapports, en plus concis*

### [TP1 Prise en main d'une librairie de traitement d'images](https://github.com/Kuuro-neko/Donnes-multimedia/tree/main/Partie%20image/TP1)

Contient les programmes concernant :
- le seuillage d'une image pgm et ppm,
- le tracé de l'histogramme d'une image pgm et ppm,
- le tracé du profil d'une ligne d'une image pgm,

### [TP2 Opérations morphologiques sur des images](https://github.com/Kuuro-neko/Donnes-multimedia/tree/main/Partie%20image/TP2)

### [TP3 Filtre inverse vidéo et floutage d'images](https://github.com/Kuuro-neko/Donnes-multimedia/tree/main/Partie%20image/TP3)

Contient les programmes concernant :
- l'inversion des niveaux de gris d'une image pgm,
- le floutage d'une image :
    - pgm, par moyenne avec les 4 voisins (filtre_flou1.cpp)
    - pgm, par moyenne avec les 8 voisins (filtre_flou2.cpp)
    - pgm, par gaussienne avec les 8 voisins (filtre_gaussien.cpp)
    - ppm, par moyenne avec les 8 voisins pour chaque couleur (filtre_flou_couleur.cpp)

### [TP4 Détection des contours d'une image avec utilisation du gradient (1er ordre)](https://github.com/Kuuro-neko/Donnes-multimedia/tree/main/Partie%20image/TP4)

Contient les programmes concernant :
- la création d'une carte de gradient d'une image pgm (norme_gradient.cpp)
- le seuillage par hystérésis d'une carte de gradient (seuil_hyst.cpp)

Pour prétraiter les images afin de faciliter la détection des contours, on peut réutiliser filtre_flou2.cpp et filtre_gaussien.cpp du TP3

### [TP5 Transformation d'espaces couleur](https://github.com/Kuuro-neko/Donnes-multimedia/tree/main/Partie%20image/TP5)

Contient les programmes concernant :
- la conversion d'une image ppm en une image pgm (RGBtoY.cpp)
- le calcul de l'erreur quadratique moyenne entre les pixels de deux images (EQM.cpp)
- la transformation d'une image ppm en trois images ppm correspondant à ses composantes Y, Cb et Cr (RGBtoYCbCr.cpp)
- la reconstruction d'une image ppm à partir de ses composantes Y, Cb et Cr (YCbCrtoRGB.cpp)
- la modification de la luminance de la composante Y d'une image au format pgm (modifY.cpp)