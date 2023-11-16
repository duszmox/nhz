#include "metroGraph.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "idopontok.h"
#include "utvonalterv.h"
int* dijkstra(struct MetroGraph* graph, int source);
int minDistance(int* distance, bool* visited, int V);

struct MetroGraph* createGraph(int numVertices) {
    struct MetroGraph* graph =
        (struct MetroGraph*)malloc(sizeof(struct MetroGraph));
    graph->allomasVSzam = numVertices;
    graph->tomb = (struct AllomasVertex*)malloc(numVertices *
                                                sizeof(struct AllomasVertex));

    for (int i = 0; i < numVertices; ++i) {
        graph->tomb[i].megallok = (struct Megallo**)malloc(
            sizeof(Megallo));        // You can use any data type or
                                     // structure for the vertex data
        graph->tomb[i].elek = NULL;  // Initialize the edge list for each vertex
    }

    return graph;
}

void addEdge(struct MetroGraph* graph, int src, int dest, int weight) {
    struct El* ujEl = (struct El*)malloc(sizeof(struct El));
    ujEl->celAllomas = dest;
    ujEl->suly = weight;
    ujEl->kov = graph->tomb[src].elek;
    graph->tomb[src].elek = ujEl;

    // For undirected graphs, you may also want to add the reverse edge
    // newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    // newEdge->destination = src;
    // newEdge->weight = weight;
    // newEdge->next = graph->array[dest].edges;
    // graph->array[dest].edges = newEdge;
}
void printGraph(struct MetroGraph* graph) {
    for (int i = 0; i < graph->allomasVSzam; ++i) {
        struct El* currentEdge = graph->tomb[i].elek;
        printf("Vertex %d:", i);
        for (int j = 0; j < graph->tomb[i].taroltMegallokSzama; j++) {
            printf(" %s,", graph->tomb[i].megallok[j]->nev);
        }
        printf("\n");
        while (currentEdge != NULL) {
            printf(" -> %d (%d)", currentEdge->celAllomas, currentEdge->suly);
            currentEdge = currentEdge->kov;
        }
        printf("\n");
    }
}
// void freeGraph(struct MetroGraph* graph) {
//     for (int i = 0; i < graph->allomasVSzam; ++i) {
//         struct El* currentEdge = graph->tomb[i].elek;
//         while (currentEdge != NULL) {
//             struct El* nextEdge = currentEdge->kov;
//             free(currentEdge);
//             currentEdge = nextEdge;
//         }
//         Megallo* currentMegallo = graph->tomb[i].megallok;
//         while (currentMegallo != NULL) {
//             Megallo* nextMegallo = currentMegallo->kovetkezo;
//             free(currentMegallo);
//             currentMegallo = nextMegallo;
//         }
//     }

//     free(graph->tomb);
//     free(graph);
// }
struct AllomasVertex* get_allomas_vertex_by_name(struct MetroGraph* graph,
                                                 char* name) {
    for (int i = 0; i < graph->allomasVSzam; i++) {
        for (int j = 0; j < graph->tomb[i].taroltMegallokSzama; j++) {
            if (strcmp(graph->tomb[i].megallok[j]->nev, name) == 0) {
                return &graph->tomb[i];
            }
        }
    }
    return NULL;
}

