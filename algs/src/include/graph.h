/**
 * Weighed graph interface
 */
#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct _graph Graph;
typedef struct _graph_it GraphIterator;

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
 * @input g Graph Poiter
 * @input v Node v index
 * @input w Node w index
 * @input weight Weight of edge
 */
void graph_add(Graph *g, int v, int w, int weight);

/**
 * Return an iteration for all nodes that connect with node v
 * @input g Graph pointer
 * @input v Node v
 * @return and iterator
 */
GraphIterator *graph_get_adjacencies(Graph *g, int v);

/**
 * Iterating throught the adjacency list
 * @input it Pointer to Node Iteration
 * @return -1 if not found, others id of the node
 */
int graph_next(GraphIterator *it);

/**
 * Free the iterator
 * @input it Iterator pointer
 * @return void
 */
void graph_freeit(GraphIterator *it);

/**
 * Get the weight of edges (u, v) if exists
 * @input g Graph pointer
 * @input u Node u
 * @input v Node v
 * @return weight of edge(u, v) if exists, return 0 if not exist
 */
int graph_weight(Graph *g, int u, int v);
#endif /* __GRAPH_H__ */
