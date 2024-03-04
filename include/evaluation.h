#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "recherche.h"

#include <pthread.h>

#define NB_THREADS 8

struct args {
	unsigned long workload_per_thread;
	float prob;
} ; typedef struct args ARG;

void* charge(void* args);
float calculProba(float prob, int nbTests);
float evalSeuil(int nbTests);
void eval(int nbTests, int methode);

#endif