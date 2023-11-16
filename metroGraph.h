
#ifndef METROGRAPH_H
#define METROGRAPH_H
#include <stdbool.h>

#include "idopontok.h"
#include "utvonalterv.h"
struct El {
    int celAllomas;
    int suly;
    struct El* kov;
};

struct AllomasVertex {
    int taroltMegallokSzama;
    struct Megallo** megallok;
    struct El* elek;
};

struct MetroGraph {
    int allomasVSzam;
    struct AllomasVertex* tomb;
};

int* dijkstra(struct MetroGraph* graph, int source);
int minDistance(int* distance, bool* visited, int V);
struct MetroGraph* createGraph(int numVertices);
void addEdge(struct MetroGraph* graph, int src, int dest, int weight);
void printGraph(struct MetroGraph* graph);
// void freeGraph(struct MetroGraph* graph);
int* get_allomas_vertex_by_name(struct MetroGraph* graph, char* name);
void resize_metro_graph(struct MetroGraph* graph);
int findPrevious(int* distance, struct El* edges);
void printPath(struct MetroGraph* graph, int* distance, int source,
               int destination);
void printSolution(struct MetroGraph* graph, int* distance, int source,
                   int destination);
Utvonalterv* dijkstra_to_utvonalterv(struct MetroGraph* graph, int* distance,
                                     int veg, int start, Idopont indulasiIdo,
                                     Metro* metro);
Utvonalterv* utvonaltervezes(Metro* metro, char* indulo, char* cel,
                             Idopont indulasiIdo);

#endif