void resize_metro_graph(struct MetroGraph* graph) {
    graph->allomasVSzam++;
    graph->tomb = (struct AllomasVertex*)realloc(
        graph->tomb, graph->allomasVSzam * sizeof(struct AllomasVertex));
    graph->tomb[graph->allomasVSzam - 1].megallok =
        (Megallo**)malloc(sizeof(Megallo));  // You can use any data type or
                                             // structure for the vertex data
    graph->tomb[graph->allomasVSzam - 1].elek = NULL;
    graph->tomb[graph->allomasVSzam - 1].taroltMegallokSzama = 0;
}
int* dijkstra(struct MetroGraph* graph, int source) {
    int* distance = (int*)malloc(graph->allomasVSzam * sizeof(int));
    bool* visited = (bool*)malloc(graph->allomasVSzam * sizeof(bool));

    for (int i = 0; i < graph->allomasVSzam; i++) {
        distance[i] = INT_MAX;
        visited[i] = false;
    }

    distance[source] = 0;

    for (int count = 0; count < graph->allomasVSzam - 1; count++) {
        int u = minDistance(distance, visited, graph->allomasVSzam);
        visited[u] = true;

        struct El* currentEdge = graph->tomb[u].elek;
        while (currentEdge != NULL) {
            int v = currentEdge->celAllomas;
            if (!visited[v] && distance[u] != INT_MAX &&
                distance[u] + currentEdge->suly < distance[v]) {
                distance[v] = distance[u] + currentEdge->suly;
            }
            currentEdge = currentEdge->kov;
        }
    }

    free(visited);
    return distance;
}

int minDistance(int* distance, bool* visited, int V) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (visited[v] == false && distance[v] <= min) {
            min = distance[v];
            min_index = v;
        }
    }

    return min_index;
}
int findPrevious(int* distance, struct El* edges) {
    struct El* currentEdge = edges;
    int minDistance = INT_MAX;
    int previous = -1;

    while (currentEdge != NULL) {
        int v = currentEdge->celAllomas;
        if (distance[v] < minDistance) {
            minDistance = distance[v];
            previous = v;
        }
        currentEdge = currentEdge->kov;
    }

    return previous;
}

void printPath(struct MetroGraph* graph, int* distance, int source,
               int destination) {
    if (distance[destination] == INT_MAX) {
        printf("No path found.\n");
        return;
    }

    // Create an array to store the path
    int* path = (int*)malloc(graph->allomasVSzam * sizeof(int));
    int pathLength = 0;

    int current = destination;
    while (current != source) {
        path[pathLength++] = current;
        current = findPrevious(distance, graph->tomb[current].elek);
    }

    printf("%s", graph->tomb[source].megallok[0]->nev);

    // Print the path in reverse order
    for (int i = pathLength - 1; i >= 0; i--) {
        printf(" -> %s", graph->tomb[path[i]].megallok[0]->nev);
    }

    free(path);
}

void printSolution(struct MetroGraph* graph, int* distance, int source,
                   int destination) {
    printf("Shortest distance from %s to %s: %d\n",
           graph->tomb[source].megallok[0]->nev,
           graph->tomb[destination].megallok[0]->nev, distance[destination]);

    // You can also print the path if needed
    printf("Shortest path: ");
    printPath(graph, distance, source, destination);
    printf("\n");
}

