#ifndef _RESEAU_H
#define _RESEAU_H

struct res {
	int** mat; // Matrice 2D représentant le réseau.
	int* nds; // Bloc mémoire contiguë qui stocke les valeurs des différents noeuds.
	int nbLi; // Nombre de lignes de la matrice.
	int nbCol; // Nombre de colonnes de la matrice.
	int percole; // Booléen indiquant si un chemin de percolation a été trouvé (1) ou non (0)
} ; typedef struct res RESEAU;

int rand_rulong(unsigned* seed);
RESEAU creationReseau(int x, int y);
RESEAU reseauAleatoire(int x, int y, float p);
int* reseauLu(char* image);
void supprReseau(RESEAU reseau);
void afficheReseau(RESEAU reseau);
void imprimeReseau(RESEAU reseau, char* chemin, int G);

#endif