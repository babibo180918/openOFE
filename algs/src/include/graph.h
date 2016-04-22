/**
 * Weighed graph interface
 */
#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct _graph Graph;

/**
 * Init and return a pointer to the graph
 * @param n Number of nodes in the graph 
 * @return Pointer to graph
 */
Graph *graph_init(int n);

/**
 * Return size of the graph
 * @param g Graph pointer
 * @return number of nodes
 */
int graph_size(Graph *g);

/**
 * Close graph and free the resources
 * @param g Graph pointer
 * @return void
 */
void graph_close(Graph *g);

/**
 * Add an edge v -> w to the graph
 * @input v Node v index
 * @input w Node w index
 * @input weight Weight of edge
 */
void graph_add(Graph *g, int v, int w, int weight);

#endif /* __GRAPH_H__ */
