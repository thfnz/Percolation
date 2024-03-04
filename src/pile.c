#include "../include/pile.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

PILE nouvellePile(){
	PILE p;
	p.tete = 0;
	p.liste = calloc(nbColonnes * nbLignes, sizeof(PIXEL));
	return p;
}

void supprPile(PILE* p){
	free(p->liste);
}

void empile(PILE* p, PIXEL pxl){
	assert(p->tete < nbLignes * nbColonnes); // Vérifie que la pile n'est pas pleine.
	p->liste[p->tete] = pxl;
	p->tete += 1;
}

PIXEL depile(PILE* p){
	assert(p->tete > 0); // Verifie que la pile n'est pas vide.
	p->tete -= 1;
	return p->liste[p->tete];
}

void affichePile(PILE* p){
	if(p->tete <= 0) printf("La pile est vide.\n");
	for(int i = 1; i <= p->tete; i++){
		printf("%d\t%d\n", p->liste[i - 1].x, p->liste[i - 1].y);
	}
	printf("\n");
}