#include "../include/evaluation.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{	
	int nbTests; // Nombre de tests par probabilité p.
	printf("Projet : Percolation.\n\t1. Recherche dichotomique du seuil de percolation.\n\t2. Calcul et tracé de la probabilité qu’un réseau percole en fonction de p.\n\t3. Calcul du gain de temps pratique après parallélisation (faux).\n\t4. Création d'un GIF.\n");
	int choix;
	scanf("%d", &choix);
	switch(choix){
	case 1:{
	// Recherche dichotomique du seuil de percolation.
		float seuil = evalSeuil(1000);
		printf("Le seuil de percolation vaut p_seuil = %f\n", seuil);
		break;
	}

	case 2:{
		// Calcul et tracé de la probabilité qu’un réseau percole en fonction de p.
		printf("Nombre de tests par probabilité p ? (300 recommandé)\n");
		scanf("%d", &nbTests);
		clock_t be = clock();
		eval(nbTests, 0); // Eval parallèle.
		clock_t en = clock();
		float tyes = (float)(en - be) / CLOCKS_PER_SEC;
		printf("t = %f", tyes);
		printf("Calcul terminé. Pour afficher le graphique, veuillez exécuter la commande suivante : Make graph\n");
		break;
	}

	case 3:{
		// Calcul du gain pratique après parallélisation.
		printf("Nombre de tests par probabilité p ? (100 recommandé)\n");
		scanf("%d", &nbTests);

		float tSerie = 0.0;
		float tParra = 0.0;

		clock_t begin = clock();
		eval(nbTests, 0); // Eval parallèle.
		clock_t end = clock();
		tParra = (float)(end - begin) / CLOCKS_PER_SEC;
		
		begin = clock();
		eval(nbTests, 1); // Eval série.
		end = clock();
		tSerie = (float)(end - begin) / CLOCKS_PER_SEC;

		printf("tParra = %fs\ttSerie = %fs\n", tParra, tSerie);

		break;
	}

	case 4:{
		// Création d'un GIF.
		float probp;
		printf("Probabilité p ?\n");
		scanf("%f", &probp);

		int nbL; // Nombre de lignes 
		int nbC; // Nombre de colonnes
		printf("Nombre de lignes ?\n");
		scanf("%d", &nbL);
		printf("Nombre de colonnes ?\n");
		scanf("%d", &nbC);

		RESEAU reseau = reseauAleatoire(nbC, nbL, probp);
		reseau = recherche(reseau, 1);
		supprReseau(reseau);

		printf("Calcul terminé. Pour afficher le graphique, veuillez exécuter la commande suivante : Make gif\n");
		break;
	}
	}

	return 0;
}