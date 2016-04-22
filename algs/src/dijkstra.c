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

/**
 *  Dijkstra shortest path searching
 *  @input g : directed graph
 *  @input snode Source node
 *  @input dnode Destination node
 *  @return void
 */
void dijkstra_search(Graph *g, int snode, int dnode)
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
        if (i == snode) {
            dist[i] = 0;
        }
        else {
            dist[i] = INT_MAX;
        }
    }
    //processed nodes, if node i is processed, pc[i] will be 1, else pc[i] will be 0. node i is processed means shortest path from snode to i has been found and the distance is dist[i]
    //Initially, pc[i] = 0 for each i # snode (only snode is considered being processed)
    int *pc = (int *)calloc(n, sizeof(int));
    pc[snode] = 1;

    //Start the loops to search for shortest path. For each loop, a new node i that minimizes the value (dist[v] + distance[v,i] ) (v is a processed node) will be added to the processed. The shortest path from snode to i is found

}
