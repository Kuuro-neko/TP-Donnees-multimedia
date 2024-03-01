# TP4 Détection des contours d'une image avec utilisation du gradient (1er ordre)

Exemples de résultats des programmes du TP4

## Création d'une carte de gradient d'une image pgm

| Image originale | Carte de gradient |
| --------------- | ----------------- |
| ![image originale](/Partie%20image/TP4/images/14.jpg) | ![carte de gradient](/Partie%20image/TP4/images/14_gradient.jpg) |

Sur les images obtenues, on peut voir que les contours sont bien détectés.

| Profil de la ligne 256 | Profil de la colonne 100 |
| ---------------------- | ------------------------ |
| ![profil ligne 256](/Partie%20image/TP4/images/14_g_l_256.png) | ![profil colonne 100](/Partie%20image/TP4/images/14_g_c_100.png) |

Sur les profils de lignes et de colonnes, les pics correspondent bien aux contours de l'image. Le bruit après 350 sur le profil de la colonne 100 est dû au bruit dans la montagne.

## Extraction des maximums locaux par seuillage

| Carte de gradient | Seuil = 5 | Seuil = 35 | Seuil = 75 |
| ----------------- | ---------- | ---------- | ---------- |
| ![carte de gradient](/Partie%20image/TP4/images/14_gradient.jpg) | ![seuil 5](/Partie%20image/TP4/images/14-g-seuil-5.jpg) | ![seuil 35](/Partie%20image/TP4/images/14-g-seuil-35.jpg) | ![seuil 75](/Partie%20image/TP4/images/14-g-seuil-75.jpg) |

Pour un seuil de :
- 5, la valeur est trop basse et on augmente le bruit
- 35, on obtient une carte de contours correcte
- 75, on perd des contours

## Seuillage par hystérésis

| Carte de gradient | Seuils = 5, 15 | Seuils = 25, 40 | Seuils = 70, 90 |
| ----------------- | -------------- | --------------- | --------------- |
| ![carte de gradient](/Partie%20image/TP4/images/14_gradient.jpg) | ![seuils 5, 15](/Partie%20image/TP4/images/14_g_hyst_5_15.jpg) | ![seuils 25, 40](/Partie%20image/TP4/images/14_g_hyst_25_40.jpg) | ![seuils 70, 90](/Partie%20image/TP4/images/14_g_hyst_70_90.jpg) |

En comparaison avec le seuillage simple, le seuillage par hystérésis permet de diminuer le bruit et d'avoir des contours mieux marqués pour des valeurs de seuil similaires.

| Profil de la ligne 256 carte de gradient | Profil de la ligne 256 carte de gradient seuillée par hystérésis | 
| --------------------------------------- | ----------------------------------------------------------- |
| ![profil ligne 256](/Partie%20image/TP4/images/14_g_l_256.png) | ![profil ligne 256 seuillée](/Partie%20image/TP4/images/14_g_hyst_25_40_l_256.png) |

Ces profils montrent l'efficacité du seuillage par hystérésis pour éliminer le bruit et marquer les contours.

## Prétraitement des images

Le préatraitement des images par floutage (ici, flou gaussien du TP3) permet de faciliter grandement la détection des contours et de diminuer le bruit, exemple :

| Image originale | Contours sans prétraitement | Contours avec prétraitement |
| --------------- | -------------------------- | --------------------------- |
| ![image originale](/Partie%20image/TP4/images/house.jpg) | ![contours sans prétraitement](/Partie%20image/TP4/images/house_contours_non_pretraitee.jpg) | ![contours avec prétraitement](/Partie%20image/TP4/images/house_contours_pretraitee.jpg) |
