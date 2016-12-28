								
								README TRABALHO 1
	
	->struct grafo
		Nesta struct foram criados dois apontadores: o *headVertex que irá apontar para o primeiro nó da lista de nós do grafo, e o *list
		que será utilizado na movimentação entre os nós da lista do grafo.
		
	->struct vertice
		Nesta struct foram criados tres apontadores: o *value que irá conter o valer armazenado no vértice, o *neighNode que irá apontar
		somente para o primeiro nó da vizinhança do vértice e o *list que será utilizado na movimentação entre os nós da lista de vizinhança
		do vértice.
		
	->struct no
		Nessa struct foram criados dois apontadores: o *next que irá apontar para o próximo nó e o *vertex que irá apontar para o vértice
		do grafo ao qual será passado ao nó.
		
	->struct lista
		Nessa struct foi criado apenas um ponteiro que será utilizado para manipular uma lista.
	
	->void put_edge()
		Essa função foi acrescentada para que a função poe_aresta ficasse mais simplificada, pois senão seria necessário escrever duas 			vezes o código presente em put_edge() na função poe_aresta() para que fosse possível adicionar um nó, na lista de vizinahnça, em ambos os 			vértices. Nessa função também é realizada a checagem de arestas já existentes no grafo.
		É necessário adicionar no grafo.h (para que não haja warning na compilação) a seguinte linha de código:
			
			void put_edge(vertice u, vertice v);
