#ifndef _PILE_H
#define _PILE_H

#include "commun.h"

struct stack {
	int tete;
	PIXEL* liste;
} ; typedef struct stack PILE;

PILE nouvellePile();
void supprPile(PILE* p);
void empile(PILE* p, PIXEL pxl);
PIXEL depile(PILE* p);
void affichePile(PILE* p);

#endif