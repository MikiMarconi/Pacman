#include "Astar.h"

extern char map[ROWS][COLS];
extern GHOST_STATE current_state;

int visited[ROWS][COLS];  // Array per tenere traccia dei nodi già visitati
Node allNodes[MAX_NODES];  // Array per memorizzare tutti i nodi
int allNodesCount = 0;     // Contatore per il prossimo nodo disponibile
Node* openList[MAX_NODES];  // Lista dei nodi da esplorare
int openListSize = 0;  // Numero di nodi nella lista aperta

// Funzione per calcolare la distanza Manhattan (euristica)
// Modifica per la modalità FRIGHTENED: distanza negativa per allontanarsi da Pac-Man
int heuristic(coord a, coord b) {
    return abs(a.i - b.i) + abs(a.j - b.j);  // Normale per inseguire Pacman
}

// Funzione per verificare se una posizione è valida (dentro la mappa e non un muro)
int aisValid(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && map[x][y] != '#');  // # è un muro
}

// Funzione per inserire un nodo nella lista aperta in ordine di f
void insertNodeInOpenList(Node* node) {
    openList[openListSize++] = node;

    // Ordina la lista per f (dalla più piccola alla più grande)
    int i;
    for (i = openListSize - 1; i > 0; i--) {
        if (openList[i]->f < openList[i - 1]->f) {
            Node* temp = openList[i];
            openList[i] = openList[i - 1];
            openList[i - 1] = temp;
        } else {
            break;
        }
    }
}

// Funzione per estrarre il nodo con il minimo f dalla lista aperta
Node* extractMinNode() {
    if (openListSize == 0) {
        return NULL;  // La lista è vuota
    }

    Node* minNode = openList[0];

    // Rimuove il primo nodo (minimo f)
    int i;
    for (i = 1; i < openListSize; i++) {
        openList[i - 1] = openList[i];
    }
    openListSize--;  // Riduce la dimensione della lista

    return minNode;
}

// Algoritmo A* senza heap e senza malloc
Node* aStar(coord start, coord goal) {
    openListSize = 0;
    allNodesCount = 0;  // Resetta il contatore per i nodi
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            visited[i][j] = 0;  // Segna tutte le celle come non visitate
        }
    }

    // Crea il nodo di partenza e lo aggiunge alla lista aperta
    Node* startNode = &allNodes[allNodesCount++];
    startNode->pos = start;
    startNode->g = 0;
    startNode->h = heuristic(start, goal);
    startNode->f = startNode->g + startNode->h;
    startNode->parent = NULL;

    // Inserisci il nodo di partenza nella lista aperta
    insertNodeInOpenList(startNode);
    visited[start.i][start.j] = 1;  // Segna la posizione di partenza come visitata

    // Ciclo principale dell'A*
    while (openListSize > 0) {
        // Estrai il nodo con il minimo f
        Node* current = extractMinNode();

        // Se abbiamo raggiunto l'obiettivo, ritorniamo il percorso
        if (current->pos.i == goal.i && current->pos.j == goal.j) {
            return current;
        }

        // Esamina i 4 vicini (su, giù, sinistra, destra)
        coord directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for ( i = 0; i < 4; i++) {
            coord neighbor = {current->pos.i + directions[i].i, current->pos.j + directions[i].j};

            // Verifica se il vicino è valido e non visitato
            if (aisValid(neighbor.i, neighbor.j) && !visited[neighbor.i][neighbor.j]) {
                // Crea il nodo per il vicino
                Node* neighborNode = &allNodes[allNodesCount++];
                neighborNode->pos = neighbor;
                neighborNode->g = current->g + 1;
                neighborNode->h = heuristic(neighbor, goal);
                neighborNode->f = neighborNode->g + neighborNode->h;
                neighborNode->parent = current;

                // Inserisci il vicino nella lista aperta
                insertNodeInOpenList(neighborNode);
                visited[neighbor.i][neighbor.j] = 1;  // Segna il vicino come visitato
            }
        }
    }

    return NULL;  // Se non è stato trovato alcun percorso
}

// Funzione per ottenere la prossima mossa del fantasmino
void aggiornaPercorsoFantasma(coord *fantasmaPos, coord pacManPos) {
    // Reset delle strutture per evitare conflitti tra modalità
    allNodesCount = 0;
    openListSize = 0;
    int i,j;
    for ( i = 0; i < ROWS; i++) {
        for ( j = 0; j < COLS; j++) {
            visited[i][j] = 0;  // Segna tutte le celle come non visitate
        }
    }

    // Calcola il percorso usando A* verso la posizione di Pac-Man
    Node* goalNode = aStar(*fantasmaPos, pacManPos);

    if (goalNode != NULL) {
        // Risaliamo nel percorso per trovare la prima mossa
        Node* currentNode = goalNode;
        Node* previousNode = NULL;

        // Risaliamo fino al primo nodo
        while (currentNode->parent != NULL) {
            previousNode = currentNode;
            currentNode = currentNode->parent;
        }

        // Ora previousNode è la prima mossa
        if (previousNode != NULL) {
            coord nextPos = previousNode->pos;

            // Assicuriamoci che il fantasma si muova solo se la posizione è cambiata
            if (fantasmaPos->i != nextPos.i || fantasmaPos->j != nextPos.j) {
                // Determina la mossa da fare
                if (nextPos.i > fantasmaPos->i) {
                    // Movimento giù
                    fantasmaPos->i++;
                } else if (nextPos.i < fantasmaPos->i) {
                    // Movimento su
                    fantasmaPos->i--;
                } else if (nextPos.j > fantasmaPos->j) {
                    // Movimento destra
                    fantasmaPos->j++;
                } else if (nextPos.j < fantasmaPos->j) {
                    // Movimento sinistra
                    fantasmaPos->j--;
                }
            }
        }
    } else {
        // Se non è stato trovato alcun percorso, il fantasmino non si muove
    }
}


coord trovaPosizioneLontanaDaPacman(coord pacManPos) {
    int maxDistance = -1;  // Inizializza la distanza massima a un valore molto basso
    coord posizioneLontana = { -1, -1 };  // Variabile per memorizzare la posizione più lontana

    // Scorri su tutta la mappa per trovare la posizione più lontana
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            // Verifica se la cella è valida (non è un muro e all'interno dei limiti)
            if (map[i][j] != '#' && aisValid(i, j)) {
                // Calcola la distanza Manhattan dalla posizione corrente a Pac-Man
                int distance = abs(i - pacManPos.i) + abs(j - pacManPos.j);

                // Se la distanza calcolata è maggiore di quella precedente, aggiorna la posizione
                if (distance > maxDistance) {
                    maxDistance = distance;
                    posizioneLontana.i = i;
                    posizioneLontana.j = j;
                }
            }
        }
    }

    return posizioneLontana;  // Ritorna la posizione più lontana da Pac-Man
}

  