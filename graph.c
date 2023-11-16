#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define V 5  // Number of vertices

struct Edge {
    int destination;
    int weight;
    struct Edge* next;
};

struct Vertex {
    int data;
    int distance;  // Distance from the source vertex
    int visited;   // Flag to track whether the vertex has been visited
    struct Edge* edges;
};

struct Graph {
    int numVertices;
    struct Vertex* array;
};

struct MinHeapNode {
    int vertex;
    int distance;
};

struct MinHeap {
    int size;
    int capacity;
    int* position;
    struct MinHeapNode** array;
};

// Function to create a new graph with a given number of vertices
struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    graph->array = (struct Vertex*)malloc(numVertices * sizeof(struct Vertex));

    for (int i = 0; i < numVertices; ++i) {
        graph->array[i].data =
            i;  // You can use any data type or structure for the vertex data
        graph->array[i].edges =
            NULL;  // Initialize the edge list for each vertex
    }

    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    newEdge->destination = dest;
    newEdge->weight = weight;
    newEdge->next = graph->array[src].edges;
    graph->array[src].edges = newEdge;

    // For undirected graphs, you may also want to add the reverse edge
    // newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    // newEdge->destination = src;
    // newEdge->weight = weight;
    // newEdge->next = graph->array[dest].edges;
    // graph->array[dest].edges = newEdge;
}

// Function to print the graph
void printGraph(struct Graph* graph) {
    for (int i = 0; i < graph->numVertices; ++i) {
        struct Edge* currentEdge = graph->array[i].edges;
        printf("Vertex %d:", i);
        while (currentEdge != NULL) {
            printf(" -> %d (%d)", currentEdge->destination,
                   currentEdge->weight);
            currentEdge = currentEdge->next;
        }
        printf("\n");
    }
}

// Function to free the allocated memory for the graph
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->numVertices; ++i) {
        struct Edge* currentEdge = graph->array[i].edges;
        while (currentEdge != NULL) {
            struct Edge* nextEdge = currentEdge->next;
            free(currentEdge);
            currentEdge = nextEdge;
        }
    }

    free(graph->array);
    free(graph);
}

struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->position = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
        (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));

    return minHeap;
}

void swapMinHeapNodes(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->distance < minHeap->array[smallest]->distance)
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->distance < minHeap->array[smallest]->distance)
        smallest = right;

    if (smallest != idx) {
        struct MinHeapNode* smallestNode = minHeap->array[smallest];
        struct MinHeapNode* idxNode = minHeap->array[idx];

        minHeap->position[smallestNode->vertex] = idx;
        minHeap->position[idxNode->vertex] = smallest;

        swapMinHeapNodes(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap) { return minHeap->size == 0; }

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (isEmpty(minHeap)) return NULL;

    struct MinHeapNode* root = minHeap->array[0];

    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->position[root->vertex] = minHeap->size - 1;
    minHeap->position[lastNode->vertex] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(struct MinHeap* minHeap, int vertex, int distance) {
    int i = minHeap->position[vertex];
    minHeap->array[i]->distance = distance;

    while (i && minHeap->array[i]->distance <
                    minHeap->array[(i - 1) / 2]->distance) {
        minHeap->position[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->position[minHeap->array[(i - 1) / 2]->vertex] = i;
        swapMinHeapNodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

void printSolution(int distances[], int n) {
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i) printf("%d \t\t %d\n", i, distances[i]);
}

void dijkstra(struct Graph* graph, int source) {
    int V = graph->numVertices;
    int distances[V];  // Array to store the shortest distances from the source
                       // vertex
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize distances and heap
    for (int v = 0; v < V; ++v) {
        distances[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, distances[v]);
        minHeap->position[v] = v;
    }

    // Set distance to source vertex to 0
    minHeap->array[source] = newMinHeapNode(source, distances[source]);
    minHeap->position[source] = source;
    distances[source] = 0;
    decreaseKey(minHeap, source, distances[source]);

    minHeap->size = V;

    while (!isEmpty(minHeap)) {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->vertex;

        struct Edge* currentEdge = graph->array[u].edges;
        while (currentEdge != NULL) {
            int v = currentEdge->destination;

            if (!minHeap->position[v] || distances[u] == INT_MAX) continue;

            int newDistance = distances[u] + currentEdge->weight;

            if (newDistance < distances[v]) {
                distances[v] = newDistance;
                decreaseKey(minHeap, v, distances[v]);
            }

            currentEdge = currentEdge->next;
        }
    }

    // Print the calculated distances
    printSolution(distances, V);
}

int main() {
    int numVertices = 5;
    struct Graph* graph = createGraph(numVertices);

    addEdge(graph, 0, 1, 2);
    addEdge(graph, 0, 2, 4);
    addEdge(graph, 1, 2, 1);
    addEdge(graph, 1, 3, 7);
    addEdge(graph, 2, 4, 3);
    addEdge(graph, 3, 4, 1);

    int source = 0;
    dijkstra(graph, source);

    // Free allocated memory
    freeGraph(graph);

    return 0;
}
