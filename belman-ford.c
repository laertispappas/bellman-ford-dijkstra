#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define MAX_DISTANCE 65535

void bellman_ford(const graph_t *g, const graph_vertex_t *source);
void relax(graph_vertex_t *u, graph_vertex_t *v);

int main(int argc, char **argv){
  graph_vertex_t *vertices;
	
  graph_t *g = graph_create_blank(6);

	graph_add_name_edge(g, 0, 'S', 1, 'A', 5);
	graph_add_name_edge(g, 0, 'S', 3, 'C', 2);
	graph_add_name_edge(g, 1, 'A', 2, 'B', 1);
	graph_add_name_edge(g, 1, 'A', 3, 'C', 2);
	graph_add_name_edge(g, 2, 'B', 5, 'T', 3);
	graph_add_name_edge(g, 3, 'C', 2, 'B', 2);
	graph_add_name_edge(g, 3, 'C', 4, 'D', 3);
	graph_add_name_edge(g, 4, 'D', 2, 'B', 7);
	graph_add_name_edge(g, 4, 'D', 5, 'T', 10);

	graph_dump(g);	

	vertices = g->vertices;
	
	bellman_ford(g, &vertices[0]);
	graph_free(g);

	return 0;
}



void relax(graph_vertex_t *u, graph_vertex_t *v){

}

void bellman_ford(const graph_t *g, const graph_vertex_t *source) {
	int v, i, u;
	graph_edge_t *edge;
	graph_vertex_t *vertices;
	int number_of_vertices;

	int *distance;
	int *predecessor;

	number_of_vertices = g->n_vertices;
	vertices = g->vertices;
	distance = malloc(number_of_vertices * sizeof(int));
	predecessor = malloc(number_of_vertices * sizeof(int));

	// Step 1: initialize graph
	for(v = 0; v < number_of_vertices; v++){
		if (&vertices[v] == source){
			distance[v] = 0;
			predecessor[v] = 0;
		} else {
			distance[v] = MAX_DISTANCE;
			predecessor[v] = -1;
		}
	}
	
	// Step 2: relax edges repeatedly
	for(i = 1, u=0; i <= number_of_vertices - 1; ++i, u = i - 1) {
    	edge = vertices[u].head_edge;
		while(edge) {
			v = edge->target;
			//relax
			if (distance[v] > distance[u] + edge->dist){
				distance[v] = distance[u] + edge->dist;
				predecessor[v] = u;
			}
			edge = edge->next;
		}
	}
	// Step 3: check for negative-weight cycles
	for(i = 1, u = 0; i <= number_of_vertices - 1; ++i, u = i -1){
		edge = vertices[u].head_edge;
			while(edge) {
				v = edge->target;
				if (distance[v] > distance[u] + edge->dist){
					printf("Graph contains a negative-weight cycle u=%d\tv=%d\n", u, v);
				}
				edge = edge->next;
			}
	}

	for(i = 0; i < number_of_vertices; i++){
		printf("distance[%c | %d]=%d \t predecessor[%c | %d]=%d\n", vertices[i].name, i, distance[i], vertices[i].name, i, predecessor[i]);
	}


}
