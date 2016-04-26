#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

int main(int argc, char **argv)
{
    Graph *g = graph_init(5);

    //Test graph_size()
    int size = graph_size(g);
    assert(size == 5);

    //Test graph_get_adjacencies and graph_next
    graph_add(g, 0, 1, 2);
    graph_add(g, 1, 2, 4);
    graph_add(g, 1, 3, 1);
    graph_add(g, 2, 4, 3);
    graph_add(g, 4, 3, 2);

    GraphIterator *it = graph_get_adjacencies(g, 0);
    int adj_cnt = 0;
    int node;
    while ((node = graph_next(it)) > 0) {
        adj_cnt++;
    }
    assert(adj_cnt == 1);
    graph_freeit(it);

    it = graph_get_adjacencies(g, 1);
    adj_cnt = 0;
    while ((node = graph_next(it)) > 0) {
        adj_cnt++;
    }
    assert(adj_cnt ==  2);
    graph_freeit(it);
    
    it = graph_get_adjacencies(g, 3);
    adj_cnt = 0;
    while ((node = graph_next(it)) > 0) {
        adj_cnt++;
    }
    assert(adj_cnt ==  0);
    graph_freeit(it);

    //Test graph_weight()
    int weight;
    weight = graph_weight(g, 0, 1);
    assert(weight == 2);
    weight = graph_weight(g, 1, 0);
    assert(weight == 0);
    
    graph_close(g);
}
