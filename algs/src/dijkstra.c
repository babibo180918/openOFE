/**
 * DIJKSTRA : SHORTEST PATH SEARCHING
 *
 * Input  : Graph, Source Node, Dest Node
 * Output : Shortest path in node linked list
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "dijkstra.h"

typedef struct _djnode {
    int from;
    int to;
    int weight;
    struct _djnode *next;
} Djnode;

/**
 *  Dijkstra shortest path searching
 *  @input g : directed graph
 *  @input snode Source node
 *  @input dnode Destination node
 *  @return void
 *  algorithm : 
 *  V is sets of all nodes
 *  X is sets of node that is every node v in X, shortest(snode, v) is calculated
 *  Initialy, snode is added to X
 *  Start the loop, in each loop, selects a node v in V-X that minimize the value dist(u, v) + shortest(u) for all u in X
 *  Move v to X
 *  Repeat until all nodes has been move to X
 */
void dijkstra_run(Graph *g, int snode, int dnode)
{
    int n = graph_size(g);
    int i;
    //Array used for tracking back the shortes path from snode to each node
    int *parentOf = (int *)malloc(sizeof(int) * n);
    for (i = 0; i < n; i++) {
        parentOf[i] = -1;
    }
    //dist[i] is the shortest distance from snode to node i
    int *dist = (int *)malloc(sizeof(int) * n);
    for (i = 0; i < n; i++) {
        dist[i] = INT_MAX;
    }
    //processed nodes, if node i is processed, pc[i] will be 1, else pc[i] will be 0. node i is processed means shortest path from snode to i has been found and the distance is dist[i]
    //Initially, pc[i] = 0 for each i # snode (only snode is considered being processed)
    int *pc = (int *)calloc(n, sizeof(int));

    Djnode *cand_edges = NULL;      //Contains all candidate edges (u, v) that u in X and v in V-X
    //First, add snode to X and update cand_edges list
    dist[snode] = 0;
    pc[snode] = 1;
    GraphIterator *it = graph_get_adjacencies(g, snode);
    int v;
    while ((v = graph_next(it)) != -1) {
        if (!pc[v]) {
            Djnode *dj = (Djnode *)malloc(sizeof(Djnode));
            dj->from = snode;
            dj->to = v;
            dj->weight = graph_weight(g, snode, v);
            dj->next = cand_edges;
            cand_edges = dj;
        }
    }
    graph_freeit(it);
    

    //Start the loops to search for shortest path. For each loop, a new node i that minimizes the value (dist[v] + distance[v,i] ) (v is a processed node) will be added to the processed. The shortest path from snode to i is found
    int processed = 1;      //snode is considered as processed
    while (processed < n) {
        //Find the min dijkstra distance
        Djnode *min_edge;
        int min_dist = INT_MAX;
        Djnode *edge = cand_edges;
        while (edge) {
            if (!pc[edge->to] && edge->weight + dist[edge->from] < min_dist) {
                min_edge = edge;
                min_dist = edge->weight + dist[edge->from];
            }
            edge = edge->next;
        }
        printf("min edge %d %d \n", edge->from, edge->to);

        //Add the found min_node to processed list
        dist[min_edge->to] = min_dist;
        pc[min_edge->to] = 1;
        parentOf[min_edge->to] =  min_edge->from;
        GraphIterator *it = graph_get_adjacencies(g, min_edge->to);
        int v;
        while ((v = graph_next(it)) != -1) {
            if (!pc[v]) {
                Djnode *dj = (Djnode *)malloc(sizeof(Djnode));
                dj->from = snode;
                dj->to = v;
                dj->weight = graph_weight(g, min_edge->to, v);
                dj->next = cand_edges;
                cand_edges = dj;
            }
        }
        graph_freeit(it);
        
        processed++;
    }

    //Print the result
    int cur = dnode;
    while (cur != snode) {
        printf("%d ", cur);
        cur = parentOf[cur];
    }
    printf("%d \n", cur);
}

