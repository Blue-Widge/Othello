Jeu de l'othello : 
Vous pouvez changer tout en haut "width" et "height" pour changer la taille du plateau, j'avais prévu de faire en sorte que le plateau puis être rectangulaire mais ça ne s'est pas fait
On peut choisir de faire du Joueur contre Joueur, une partie contre une IA contre regarder une partie en IA
l'affichage enh couleur est compatible windows et linux, en affichage console uniquement
Un algorithme d'alpha béta est utilisée pour l'IA, le meilleur coup est calculé en fonction de trois facteurs : la différence de pions avec l'adversaire, le poids de poser ses pions le plus loin du centre et le poids de controler les coins
Le code a été optimisé pour limiter grandement les fuites mémoires (aucune il me semble)
Aucune librairie externe n'est requise