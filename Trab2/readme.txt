


								README - Lucas Pazelo Vargas de Oliveira - GRR20113495
								
			Neste trabalho foram criadas as seguintes novas funções:
			
			1 - vertice acha_vertice(grafo g, char *s, int acrescenta) - função responsável por criar um novo vértice
				e acrescentá-lo a struct grafo;
				
			2 - grafo poe_arco(grafo g, vertice v, vertice u, int p) - função responsável por criar uma struct arco
				e ligar o vertice de origem a esse novo arco, assim como ligar a struct arco ao vértice de destino;
				
			3 - no cria_caminho(char *s) - função responsável por criar uma nova struct no e uma nova struct vertice, que 
				receberá o valor passado em 's', ligando o no ao novo vértice e retornando o no;
				
			4 - no cria_destino(vertice v) - função responsável por criar o último vértice (vértice de destino) de cada posição
				do vetor de passeios;
				
			5 - int compara(vertice v, lista l) - função responsável por comparar se um determinado vértice do grafo esta
				sendo conectado por um arco presente na lista de arcos de outro vértice do grafo;
				
			6 - void shortest_path(unsigned int num_vert, int **matriz, int **mtr, grafo g) - função responsável por criar duas
				matrizes, uma responsável por guardar os pesos minimos entre dois vértices e a outra responsável por guardar
				o caminho percorrido para ir de u a v.
				
			Obs: O seguinte warning esta ocorrendo - warning: padding struct size to alignment boundary - mas não consegui 
					determinar o motivo pelo qual isso esta acontecendo.
				
