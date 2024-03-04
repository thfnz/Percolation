#include "../include/reseau.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int rand_rExplicite(unsigned int *seed){
	// ! Uniquement nécessaire pour l'exécution du programme sur les machines de l'école (cf. readme "C99 et rand_r").
	// Source : glibc/stdlib/rand_r.c
  unsigned int next = *seed;
  int result;

  // Xn+1 = (a * Xn + c) mod m (https://fr.wikipedia.org/wiki/Générateur_congruentiel_linéaire)
  next *= 1103515245; 
  next += 12345;
  result = (unsigned int) (next / 65536) % 2048; // Décalage de 16 bits (2^16 = 65536) vers la droite -> faibles valeurs, suppression des bits de poids faibles.

  // 2e itération
  next *= 1103515245;
  next += 12345;
  result <<= 10; // Décalage de 10 bits vers la gauche.
  result ^= (unsigned int) (next / 65536) % 1024; 

  // 3e itération
  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  *seed = next;

  return result;
}

RESEAU creationReseau(int x, int y){
	// Allocation contiguë de la matrice 2D.
	int** matrice = calloc(y, sizeof(int*));
	int* noeuds = calloc(x * y, sizeof(int)); // Allocation d'un bloc mémoire contiguë qui stocke les valeurs des différents noeuds.

	for(int i = 0; i < y; i++){ // Allocation des pointeurs vers la bonne partie de la zone mémoire allouée. 
		*(matrice + i) = noeuds + i * x;
	}

	// Création d'un réseau.
	RESEAU reseau;
	reseau.mat = matrice;
	reseau.nds = noeuds;
	reseau.nbLi = y;
	reseau.nbCol = x;
	reseau.percole = 0;
	
	return reseau;
}

RESEAU reseauAleatoire(int x, int y, float p){
	RESEAU reseau = creationReseau(x, y);
	float tirage = 0;
	unsigned long seed = (unsigned long)time(NULL) + (unsigned long)pthread_self(); // Utilise l'id du thread comme seed de rand_r. L'appel de pthread_self() à cet instant permet de rendre la fonction utilisable et par la méthode // et la méthode série.

	// Affectation aléatoire des états de chaque noeud.
	for(int i = 0; i < y; i++){ // Pour chaque lignes
		for(int j = 0; j < x; j++){ // Pour chaque colonnes
			tirage = (float)rand_r((unsigned int*)&seed) / (float)RAND_MAX; // Renvoie un réel aléatoire entre 0 et 1.
			if(tirage <= p) reseau.mat[i][j] = 1;
			else reseau.mat[i][j] = 0;
		}
	}
	
	return reseau;
}

void supprReseau(RESEAU reseau){
	free(reseau.mat);
	free(reseau.nds);
}

void afficheReseau(RESEAU reseau){
	for(int i = 0; i < reseau.nbLi; i++){
		for(int j = 0; j < reseau.nbCol; j++){
			printf("%d\t", reseau.mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void imprimeReseau(RESEAU reseau, char* chemin, int G){
	// Crée une image PGM représentant le réseau.
	FILE* f = fopen(chemin, "w+");
	fprintf(f, "P2\n%d %d\n255\n", reseau.nbCol * G, reseau.nbLi * G);

	for(int i = 0; i < reseau.nbLi; i++){ // Pour chaque ligne
		for(int repet_ligne = 0; repet_ligne < G; repet_ligne++){ // Grossissement
			for(int j = 0; j < reseau.nbCol; j++){ // Pour chaque colonne
				for(int repet_pxl = 0; repet_pxl < G; repet_pxl++){ // Grossissement
					int val_pxl = 0; // Noir
					switch(reseau.mat[i][j]){
					case 1:
						val_pxl = 128; // Gris
						break;

					case 2:
						val_pxl = 255; // Blanc
						break;
					}

					fprintf(f, "%d ", val_pxl);
				}
			}
			fprintf(f, "\n");
		}
	}	

	fclose(f);
}