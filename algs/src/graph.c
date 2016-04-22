#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

struct _graph {
    int size;       //Size of graph
    int **matrix;   //Adjacent matrix
};

Graph *graph_init(int n)
{
    if (n <= 0) return NULL;
    int i;
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->size = n;
    g->matrix = (int **)malloc(sizeof(int *) * n);
    g->matrix[0] = (int *)calloc(n * n, sizeof(int));
    for (i = 1; i < n; i++) {
        g->matrix[i] = g->matrix[0] + n * i;
    }
    return g;
}

int graph_size(Graph *g)
{
    return g->size;
}

void graph_close(Graph *g)
{
    free(g->matrix[0]);
    free(g->matrix);
}

void graph_add(Graph *g, int v, int w, int weight)
{
    g->matrix[v][w] = weight;
}
