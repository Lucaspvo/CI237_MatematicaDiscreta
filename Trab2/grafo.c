#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "grafo.h"

const unsigned int max_nome = 1024;
const int infinito = INT_MAX;

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
	int type;

} *lista;
//------------------------------------------------------------------------------
typedef struct no {

	void *object;
	struct no *next;

} *no;

//------------------------------------------------------------------------------
typedef struct arco {

	struct vertice *vertex;
	int peso;
	
} *arco;

//------------------------------------------------------------------------------
typedef struct passeio {

	struct lista *list;
	struct no *origin;
	struct no *destiny;
	int peso;

} *passeio;

//==============================================================================
//New Functions
vertice acha_vertice(grafo g, char *s, int acrescenta);
grafo poe_arco(grafo g, vertice v, vertice u, int p);
no cria_caminho(char *s);
no cria_destino(vertice v);
int compara(vertice v, lista l);
void shortest_path(unsigned int num_vert, int **matriz, int **mtr, grafo g);

//==============================================================================
void *objeto(no n){
	return n->object;
}

//------------------------------------------------------------------------------
//Return the first node of the list, or NULL if there isn't any
no primeiro_no(lista l) { 
	if (l)
		return l->node;
	else
		return NULL;
}

//------------------------------------------------------------------------------
//Return the next node of the list, or NULL if there isn't any
no proximo_no(no n) { 
	if (n->next != NULL)
		return n->next;
	else
		return NULL;
}

//------------------------------------------------------------------------------
//Return the value/name of teh vertex
char *nome(vertice v) {
	return v->value;
}

//------------------------------------------------------------------------------
//Create a list
lista cria_lista(void){
	lista list = malloc(sizeof(struct lista));
	list->node = NULL;
	return list;
}

//------------------------------------------------------------------------------
//Return the origin vertex of the path
vertice origem_passeio(passeio c){
	return c->origin->object;
}

//------------------------------------------------------------------------------
//Return the destiny vertex of the path
vertice destino_passeio(passeio c){
	return c->destiny->object;
}

//------------------------------------------------------------------------------
//Return the midlle vertices of the path
lista vertices_passeio(passeio c){
	return c->list;
}

//------------------------------------------------------------------------------
//Return the smallest weight of the path
int peso_passeio(passeio c){
	return c->peso;
}

//------------------------------------------------------------------------------
//Create a path
passeio cria_passeio(vertice v){
	passeio p = malloc(sizeof(struct passeio));
	no node = malloc(sizeof(struct no));
	node->next = NULL;
	node->object = malloc(sizeof(struct vertice));
	((vertice)node->object)->neighNode = NULL;
	((vertice)node->object)->list = NULL;
	((vertice)node->object)->value = malloc(sizeof(char)*(strlen(v->value)+1));
	strcpy(((vertice)node->object)->value, v->value);
	p->list = cria_lista();
	p->list->type = 0;
	p->list->node = node;
	p->origin = node;
	p->destiny = node;
	p->peso = 0;
	return p;
}

//------------------------------------------------------------------------------
//Return the number of vertices in the graph
unsigned int n_vertices(grafo g){
	unsigned int num=0;
	g->list->node = g->headVertex;
	while(g->list->node != NULL){
		num++;
		g->list->node = g->list->node->next;
	}
	return num;
}

//------------------------------------------------------------------------------
//Create a graph
grafo cria_grafo(void) {
	grafo graph = malloc(sizeof(struct grafo));
	graph->headVertex = NULL;
	graph->list = cria_lista();
	graph->list->type = 0;
	graph->list->node = NULL;
	if (graph != NULL)
		return graph;
	else
		return NULL;
}

