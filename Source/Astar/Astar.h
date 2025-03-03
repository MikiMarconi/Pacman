
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Game/game.h"

#define MAX_NODES (ROWS*COLS)

typedef struct Node {
    coord pos;      // Coordinate del nodo
    int g;          // Costo per arrivare a questo nodo
    int h;          // Distanza euristica da questo nodo all'obiettivo
    int f;          // Costo totale (f = g + h)
    struct Node* parent; // Puntatore al nodo genitore
} Node;




 // Numero di nodi nella lista aperta

// Funzione per calcolare la distanza Manhattan (euristica)
int heuristic(coord a, coord b);

// Verifica se una posizione è dentro la mappa e non è un muro
//int isValid(int x, int y);

// Funzione per creare una nuova coda di priorità (heap)

coord trovaPosizioneLontanaDaPacman(coord pacManPos);
// Funzione che implementa l'algoritmo A* usando una coda di priorità (heap)
Node* aStar(coord start, coord goal);

// Funzione principale che gestisce l'aggiornamento del fantasma
void aggiornaPercorsoFantasma(coord *fantasmaPos, coord pacManPos);


