#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "pqueue/priority_queue.h"
#include "pqueue/constants.h"


#define MAX_DISTANCE 65535

// for priority queue
int comparator( const void *item_1, const void *item_2 )
{
        int value_1, value_2;
	graph_vertex_t *u;
	graph_vertex_t *v;
	
	u = (graph_vertex_t *)item_1;
	v = (graph_vertex_t *)item_2;

	value_1 = u->t_dist;
	value_2 = v->t_dist;

        if( value_1 < value_2 )
        {
                return SMALLER;
        }
        else if( value_1 == value_2 )
        {
                return EQUAL;
        }
        else
        {
                return GREATER;
        }
}

void dijkstra(const graph_t *g, const graph_vertex_t *source);

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
	
	dijkstra(g, &vertices[0]);	

	graph_free(g);
	return 0;
}
void dijkstra(const graph_t *g, const graph_vertex_t *source) {
	int v, i;
	graph_edge_t *edge;
	graph_vertex_t *vertices;
	int number_of_vertices;
	priority_queue *pq = NULL;
	graph_vertex_t *vertec_u;
	int *visited;

	int *predecessor;

	number_of_vertices = g->n_vertices;
	vertices = g->vertices;
	predecessor = malloc(number_of_vertices * sizeof(int));
  	pq = create_priority_queue( 16, &comparator );
  	visited = malloc(number_of_vertices * sizeof(int));	
	
	// Step 1: initialize graph
	for(v = 0; v < number_of_vertices; v++){
		if (&vertices[v] == source){
			vertices[v].t_dist = 0; // test
			predecessor[v] = 0;
			visited[v] = 0;
		} else {
			vertices[v].t_dist = MAX_DISTANCE; // test
			predecessor[v] = -1;
			visited[v] = 0;
		}
        	priority_queue_insert( pq, (void *)&vertices[v] );
	}
	

	// while Q not empty
	while(vertec_u = (graph_vertex_t *)priority_queue_poll(pq)) {
		printf("extracted %c distance=%d \n", vertec_u->name, vertec_u->t_dist);
		edge = vertec_u->head_edge;
		visited[vertec_u->head_edge->reverse->target] = 1;
		while(edge) {
			v = edge->target;
			if(!visited[v]){
				printf("got neighbor %d-%c with d=%d \n", v, vertices[v].name, vertices[v].t_dist);
				// relax
				if (vertices[v].t_dist > vertec_u->t_dist + edge->dist){
					vertices[v].t_dist = vertec_u->t_dist + edge->dist;
					predecessor[v] = vertec_u->head_edge->reverse->target;
					repair_top( pq, 1, pq->heap_size );
					printf("Changed %c distance to %d\n", vertices[v].name, vertices[v].t_dist);
				}

			}
			edge = edge->next;
			// print queue
            printf( "%d [", pq->heap_size );
            for ( i = 1; i <= pq->heap_size; i++ )
            {
                    printf( " %c ", (*(graph_vertex_t *)pq->heap_array[i]).name );
            }
            printf( "]\n" );
		}
	}

	for(i = 0; i < number_of_vertices; i++){
		printf("distance[%c | %d]=%d \t predecessor[%c | %d]=%d\n", vertices[i].name, i, vertices[i].t_dist, vertices[i].name, i, predecessor[i]);
	}




	int *r;
	while(r = (int *)priority_queue_poll(pq)) {
		printf("%d is %d\n",pq->heap_size, *r);
	}

	free_priority_queue( &pq );
	free(predecessor);
	free(visited);
}