struct Utvonalterv* dijkstra_to_utvonalterv(struct MetroGraph* graph,
                                            int* distance, int veg, int start,
                                            Idopont indulasiIdo, Metro* metro) {
    if (distance[start] == INT_MAX) {
        printf("No path found.\n");
        return NULL;
    }

    int pathLength = 0;
    int* path = (int*)malloc(pathLength * sizeof(int));

    int current = start;
    while (current != veg) {
        path = (int*)realloc(path, (pathLength + 1) * sizeof(int));
        path[pathLength++] = current;
        current = findPrevious(distance, graph->tomb[current].elek);
    }

    struct Utvonalterv* tripPlan = NULL;
    for (int i = pathLength; i >= 1; i--) {
        struct Utvonalterv* uj =
            (Utvonalterv*)malloc(sizeof(struct Utvonalterv));
        if (uj == NULL) {
            printf("Nem sikerult helyet foglalni az utvonaltervnek!\n");
            return NULL;
        }
        if (i == pathLength) {
            uj->indulo = graph->tomb[veg].megallok[0];
            uj->indulasiIdo = &indulasiIdo;
        } else {
            uj->indulo = graph->tomb[path[i]].megallok[0];
            uj->indulasiIdo = (Idopont*)malloc(sizeof(Idopont));
            Utvonalterv* elozo = tripPlan;
            while (elozo->kovetkezo != NULL) {
                elozo = elozo->kovetkezo;
            }
            *uj->indulasiIdo = *elozo->erkezesiIdo;
        }
        Megallo** celMegallok = graph->tomb[path[i - 1]].megallok;
        for (int j = 0; j < graph->tomb[path[i - 1]].taroltMegallokSzama; j++) {
            Megallo* mozgo = celMegallok[j];
            while (mozgo != NULL) {
                if (strcmp(mozgo->nev, uj->indulo->nev) != 0) {
                    uj->cel = celMegallok[j];
                    break;
                }
                mozgo = mozgo->kovetkezo;
            }
            mozgo = celMegallok[j];
            while (mozgo != NULL) {
                if (strcmp(mozgo->nev, uj->indulo->nev) != 0) {
                    uj->cel = celMegallok[j];
                    break;
                }
                mozgo = mozgo->elozo;
            }
        }
        uj->vonal = are_megallok_on_same_vonal_string(metro, uj->indulo->nev,
                                                      uj->cel->nev);
        int* megalloTav =
            megallo_distance(uj->vonal, uj->indulo->nev, uj->cel->nev);
        uj->erkezesiIdo = (Idopont*)malloc(sizeof(Idopont));
        int indulasiIdoIndex = 0;
        if (*megalloTav > 0) {
            while (ido_kisebb(uj->indulo->ido1[indulasiIdoIndex],
                              *uj->indulasiIdo)) {
                indulasiIdoIndex++;
            }
            *uj->indulasiIdo = uj->indulo->ido1[indulasiIdoIndex];
            *uj->erkezesiIdo = uj->cel->ido1[indulasiIdoIndex];
        } else {
            while (ido_kisebb(uj->indulo->ido2[indulasiIdoIndex],
                              *uj->indulasiIdo)) {
                indulasiIdoIndex++;
            }
            *uj->indulasiIdo = uj->indulo->ido2[indulasiIdoIndex];
            *uj->erkezesiIdo = uj->cel->ido2[indulasiIdoIndex];
        }
        if (tripPlan == NULL) {
            tripPlan = uj;
        } else {
            struct Utvonalterv* mozgo = tripPlan;
            while (mozgo->kovetkezo != NULL) {
                mozgo = mozgo->kovetkezo;
            }
            mozgo->kovetkezo = uj;
        }
    }
    free(path);
    return tripPlan;
}