//------------------------------------------------------------------------------
//Destroy a list
int destroi_lista(lista l){
	//If it is a list of arcs, then it's type it will be 1
	if (l->type == 1){
		lista list = malloc(sizeof(struct lista));
		list->node = l->node;
		if(list->node == NULL){
			return 1;
		}
		while(list->node->next != NULL){
			l->node = list->node;
			free(l->node->object);
			list->node = list->node->next;
		}
		free(list->node->object);
		free(list);
		return 1;
	}
	//If it is a list of vertices, then it's type it will be 0
	else if (l->type == 0){
		lista list = malloc(sizeof(struct lista));
		list->node = l->node;
		while(list->node->next != NULL){
			l->node = list->node;
			free(((vertice)l->node->object)->value);
			free(l->node->object);
			list->node = list->node->next;
		}
		free(list);
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//Destroy a path
int destroi_passeio(passeio c){
	if(!c)
		return 0;
	destroi_lista(c->list);
	return 1;
}

//------------------------------------------------------------------------------
//Destroy the graph
int destroi_grafo(grafo g) {
	g->list->node = g->headVertex;
	//Check if the graph is NULL
	if(g->headVertex == NULL)
		return 0;
	//If it is not, delete every node and vertex created
	while(g->list->node != NULL){
		destroi_lista(((vertice)g->list->node->object)->list);
		g->list->node = g->list->node->next;
		
	}
	g->list->node = g->headVertex;
	destroi_lista(g->list);
	free(g);
	return 1;
}

//------------------------------------------------------------------------------
//Put the new vertices in the graph
vertice acha_vertice(grafo g, char *s, int acrescenta) {
		g->list->node = g->headVertex;

		/*Check if the graph it is empty, and if it is, I create the first node that will point to the first vertex*/
		if(g->headVertex == NULL && acrescenta != 0){
			no node = malloc(sizeof(struct no));
			node->object = malloc(sizeof(struct vertice));
			((vertice)node->object)->value = malloc(sizeof(char)*(strlen(s)+1));
			strcpy(((vertice)node->object)->value, s);
			((vertice)node->object)->neighNode = NULL;
			((vertice)node->object)->list = cria_lista();
			((vertice)node->object)->list->type = 1;
			((vertice)node->object)->list->node = NULL;
			node->next = NULL;
			g->headVertex = node;
			g->list->node = node;
			return node->object;
		}
		/*Check until the last node of the list to see if the vertex already exists*/
		while(g->list->node->next != NULL){
			if(strcmp(((vertice)g->list->node->object)->value, s) == 0){
				return g->list->node->object;
			}
			g->list->node = g->list->node->next;
		}
		
		/*Here I check the last one*/
		if(strcmp(((vertice)g->list->node->object)->value, s) == 0){
			return g->list->node->object;
		}
		/*If the vertex doesn't exist, I create a new one with the value in 's'*/
		else if(acrescenta != 0){

			no node = malloc(sizeof(struct no));
			node->object = malloc(sizeof(struct vertice));
			((vertice)node->object)->value = malloc(sizeof(char)*(strlen(s)+1));
			strcpy(((vertice)node->object)->value, s);
			((vertice)node->object)->neighNode = NULL;
			((vertice)node->object)->list = cria_lista();
			((vertice)node->object)->list->type = 1;
			((vertice)node->object)->list->node = NULL;
			node->next = NULL;
			g->list->node->next = node;
			return node->object;
		}
	return NULL;
}

//------------------------------------------------------------------------------
//Create an arc that has u as an origin vertex and v as a destiny vertex
grafo poe_arco(grafo g, vertice v, vertice u, int p) {

	u->list->node = u->neighNode;

	arco arc;
	no node;
	
	if(u->neighNode == NULL){
		arc = malloc(sizeof(struct arco));
		node = malloc(sizeof(struct no));
		arc->peso = p;
		arc->vertex = v;
		node->next = NULL;
		node->object = arc;
		u->neighNode = node;
		u->list->node = node;
	}
	
	else{
		while(u->list->node->next != NULL)
			u->list->node = u->list->node->next;
		arc = malloc(sizeof(struct arco));
		node = malloc(sizeof(struct no));
		arc->peso = p;
		arc->vertex = v;
		node->next = NULL;
		node->object = arc;
		u->list->node->next = node;
	}
	
	return g;
}

//------------------------------------------------------------------------------
//Read the input data and create a graph with this data
grafo le_grafo(FILE *input){
	grafo g = cria_grafo();
  
	if ( ! g )
		return NULL;
		

	char u[max_nome], v[max_nome], peso[max_nome];
	while ( fscanf(input, "%s %s", u, v) == 2 ){
		if (strcmp(v,"-")==0) {
			acha_vertice(g,u,1);
		}
		else {
			fscanf(input, "%s", peso);
			int pesoi = atoi(peso);
			if ( ! poe_arco(g, acha_vertice(g,v,1), acha_vertice(g,u,1), pesoi) ) {
				return NULL;
			}
		}
	}
	
	return g;
}

//------------------------------------------------------------------------------
//Create a new node pointing to a new vertex and return this node
no cria_caminho(char *s){
	no node = malloc(sizeof(struct no));
	vertice vert = malloc(sizeof(struct vertice));
	vert->neighNode = NULL;
	vert->list = cria_lista();
	vert->list->type = 0;
	vert->list->node = NULL;
	vert->value = malloc(sizeof(char)*max_nome);
	strcpy(vert->value, s);
	node->next = NULL;
	node->object = vert;
	return node;
}

//------------------------------------------------------------------------------
//Create a new node that points to a new vertex and it is copied the vertex->value from the parameter to the new node
no cria_destino(vertice v){
	no node = malloc(sizeof(struct no));
	vertice vert = malloc(sizeof(struct vertice));
	vert->neighNode = NULL;
	vert->list = cria_lista();
	vert->list->type = 0;
	vert->list->node = NULL;
	vert->value = malloc(sizeof(char)*max_nome);
	strcpy(vert->value, v->value);
	node->next = NULL;
	node->object = vert;
	return node;
}

//------------------------------------------------------------------------------
//Compare to see if there is an arc with the same value present in one of the vertex's graph
int compara(vertice v, lista l){
		while(l->node != NULL){
			if(strcmp(((arco)l->node->object)->vertex->value, v->value) == 0)
				return 0;
			l->node = l->node->next;
		}
	return 1;
}

//------------------------------------------------------------------------------
//Create two matrix and at the end:
//-One will save the smallest weight from one vertex to another, and
//-The other will save the vertices that integrate the path from one vertex to another
void shortest_path(unsigned int num_vert, int **matriz, int **mtr, grafo g){

	unsigned int i, j;
	vertice vertx;
	no node = g->headVertex;
	
	for(i=0; i<num_vert; i++){
		vertx = node->object;
		vertx->list->node = vertx->neighNode;
		g->list->node = g->headVertex;
		for (j=0; j<num_vert; j++){
			if(vertx->list->node != NULL){
				if(compara(g->list->node->object, vertx->list) == 0){
					matriz[i][j] = ((arco)vertx->list->node->object)->peso;
					mtr[i][j] = (int)j+1;
				}
				else{
					matriz[i][j] = infinito;
					mtr[i][j] = 0;
				}
			}
			else{
				matriz[i][j] = infinito;
				mtr[i][j] = 0;
			}
			vertx->list->node = vertx->neighNode;
			g->list->node = g->list->node->next;
		}
		
		node = node->next;
	}

		
}

//------------------------------------------------------------------------------
//Create a vector of paths that in each position of this vector is malloced a struct passeio and each one of those structs
//contain the way that costs less between two vertices in the graph
passeio *todos_caminhos_minimos(grafo g){
	unsigned int num_vert = n_vertices(g);
	unsigned int i, j, k;
	int aux;
	int **matriz, **mtr;
	vertice vertx;
	passeio *vetor;

	if ( ! g )
		return NULL;

	matriz = (int **)malloc(num_vert * sizeof(int *));
  	for (i = 0; i < num_vert; ++i)
      		matriz[i] = (int *)malloc(num_vert * sizeof(int)); 

    mtr = (int **)malloc(num_vert * sizeof(int *));
  	for (i = 0; i < num_vert; ++i)
      		mtr[i] = (int *)malloc(num_vert * sizeof(int));

	vetor = malloc(sizeof(passeio) * (num_vert*num_vert));
	
	for(i=0;i<num_vert*num_vert;i++) {
		vetor[i] = malloc(sizeof(struct passeio));
	}
	
	no node = g->headVertex;
	
	char **indice = (char **)malloc(sizeof(char*)*num_vert);
	for(i=1;i<=num_vert;i++) {
		indice[i] = (char*) malloc (sizeof(char)*max_nome);
		strcpy(indice[i], ((vertice)node->object)->value);
		node = node->next;
	}
	
	shortest_path(num_vert, matriz, mtr, g);
	
	//Find out the smallest weight and the path responsible for this weight
	for(k=0; k<num_vert; k++){
		for(i=0; i<num_vert; i++)
			for(j=0; j<num_vert; j++){
				if ((matriz[i][k]+matriz[k][j] < matriz[i][j]) && (matriz[i][k]!=infinito && matriz[k][j]!=infinito)){
					matriz[i][j] = matriz[i][k]+matriz[k][j];
					mtr[i][j] = mtr[i][k];
				}
			}			
	}
	
	//Create the vector containing the smallest weight, the origin and destiny vertices and the path between those two vertices
	node = g->headVertex;		
	for(i=0; i<num_vert; i++){
		vertx = node->object;
		g->list->node = g->headVertex;
		for(j=0; j<num_vert; j++){
			vetor[i*num_vert+j] = cria_passeio(vertx);
			aux = (int)i;
			if (mtr[aux][j] != 0){
				while(mtr[aux][j] != (int)j+1){
					vetor[i*num_vert+j]->destiny->next = cria_caminho(indice[mtr[aux][j]]);
					vetor[i*num_vert+j]->destiny = vetor[i*num_vert+j]->destiny->next;
					aux = mtr[aux][j]-1;
				}
				vetor[i*num_vert+j]->destiny->next = cria_destino(g->list->node->object);
				vetor[i*num_vert+j]->destiny = vetor[i*num_vert+j]->destiny->next;
				vetor[i*num_vert+j]->peso = matriz[i][j];
			}
			else{
				vetor[i*num_vert+j]->destiny->next = cria_destino(g->list->node->object);
				vetor[i*num_vert+j]->destiny = vetor[i*num_vert+j]->destiny->next;
				vetor[i*num_vert+j]->peso = matriz[i][j];
			}
			g->list->node = g->list->node->next;
		}
		node = node->next;
	}		
	return vetor;
}	
