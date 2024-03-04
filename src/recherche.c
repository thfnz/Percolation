#include "../include/recherche.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>

void rechercheVoisins(PIXEL pxl, RESEAU reseau, PIXEL* voisins){
	// Cette fonction renvoie une liste de pixels voisins de pxl après vérification pour les pixels présents sur la même ligne. Le test concernant les sorties par le haut est effectué lors de l'étude des voisins (cf fonction rechercheParPxl). Il est de plus impossible de sortir par le bas. 
	int i = 0;
	
	for(int j = 1; j >= -1; j = j - 2){ // j prend la valeur -1 et +1
		// Voisins en haut et en bas.
		voisins[i].x = pxl.x;
		voisins[i].y = pxl.y + j;

		//printf("voisins[i].x = %d \t voisins[i].y = %d \n", voisins[i].x, voisins[i].y);
		i += 1;

		// Sortie du réseau par la gauche.
		if(pxl.x + j < 0){
			voisins[i].x = reseau.nbCol - 1;
			voisins[i].y = pxl.y;
		}
		// Sortie du réseau par la droite.
		else if(pxl.x + j == reseau.nbCol){
			voisins[i].x = 0;
			voisins[i].y = pxl.y;
		}

		else{
			voisins[i].x = pxl.x + j;
			voisins[i].y = pxl.y;
		}

		i += 1; 
	}
}

int rechercheParPxl(PIXEL pxl, RESEAU reseau){
	int trouve = 0;

	reseau.mat[pxl.y][pxl.x] = 2; // Passage du pixel à l'état conducteur.
	if(pxl.y == reseau.nbLi - 1) return 1;
	
	PIXEL voisins[4];
	rechercheVoisins(pxl, reseau, voisins);
	for(int i = 0; i < 4; i++){
		if(voisins[i].y < 0 || trouve == 1) continue; // Il est inutile ici de vérifier que l'on ne sort pas de la matrice par le bas.
		if(reseau.mat[voisins[i].y][voisins[i].x] == 1) trouve = rechercheParPxl(voisins[i], reseau);
	}

	return trouve;
}

int rechercheRecursive(RESEAU reseau){
	// Recherche d’un chemin de percolation (version récursive).
	for(int i = 0; i < reseau.nbCol; i++){
		PIXEL pxl;
		pxl.x = i;
		pxl.y = 0;
		if(reseau.mat[pxl.y][pxl.x] == 1){
			if(rechercheParPxl(pxl, reseau)) return 1;
		}
	}
	return 0;
}

void nomPgm(RESEAU reseau, int* i){
	// Nomme et lance la création des images individuelles composant le GIF.
	char filename[27] = "./resultats/image";
	char indice[6];

	if(*i < 10) sprintf(indice, "000%d_", *i);
	else if (*i < 100) sprintf(indice, "00%d_", *i);
	else if (*i < 1000) sprintf(indice, "0%d_", *i);
	else sprintf(indice, "%d", *i);
	
	char* pgm = ".pgm";
	strcat(filename, indice);
	strcat(filename, pgm);
	imprimeReseau(reseau, filename, 5);
	*i += 1;
}

RESEAU recherche(RESEAU reseau, int gif){
	// Recherche d’un chemin de percolation (version itérative).
	int indimage = 0;
	PILE p = nouvellePile();

	for(int i = 0; i < reseau.nbCol; i++){
		PIXEL pxlPremiere;
		pxlPremiere.x = i;
		pxlPremiere.y = 0;
		
		if(reseau.mat[pxlPremiere.y][pxlPremiere.x] == 1){
			reseau.mat[pxlPremiere.y][pxlPremiere.x] = 2;
			if(gif == 1) nomPgm(reseau, &indimage); // Création d'une image à chaque instant si gif == 1.
			empile(&p, pxlPremiere);
			
			while(p.tete > 0){
				PIXEL pxl;
				pxl = depile(&p);
				if(pxl.y == reseau.nbLi - 1){
					reseau.percole = 1;
					supprPile(&p);
					return reseau;
				}

				PIXEL voisins[4];
				rechercheVoisins(pxl, reseau, voisins);
				for(int i = 0; i < 4; i++){
					if(voisins[i].y < 0) continue; // Il est inutile ici de vérifier que l'on ne sort pas de la matrice par le bas.
					if(reseau.mat[voisins[i].y][voisins[i].x] == 1){
						//pthread_mutex_lock(&mutex);
						reseau.mat[voisins[i].y][voisins[i].x] = 2;

						if(gif == 1) nomPgm(reseau, &indimage); // Création d'une image à chaque instant si gif == 1.

						empile(&p, voisins[i]);
					}
				}
			}
		}
	}

	supprPile(&p);
	return reseau;
}