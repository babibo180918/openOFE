#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

struct _graph {
    int size;       //Size of graph
    int **matrix;   //Adjacent matrix
};

typedef struct _node {
    int id;
    struct _node *next;
} Node;

struct _graph_it {
    Node *list;
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

GraphIterator *graph_get_adjacencies(Graph *g, int v)
{
    GraphIterator *nodeIt = (GraphIterator *)malloc(sizeof(GraphIterator));
    nodeIt->list = NULL;
    int i;
    for (i = 0; i < g->size; i++) {
        if (g->matrix[v][i] > 0) {
            Node *newnode = (Node *)malloc(sizeof(GraphIterator));
            newnode->id = i;
            newnode->next = nodeIt->list;
            nodeIt->list = newnode;
        }
    }
    return nodeIt;
}

int graph_next(GraphIterator *it)
{
    int ret = -1;
    if (it->list) {
        Node *node = it->list;
        it->list = it->list->next;
        ret = node->id;
        free(node);
    }
    return ret;
}

void graph_freeit(GraphIterator *it)
{
    while (it->list) {
        Node *node = it->list;
        it->list = it->list->next;
        free(node);
    }
    free(it);
}

int graph_weight(Graph *g, int u, int v)
{
    return g->matrix[u][v];
}
