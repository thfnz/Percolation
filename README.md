# Projet : Percolation - ferthoma

Ce programme comporte 4 fonctionnalités :
- recherche dichotomique du seuil de percolation;
- calcul et tracé de la probabilité qu’un réseau percole en fonction de p;
- calcul du gain de temps pratique après parallélisation;
- création d'un GIF animé.

## Make et Make clean

Avant première compilation du programme, les dossiers ./bin, ./obj et ./resultats doivent être créés à partir de :

```bash
  Make clean
```

Cette commande supprime également les potentiels anciens résultats et exécutables.

La compilation du programme se fait à l'aide de la commande :

```bash
  Make
```

Elle génère un exécutable ./bin/main.exe.


## Recherche dichotomique du seuil de percolation.
Cette première fonction calcul et renvoie la probabilité p_seuil pour laquelle chaque réseau aléatoire a 50% de chance d'avoir au moins un chemin de percolation possible et 50% de chance d'être fermé.

## Calcul et tracé de la probabilité qu’un réseau percole en fonction de p.

Cette fonction permet le tracé de la probabilité qu'un réseau percole en fonction de p. Il est obtenu à l'aide de la commande :

```bash
  Make graph
```

#### Résultat.

![](https://www.zupimages.net/up/23/19/epo8.png)

(Ce graphique a été obtenu avec nbLignes = nbColonnes = 500 et nbTests = 500).

## Calcul du gain de temps pratique après parallélisation (faux).

Cette fonction affiche la différence de temps nécessaire à l'exécution de la partie précédente entre la méthode série et la méthode parallèle.

#### Résultat.

On constate que tParra > tSerie. Ainsi les recherches des chemins de percolation lors de l'appel des autres fonctions du programme seront réalisées en série .

## Création d'un GIF animé.

Cette fonction crée un GIF animé représentant la recherche d'un chemin de percolation.

Une fois le programme exécuté, le GIF peut être généré à l'aide de la commande :

```bash
  Make gif
```

! Attention à ne pas choisir des dimensions trop importantes pour le réseau, le GIF étant limité à 9999 images.

#### Résultat.

![](https://www.zupimages.net/up/23/20/jds1.gif)

#### Bug.

- Comme le montre l'exemple animé ci-dessus, l'utilisation des structures PILE lors de la recherche itérative force la fonction à explorer d'abord les noeuds au dessus du pixel étudié avant ceux d'en dessous.

## C99 et rand_r.

Afin de rendre la génération des réseaux aléatoires thread-safe, la fonction rand_r() doit être utilisée à la place de la fonction rand().

La seed utilisée par la fonction rand_r est d'ailleurs générée à l'aide de l'identifiant du thread utilisé et le temps par :

```C
   unsigned long seed = (unsigned long)time(NULL) + (unsigned long)pthread_self();
```

Or, pour garantir l'exécution du programme sur les machines de l'école, ce dernier est compilé à l'aide de la version C99 qui devrait contenir une déclaration implicite de la fonction rand_r (source : https://linux.die.net/man/3/rand_r).

Pourtant, une erreur de type "warning : implicit declaration of rand_r" est obtenue lors de la compilation. Elle sera donc explicitement déclarée dans le fichier "./src/reseau.c" sous le nom rand_rExplicite (cf. ligne 63).

## Problème de parallélisation.

L'objectif de la parallélisation du problème était de répartir la création des réseaux aléatoires et leur recherche respective sur plusieurs threads afin de gagner du temps de calcul. Or, comme le montre la partie "calcul du gain de temps pratique après parallélisation", cette dernière s'avère inefficace. 

Pourtant, la commande 
```bash
  top
```
affiche bel et bien que main.exe utilise le nombre de threads demandé. De plus, plus ce dernier est faible, plus tParra diminue et se rapproche de tSérie, indiquant qu'en réalité, la majorité des threads restent en attente.

Pourtant, sauf pour la variable ARG argument (cf l24 du fichier evaluation.c), aucune variable ne semble être partagée entre les threads.

## Piles allouées dynamiquement.

Pour obtenir des résultats satisfaisants, il est important lors des recherches de générer des réseaux d'une taille assez grande (250px x 250 px par défaut). Or, lors de l'exécution de la méthode parallèle, si pile.liste n'est pas allouée dynamiquement et si les réseaux sont créés trop grands (300px x 300px par exemple), une erreur de type "Zsh : bus error" peut apparaître à cause de la création de la PILE p lors de l'appel de la fonction recherche() Plus spécifiquement, c'est l'instruction :

```bash
  p.tete = 0;
```

qui est source d'erreur. La taille limite a l'air de dépendre de la machine sur laquelle le programme s'exécute. 

Ainsi, les piles seront allouées dynamiquement à l'aide de :
```C
PILE nouvellePile(){
  PILE p;
  p.tete = 0;
  p.liste = calloc(nbColonnes * nbLignes, sizeof(PIXEL));
  return p;
}

void supprPile(PILE* p){
  free(p->liste);
}
```

Même avec ces changements, une erreur concurrente (zsh: killed) apparaît lorsque l'on demande la création de réseau particulièrement imposants (1000px x 1000px par exemple) mais ce cas peut être oublié car sans, les temps de calculs seraient dans tous les cas bien trop importants.