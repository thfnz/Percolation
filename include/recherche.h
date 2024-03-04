#ifndef _RECHERCHE_H
#define _RECHERCHE_H

#include "pile.h"
#include "reseau.h"

void rechercheVoisins(PIXEL pxl, RESEAU reseau, PIXEL* voisins);
int rechercheParPxl(PIXEL pxl, RESEAU reseau);
int rechercheRecursive(RESEAU reseau);
void nomPgm(RESEAU reseau, int* i);
RESEAU recherche(RESEAU reseau, int gif);

#endif