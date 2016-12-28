#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int max_tam = 1024;

//------------------------------------------------------------------------------
typedef struct grafo {

	struct no *headVertex;
	struct lista *list;

} *grafo;
//------------------------------------------------------------------------------
typedef struct vertice {

	char *value;
	struct no *neighNode;
	struct lista *list;

} *vertice;
//------------------------------------------------------------------------------
typedef struct lista {
  
	struct no *node;

} *lista;
//------------------------------------------------------------------------------
typedef struct no {

	struct no *next;
	struct vertice *vertex;

} *no;
//=============================================================================
no primeiro_no(lista l) { 
	if (l)
		return l->node;
	else
		return NULL;
}
//------------------------------------------------------------------------------
no proximo_no(no n) { 
	if (n->next != NULL)
		return n->next;
	else
		return NULL;
}
//------------------------------------------------------------------------------
vertice vert(no n) { 
	return n->vertex;
}

//------------------------------------------------------------------------------
char *nome(vertice v) {
	return v->value;
}
//------------------------------------------------------------------------------
lista vizinhanca(vertice v) {
	/*before returning the list, I point the list pointer to the first node of the vertex neighbourhood*/
	v->list->node = v->neighNode; 
	return v->list;
}
//------------------------------------------------------------------------------
grafo cria_grafo(void) {
	grafo graph = malloc(sizeof(grafo));
	graph->headVertex = NULL;
	graph->list = malloc(sizeof(lista));
	graph->list->node = NULL;
	if (graph != NULL)
		return graph;
	else
		return NULL;
}
//------------------------------------------------------------------------------
int destroi_grafo(grafo g) {
	
	/*Check if the graph is NULL*/
	if(g == NULL)
		return 0;
	/*If it is not, delete every node and vertex created*/
	while(g->list->node != NULL){
		vertice vertx = g->list->node->vertex;
		while(vertx->list->node != NULL){
			vertx->neighNode = vertx->list->node->next;
			free(vertx->list->node);
			vertx->list->node = vertx->neighNode;
		}
		free(vertx);
		g->headVertex = g->list->node->next;
		free(g->list->node);
		g->list->node = g->headVertex;
	}
	free(g);
	return 1;
}
//------------------------------------------------------------------------------
lista vertices(grafo g) { 
	/*before returning the list, I point the list pointer to the first node of the graph*/
	g->list->node = g->headVertex;
	return g->list;
}
//------------------------------------------------------------------------------
vertice acha_vertice(grafo g, char *s, int acrescenta) {
	g->list->node = g->headVertex;
	/*Check if the graph it is empty, and if it is, I create the first node that will point to the first vertex*/
	if(g->headVertex == NULL && acrescenta != 0){
		no node = malloc(sizeof(no));
		node->vertex = malloc(sizeof(vertice));
		node->vertex->value = malloc(max_tam*sizeof(char));
		strcpy(node->vertex->value, s);
		node->vertex->neighNode = NULL;
		node->vertex->list = malloc(sizeof(lista));
		node->vertex->list->node = NULL;
		node->next = NULL;
		g->headVertex = node;
		g->list->node = node;
		return node->vertex;
	}
	/*Check until the last node of the list to see if the vertex already exists*/
	while(g->list->node->next != NULL){
		if(strcmp(g->list->node->vertex->value, s) == 0){
			return g->list->node->vertex;
		}
		g->list->node = g->list->node->next;
	}
	/*Here I check the last one*/
	if(strcmp(g->list->node->vertex->value, s) == 0){
		return g->list->node->vertex;
	}
	/*If the vertex doesn't exist, I create a new one with the value in 's'*/
	else if(acrescenta != 0){
		no node = malloc(sizeof(no));
		node->vertex = malloc(sizeof(vertice));
		node->vertex->value = malloc(max_tam*sizeof(char));
		strcpy(node->vertex->value, s);
		node->vertex->neighNode = NULL;
		node->vertex->list = malloc(sizeof(lista));
		node->vertex->list->node = NULL;
		node->next = NULL;
		g->list->node->next = node;
		return node->vertex;
	}
	return NULL;
}

//------------------------------------------------------------------------------

void put_edge(vertice u, vertice v){
	u->list->node = u->neighNode;
	v->list->node = v->neighNode;

	no node;
	/*Check if the vertex has no edge*/
	if(u->neighNode == NULL){
		node = malloc(sizeof(no));
		node->next = NULL;
		node->vertex = v;
		u->neighNode = node;
		u->list->node = node;
	}
	/*If it already has neighbour's nodes, check if this edge already exists, if it doesn't I create a new edge*/
	else{
		while(u->list->node->next != NULL && strcmp(u->list->node->vertex->value, v->value) != 0)
			u->list->node = u->list->node->next;
		if(strcmp(u->list->node->vertex->value, v->value) != 0){
			node = malloc(sizeof(no));
			node->next = NULL;
			node->vertex = v;
			u->list->node->next = node;
		}
	}
}

//------------------------------------------------------------------------------
grafo poe_aresta(grafo g, vertice u, vertice v) {
	
	put_edge(u, v);
	put_edge(v, u);
	
	return g;
}
