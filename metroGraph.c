#include "metroGraph.h"

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debugmalloc.h"
#include "idopontok.h"
#include "utvonalterv.h"

int* dijkstra(struct MetroGraph* graph, int source);
int minDistance(int* distance, bool* visited, int V);

struct MetroGraph* createGraph(int allomasVSzam) {
    struct MetroGraph* graph =
        (struct MetroGraph*)malloc(sizeof(struct MetroGraph));
    graph->allomasVSzam = allomasVSzam;
    graph->tomb = (struct AllomasVertex*)malloc(allomasVSzam *
                                                sizeof(struct AllomasVertex));

    for (int i = 0; i < allomasVSzam; ++i) {
        graph->tomb[i].megallok = (struct Megallo**)malloc(sizeof(Megallo));
        graph->tomb[i].elek = NULL;
    }

    return graph;
}

void addEl(struct MetroGraph* graph, int src, int cel, int suly) {
    struct El* ujEl = (struct El*)malloc(sizeof(struct El));
    ujEl->celAllomas = cel;
    ujEl->suly = suly;
    ujEl->kov = graph->tomb[src].elek;
    graph->tomb[src].elek = ujEl;
}

void freeGraph(struct MetroGraph* graph) {
    for (int i = 0; i < graph->allomasVSzam; i++) {
        struct El* currentEdge = graph->tomb[i].elek;
        while (currentEdge != NULL) {
            struct El* tmp = currentEdge;
            currentEdge = currentEdge->kov;
            free(tmp);
        }
        free(graph->tomb[i].megallok);
    }
    free(graph->tomb);
    free(graph);
}
int* get_allomas_vertex_by_name(struct MetroGraph* graph,
                                const char* megalloNev) {
    for (int i = 0; i < graph->allomasVSzam; i++) {
        for (int j = 0; j < graph->tomb[i].taroltMegallokSzama; j++) {
            if (strcmp(graph->tomb[i].megallok[j]->nev, megalloNev) == 0) {
                int* index = (int*)malloc(sizeof(int));
                *index = i;
                return index;
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

// Forrás: http://cs.bme.hu/bsz2/bsz2_jegyzet.pdf 160. oldal
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
        Idopont* tmpIndulasiIdo = (Idopont*)malloc(sizeof(Idopont));
        uj->indulasiIdo = (Idopont*)malloc(sizeof(Idopont));
        if (i == pathLength) {
            *tmpIndulasiIdo = indulasiIdo;
        } else {
            Utvonalterv* elozo = tripPlan;
            while (elozo->kovetkezo != NULL) {
                elozo = elozo->kovetkezo;
            }
            *tmpIndulasiIdo = *elozo->erkezesiIdo;
        }
        Megallo** induloMegallo =
            graph->tomb[i == pathLength ? veg : path[i]].megallok;
        Megallo** celMegallo = graph->tomb[path[i - 1]].megallok;
        for (int j = 0;
             j <
             graph->tomb[i == pathLength ? veg : path[i]].taroltMegallokSzama;
             j++) {
            Megallo* common = is_string_on_megallo_vonal(induloMegallo[j],
                                                         celMegallo[0]->nev);
            if (common != NULL) {
                uj->cel = common;
                break;
            }
        }
        for (int j = 0; j < graph->tomb[path[i - 1]].taroltMegallokSzama; j++) {
            Megallo* common = is_string_on_megallo_vonal(celMegallo[j],
                                                         induloMegallo[0]->nev);
            if (common != NULL) {
                uj->indulo = common;
                break;
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
                              *tmpIndulasiIdo)) {
                indulasiIdoIndex++;
                if (indulasiIdoIndex == uj->indulo->ido1Hossz) {
                    free_utvonalterv(tripPlan);
                    free(megalloTav);
                    free(tmpIndulasiIdo);
                    free(uj->indulasiIdo);
                    free(uj->erkezesiIdo);
                    free(uj);
                    free(path);
                    return NULL;
                };
            }
            *uj->indulasiIdo = uj->indulo->ido1[indulasiIdoIndex];
            *uj->erkezesiIdo = uj->cel->ido1[indulasiIdoIndex];
        } else {
            while (ido_kisebb(uj->indulo->ido2[indulasiIdoIndex],
                              *tmpIndulasiIdo)) {
                indulasiIdoIndex++;
                if (indulasiIdoIndex == uj->indulo->ido2Hossz) {
                    free_utvonalterv(tripPlan);
                    free(megalloTav);
                    free(tmpIndulasiIdo);
                    free(uj->indulasiIdo);
                    free(uj->erkezesiIdo);
                    free(uj);
                    free(path);
                    return NULL;
                };
            }
            *uj->indulasiIdo = uj->indulo->ido2[indulasiIdoIndex];
            *uj->erkezesiIdo = uj->cel->ido2[indulasiIdoIndex];
        }
        uj->kovetkezo = NULL;
        if (tripPlan == NULL) {
            tripPlan = uj;
        } else {
            struct Utvonalterv* mozgo = tripPlan;
            while (mozgo->kovetkezo != NULL) {
                mozgo = mozgo->kovetkezo;
            }
            mozgo->kovetkezo = uj;
        }
        free(megalloTav);
        free(tmpIndulasiIdo);
    }
    free(path);
    return tripPlan;
}

struct Utvonalterv* utvonaltervezes(Metro* metro, char* indulo, char* cel,
                                    Idopont indulasiIdo) {
    if (indulo == NULL || cel == NULL) {
        return NULL;
    }
    if (strcmp(indulo, cel) == 0) {
        return NULL;
    }
    int vonalakSzama = 0;
    int fixStations = 0;
    struct MetroGraph* metroGraph = createGraph(0);
    Vonal* mozgo = metro->vonalak;
    AtszallasiMegallo** atszallasiMegallok = (AtszallasiMegallo**)malloc(
        sizeof(AtszallasiMegallo) * vonalakSzama);  // array of pointers to
                                                    // atszallasi megallok
    while (mozgo != NULL) {
        AtszallasiMegallo* uj = atszallasi_megallok_on_vonal(metro, mozgo);
        if (uj == NULL) {
            printf("Nem sikerult atszallasi megallokat szerezni!\n");
            return NULL;
        }
        atszallasiMegallok = (AtszallasiMegallo**)realloc(
            atszallasiMegallok, sizeof(AtszallasiMegallo) * (++vonalakSzama));
        atszallasiMegallok[vonalakSzama - 1] = uj;
        mozgo = mozgo->kovetkezo;
    }
    for (int i = 0; i < vonalakSzama; i++) {
        AtszallasiMegallo* mozgo2 = *(atszallasiMegallok + i);
        while (mozgo2 != NULL) {
            int* allomasVertex =
                get_allomas_vertex_by_name(metroGraph, mozgo2->megallo->nev);
            if (allomasVertex == NULL) {
                resize_metro_graph(metroGraph);
                metroGraph->tomb[metroGraph->allomasVSzam - 1].megallok[0] =
                    mozgo2->megallo;
                metroGraph->tomb[metroGraph->allomasVSzam - 1]
                    .taroltMegallokSzama = 1;
                fixStations++;
            } else {
                metroGraph->tomb[*allomasVertex]
                    .megallok[metroGraph->tomb[*allomasVertex]
                                  .taroltMegallokSzama] = mozgo2->megallo;
                metroGraph->tomb[*allomasVertex].taroltMegallokSzama++;
            }
            mozgo2 = mozgo2->kovetkezo;
            free(allomasVertex);
        }
    }
    int* induloVertex = get_allomas_vertex_by_name(metroGraph, indulo);
    int* celVertex = get_allomas_vertex_by_name(metroGraph, cel);
    bool induloVan = false;
    bool celVan = false;
    if (induloVertex == NULL) {
        induloVan = true;
        resize_metro_graph(metroGraph);
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
        free_vonals(induloVonal);
    }
    if (celVertex == NULL) {
        celVan = true;
        resize_metro_graph(metroGraph);
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
        free_vonals(celVonal);
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
                int* megalloTav =
                    megallo_distance(kozosVonal, induloVonalNev, celVonalNev);
                addEl(metroGraph, i, j, abs(*megalloTav));
                free(megalloTav);
            }
        }
    }
    int source = metroGraph->allomasVSzam - fixStations == 2
                     ? metroGraph->allomasVSzam - 2
                 : induloVan ? metroGraph->allomasVSzam - 1
                             : *induloVertex;
    int destination = metroGraph->allomasVSzam - fixStations == 2
                          ? metroGraph->allomasVSzam - 1
                      : celVan ? metroGraph->allomasVSzam - 1
                               : *celVertex;
    int* distance = dijkstra(metroGraph, source);

    Utvonalterv* utvonalterv = dijkstra_to_utvonalterv(
        metroGraph, distance, source, destination, indulasiIdo, metro);
    free(distance);
    free(induloVertex);
    free(celVertex);
    freeGraph(metroGraph);
    free_atszallasi_megallok(atszallasiMegallok, vonalakSzama);
    return utvonalterv;
}