struct Utvonalterv* utvonaltervezes(Metro* metro, char* indulo, char* cel,
                                    Idopont indulasiIdo) {
    int vonalakSzama = 0;
    struct MetroGraph* metroGraph = createGraph(0);
    Vonal* mozgo = metro->vonalak;
    AtszallasiMegallo** atszallasiMegallok = (AtszallasiMegallo**)malloc(
        sizeof(AtszallasiMegallo));  // array of pointers to atszallasi megallok
    while (mozgo != NULL) {
        AtszallasiMegallo* uj = atszallasi_megallok_on_vonal(metro, mozgo);
        if (uj == NULL) {
            printf("Nem sikerult atszallasi megallokat szerezni!\n");
            return NULL;
        }

        atszallasiMegallok[vonalakSzama] = uj;
        vonalakSzama++;
        mozgo = mozgo->kovetkezo;
    }
    for (int i = 0; i < vonalakSzama; i++) {
        AtszallasiMegallo* mozgo2 = *(atszallasiMegallok + i);
        while (mozgo2 != NULL) {
            struct AllomasVertex* allomasVertex =
                get_allomas_vertex_by_name(metroGraph, mozgo2->megallo->nev);
            if (allomasVertex == NULL) {
                resize_metro_graph(metroGraph);
                metroGraph->tomb[metroGraph->allomasVSzam - 1].megallok[0] =
                    mozgo2->megallo;
                metroGraph->tomb[metroGraph->allomasVSzam - 1]
                    .taroltMegallokSzama = 1;
            } else {
                allomasVertex->megallok[allomasVertex->taroltMegallokSzama] =
                    mozgo2->megallo;
                allomasVertex->taroltMegallokSzama++;
            }
            mozgo2 = mozgo2->kovetkezo;
        }
    }
    struct AllomasVertex* induloVertex =
        get_allomas_vertex_by_name(metroGraph, indulo);
    struct AllomasVertex* celVertex =
        get_allomas_vertex_by_name(metroGraph, cel);
    if (induloVertex == NULL) {
        resize_metro_graph(metroGraph);
        metroGraph->tomb[metroGraph->allomasVSzam - 1].megallok[0] =
            (Megallo*)malloc(sizeof(Megallo));
        Vonal* induloVonal = find_vonal_for_megallo_string(metro, indulo);
        if (induloVonal == NULL) {
            printf("Nem sikerult vonalat szerezni!\n");
            return NULL;
        }
        Megallo* induloMegallo = induloVonal->megallo;
        while (induloMegallo != NULL) {
            if (strcmp(induloMegallo->nev, indulo) == 0) {
                metroGraph->tomb[metroGraph->allomasVSzam - 1].megallok[0] =
                    induloMegallo;
                metroGraph->tomb[metroGraph->allomasVSzam - 1]
                    .taroltMegallokSzama = 1;
                break;
            }
            induloMegallo = induloMegallo->kovetkezo;
        }
    }
    if (celVertex == NULL) {
        resize_metro_graph(metroGraph);
        metroGraph->tomb[metroGraph->allomasVSzam - 1].megallok[0] =
            (Megallo*)malloc(sizeof(Megallo));
        Vonal* celVonal = find_vonal_for_megallo_string(metro, cel);
        Megallo* celMegallo = celVonal->megallo;
        while (celMegallo != NULL) {
            if (strcmp(celMegallo->nev, cel) == 0) {
                metroGraph->tomb[metroGraph->allomasVSzam - 1].megallok[0] =
                    celMegallo;
                metroGraph->tomb[metroGraph->allomasVSzam - 1]
                    .taroltMegallokSzama = 1;
                break;
            }
            celMegallo = celMegallo->kovetkezo;
        }
    }
    for (int i = 0; i < metroGraph->allomasVSzam; i++) {
        for (int j = 0; j < metroGraph->allomasVSzam; j++) {
            if (i == j) {
                continue;
            }
            char* induloVonalNev = metroGraph->tomb[i].megallok[0]->nev;
            char* celVonalNev = metroGraph->tomb[j].megallok[0]->nev;
            Vonal* kozosVonal = are_megallok_on_same_vonal_string(
                metro, induloVonalNev, celVonalNev);
            if (kozosVonal != NULL) {
                addEdge(metroGraph, i, j,
                        abs(*megallo_distance(kozosVonal, induloVonalNev,
                                              celVonalNev)));
            }
        }
    }
    printGraph(metroGraph);
    int source = metroGraph->allomasVSzam - 2;
    int destination = metroGraph->allomasVSzam -
                      1;  // Replace with the index of your destination vertex
    int* distance = dijkstra(metroGraph, source);

    // Print the solution
    printSolution(metroGraph, distance, source, destination);
    Utvonalterv* utvonalterv = dijkstra_to_utvonalterv(
        metroGraph, distance, source, destination, indulasiIdo, metro);
    Utvonalterv* mozgoUtvonalterv = utvonalterv;
    while (mozgoUtvonalterv != NULL) {
        printf("Indulo: %s\n", mozgoUtvonalterv->indulo->nev);
        printf("Cel: %s\n", mozgoUtvonalterv->cel->nev);
        printf("Vonal: %s\n", mozgoUtvonalterv->vonal->nev);
        printf("Indulasi ido: %s\n",
               idopont_to_string(*mozgoUtvonalterv->indulasiIdo));
        printf("Erkezesi ido: %s\n\n",
               idopont_to_string(*mozgoUtvonalterv->erkezesiIdo));

        mozgoUtvonalterv = mozgoUtvonalterv->kovetkezo;
    }
    free(distance);
}
