#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "common.h"

int main(int argc, char **argv)
{
    printf("Test Dijkstra\n");
    Graph *g = graph_init(5);
    graph_add(g, 0, 1, 2);
    graph_add(g, 1, 2, 4);
    graph_add(g, 1, 3, 1);
    graph_add(g, 2, 4, 3);
    graph_add(g, 4, 3, 2);

    dijkstra_run(g, 0, 4);
    
    graph_close(g);
}
