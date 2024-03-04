#include "../include/evaluation.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

void* charge(void* args){
	unsigned int* result = calloc(1, sizeof(unsigned));
	ARG arg = *(ARG*)args;

	for(unsigned i = 0; i < arg.workload_per_thread; i++){
		RESEAU reseau = reseauAleatoire(nbColonnes, nbLignes, arg.prob);
		reseau = recherche(reseau, 0);
		if(reseau.percole == 1) (*result)++;
	}

	return result;
}

float calculProba(float prob, int nbTests){
	pthread_t identifiants[NB_THREADS];
	void* result;
	ARG argument;
	
	for(unsigned thread = 0; thread < NB_THREADS; thread++){
		argument.workload_per_thread = nbTests / NB_THREADS;
		argument.prob = prob;
		pthread_create(&identifiants[thread], NULL, charge, &argument);
	}
	
	unsigned somme = 0;

	for(size_t thread = 0; thread < NB_THREADS; thread++){
		pthread_join(identifiants[thread], &result);
		somme += *(unsigned int*)result;
		free(result);
	}

	return (float)somme / (float)nbTests;
}

float calculProbaSerie(float prob, int nbTests){
	int somme = 0;
	for(int i = 0; i < nbTests; i++){
		RESEAU reseau = reseauAleatoire(nbColonnes, nbLignes, prob);
		reseau = recherche(reseau, 0);
		if(reseau.percole == 1) somme += 1;
		supprReseau(reseau);
	}

	return (float)somme / (float)nbTests;
}

float evalSeuil(int nbTests){
	// Recherche dichotomique du seuil de percolation.
	float epsilon = 0.05;
	float pmin = 0.5;
	float pmax = 0.8;
	float p = 0;
	float probPercole = 0;

	while(fabs(0.5 - probPercole) > epsilon){
		p = (pmin + pmax) / 2;
		probPercole = calculProba(p, nbTests);
		if(probPercole > 0.5) pmax = p;
		else pmin = p;
	}

	return p;
}

void eval(int nbTests, int methode){
	// methode = 0 problème parrallèle, methode != 0 problème série.
	float pmin = 0.5;
	float pmax = 0.8;
	float pas = (pmax - pmin) / 500; // 300 valeur arbitraire.
	float probPercole = 0;

	// Création du fichier CSV.
	FILE* f = fopen("./resultats/evaluation.csv", "w+");
	
	for(int i = 0; i < 501; i++){
		if(methode == 0) probPercole = calculProba(pmin + i * pas, nbTests);
		else probPercole = calculProbaSerie(pmin + i * pas, nbTests);
		fprintf(f, "%f, %f\n", pmin + i * pas, probPercole);
	}

	fclose(f);
}