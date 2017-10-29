/*
  Nome: Matheus Esteves Zanoto   RA: 184256
  Nome: Giovanna Batalha         RA: 197960

  Objetivos:
  Utilizar uma árvore para gerenciar processos e a memória disponível em um S.O.

  Entradas:
  - Inteiro que representa o expoente na base 2 da memória disponível no S.0
  	Por exemplo, ao receber 10, sabemos que o S.O possui a memória 2 ^ 10.
  - O número da operação desejada e os parêmetros da mesma
    1.Inicia Processo - código do processo, tamanho do processo
    2.Finaliza Processo - código do processo
    3.Calcula Fragmentação 
    4.Relatório do sistema
    5.Imprime sementes geradoras
    6.Imprime processos

  Saídas:
  Depende das operações:
  1.Processo (cod​) de tamanho size​ inicializado com sucesso Ou Memoria insuficiente
  2.Processo (cod​) finalizado com sucesso ou Nao existe processo de codigo cod​ inicializado no sistema
  3.Quantidade total de memoria desperdicada por fragmentacao: fragmentacaoCalculada
  4.    [RELATORIO DE SISTEMA]
		ocup​ ​Ocupados
		livre​ ​Livres
		part​ ​Particionados
		Memoria utilizada = mem​ / 100
  5. Exemplo:
  	[SEMENTES GERADORAS]
	Sim =​ ​(L:512)(P:1024)(O:496/512[862])
	Pre = (P:1024)(L:512​ ​)(O:496/512[862])
	Pos = (L:512)(O:496/512[862])(P:1024)

	Um nó Livre​ é representado no formato (L:cap)​.
	Um nó Particionado​ é representado no formato (P:cap)
	Um nó Ocupado​ é representado no formato (O:p/cap[cod])
	cap​ ​indica a capacidade de armazenamento do nó
	p​ indica o tamanho de um processo alocado no nó Ocupado
	cod​ indica o código do processo alocado no nó Ocupado
  6.[PROCESSOS PRESENTES NA MEMORIA]
	Mem_ocupada0​ ​[Processo: cod0​]
	Mem_ocupada1​ [Processo: cod1​]
	…
	Mem_ocupadaN​ [Processo: codN​]

	ou

	[PROCESSOS PRESENTES NA MEMORIA]
	Nenhum processo presente
	
	Obs.: Mem_ocupadaX se refere ao tamanho da partição que o
	processo codX​ está utilizando.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum Estado{
  LIVRE,OCUPADO,PARTICIONADO
} Estado;

typedef struct Processo{
  int codigo;
  int tamanho;
} Processo;

typedef struct Memoria{
  int valor; //Guarda a potencia (exemplo: guarda 10, mas sabemos que é 2^10)
  Estado estado;
  int fragmentacao;
  Processo* processo;
} Memoria;

typedef struct No{
  Memoria memoria;
  struct No* esq;
  struct No* dir;
} No;

typedef struct Arvore{
  No* raiz;
} Arvore;

void inicializarNo(No* no, int valor);
void inicializarArvore(Arvore* arvore, int valor);


int finalizarProcesso(No* no, int cod);
int iniciarProcesso(No* no, Processo processo);

int calcularFragmentacao(No* no); 
void relatorioSistema(Arvore* arvore); 
int* calculaRelatorioSistema(No* no); 
void imprimirNo(No* no);
void imprimirSimetrico(No* no);
void imprimirPreOrdem(No* no);
void imprimirPosOrdem(No* no);
void imprimirSementesGeradoras(Arvore* arvore);
void imprimirProcessos(Arvore* arvore);
int pot(int base, int expoente); 
void imprimeArvore(No* no); 
int imprimeProcessos(No* no);

int main(){
  int potencia;
  scanf("%d",&potencia);

  Arvore arvore;
  inicializarArvore(&arvore,potencia);

  int op;
  while(scanf("%d", &op) != EOF){
    switch(op){
	  case 1:{
	  	Processo processo;
	  	scanf("%d %d",&processo.codigo,&processo.tamanho);
	    int iniciouProcesso = iniciarProcesso(&(*arvore.raiz),processo);
	    if (iniciouProcesso == 0)
	  	  printf("Memoria insuficiente\n");
	  	else
	  	  printf("Processo (%d) de tamanho %d inicializado com sucesso\n",processo.codigo,processo.tamanho);
	  } break;
	  case 2:{
        int cod;
        scanf("%d",&cod);
        int finalizouProcesso = finalizarProcesso(&(*arvore.raiz),cod);
        if (finalizouProcesso == 0)
          printf("Nao existe processo de codigo %d inicializado no sistema\n",cod);
        else
          printf("Processo (%d) finalizado com sucesso\n",cod);
	  } break;
	  case 3:{
	    int fragmentacao = calcularFragmentacao(arvore.raiz);
	  	printf("Quantidade total de memoria desperdicada por fragmentacao: %d\n",fragmentacao);
	  } break;
	  case 4:{
	    relatorioSistema(&arvore);
	  } break;
	  case 5:{
        imprimirSementesGeradoras(&arvore);
	  } break;
	  case 6:{
	  	printf("[PROCESSOS PRESENTES NA MEMORIA]\n");
	  	if(imprimeProcessos(arvore.raiz) == 0){	
	  		printf("Nenhum processo presente\n");
	  	}
	  }break;
	  default:{
	  	imprimeArvore(arvore.raiz);
	  }
    }
  }	
}

//Método auxiliar para ser possível visualizar a estrutura atual.
void imprimeArvore(No* no){
	if(no == NULL)
		return;


	int estado = no->memoria.estado;
	char* estadoString = malloc(sizeof(char*));
	if(estado == 0){
		estadoString = "LIVRE";
	}

	if(estado == 1){
		estadoString = "OCUPADO ()";
	}

	if(estado == 2){
		estadoString = "PARTICIONADO";
	}

	int potencia = pot(2, no->memoria.valor);

	printf("\n%d %s", potencia, estadoString);
	printf("[");

	imprimeArvore(no->esq);
	imprimeArvore(no->dir);

	printf("]");
}

//Imprime os processos, ou seja, ocorre somente se o nó está OCUPADO
int imprimeProcessos(No* no){
	int dir, esq, printouNo;

    printouNo = 0;
	if(no == NULL){
		return 0;
	}

	esq = imprimeProcessos(no->esq);

	if(no->memoria.estado == OCUPADO){
		//printar o atual
		printf("%d [Processo: %d]\n", pot(2, no->memoria.valor), no->memoria.processo->codigo);
		printouNo = 1;
	}

	dir = imprimeProcessos(no->dir);

	if(esq == 0 && dir == 0 && printouNo == 0){
		return 0;
	}

	return 1;
}

//Função que retorna potência
int pot(int base, int expoente){
    int i;
    int resp = 1;

    if(expoente == 0){
    	return resp;
    }

    for (i = 0; i < expoente; i++){
    	resp *= base;	
    }

    return resp;
}

//Função recursiva, recebe inicialmente a raíz da árvore
//Enquanto o tamanho X do processo não segue a regra 2^i-1 < X <= 2^i, particionamos o nó
int iniciarProcesso(No* no, Processo processo){
	int metadeAtual = pot(2, no->memoria.valor-1);
	int valorAtual = pot(2, no->memoria.valor);

	//Adiciona o processo é maior que a metade do tamanho do nó e menor ou igual ao tamanho do nó
	if(processo.tamanho > metadeAtual && processo.tamanho <= valorAtual){
		//Chegou no nó que cabe o processo, mas está ocupado ou particionado, ou seja, não podemos incluir
		if(no->memoria.estado == OCUPADO || no->memoria.estado == PARTICIONADO){
			return 0;
		}

		//Chegou no nó que cabe o processo e adicionou o processo
		if (no->memoria.estado == LIVRE){
		  no->memoria.processo = malloc(sizeof(Processo));
		  *(no->memoria.processo) = processo;
		  no->memoria.fragmentacao = pot(2, no->memoria.valor) - processo.tamanho;
		  no->memoria.estado = OCUPADO;
		  return 1;
		}
	}
	else
	  if (processo.tamanho > valorAtual)
	    return 0;

	//Só particiona se não estiver ocupado
	if(no->memoria.estado == OCUPADO){
		return 0;
	}

	//Se não adicionou, particionamos o nó
	if(no->esq == NULL && no->dir == NULL){
		no->memoria.estado = PARTICIONADO;
	}

	//Criamos o nó da esquerda
	if(no->esq == NULL){
		no->esq = malloc(sizeof(No));
		inicializarNo(&(*no->esq), no->memoria.valor-1);
	}

	//Criamos o nó da direita
	if(no->dir == NULL){
		no->dir = malloc(sizeof(No));
		inicializarNo(&(*no->dir), no->memoria.valor-1);
	}

	//Tentamos adicionar, na partição da esquerda e depois na partição da direita
	int esqLivre = iniciarProcesso(&(*no->esq), processo);
	int dirLivre;

	if(esqLivre == 0){
		dirLivre = iniciarProcesso(&(*no->dir), processo);

		if(dirLivre==0){
			return 0;
		}
	}

	return 1;
}

//Responsável por liberar o nó que possui o processo procurado, e remover as partições que não são mais necessárias
int finalizarProcesso(No* no, int cod){
  //Atualiza o nó que possui o processo para LIVRE
  if (no->memoria.estado == OCUPADO && no->memoria.processo->codigo == cod){
    free(no->memoria.processo);
    no->memoria.fragmentacao = 0;
    no->memoria.estado = LIVRE;
    return 1;
  }
  if (no->memoria.estado == PARTICIONADO){
    int finalizouEsq = finalizarProcesso(&(*no->esq),cod);
    int finalizouDir = finalizarProcesso(&(*no->dir),cod);

    //Se os nós da direita e da esquerda tem o estado LIVRE, devemos removê-los 
    if (no->esq->memoria.estado == LIVRE && no->dir->memoria.estado == LIVRE){
      free(no->esq);
      free(no->dir);
      no->esq = NULL;
      no->dir = NULL;
      no->memoria.estado = LIVRE;
      return 1;
    }
    if (finalizouEsq == 1 || finalizouDir == 1)
      return 1;
  }
  return 0;
}

//Percorre a árvore somando a fragmentação de cada nó que possui um processo, ou seja, dos nós com estado OCUPADO
int calcularFragmentacao(No* raiz){
	if(raiz == NULL){
		return 0;
	}

	int result = raiz->memoria.fragmentacao;
	int fragDir = calcularFragmentacao(raiz->dir);
	int fragEsq = calcularFragmentacao(raiz->esq);

	return result + fragEsq + fragDir;
}

//Percorre a árvore e retorna um array com a quantidade de nós com estado OCUPADO, LIVRE, PARTICIONADO e com o total
// de memória ocupada da árvore, ou seja, com a soma dos tamanhos dos nós com espaço OCUPADO
//O retorno é:
//retorno[0] - número de nós ocupados
//retorno[1] - número de nós livres
//retorno[2] - número de nós particionados
//retorno[3] - total de memória ocupada
int* calculaRelatorioSistema(No* no){
	int* result = malloc(4*sizeof(int)); //array com os valores numOcupados, numLivres, numParticionados, memoriaOcupada

	//Se é nulo retorna o vetor com zeros
	if(no == NULL){
		result[0] = 0;
		result[1] = 0;
		result[2] = 0;
		result[3] = 0;
		return result;
	}

	int* resultDir = calculaRelatorioSistema(no->dir);
	int* resultEsq = calculaRelatorioSistema(no->esq);

	//Somamos os valores do direito e esq
	result[0] = resultDir[0] + resultEsq[0];
	result[1] = resultDir[1] + resultEsq[1];
	result[2] = resultDir[2] + resultEsq[2];
	result[3] = resultDir[3] + resultEsq[3];

	//Verificamos o proprio nó e somamos
	if(no->memoria.estado == 0){
		result[1]++; //LIVRE
	}

	if(no->memoria.estado == 1){
		result[3] += pot(2,no->memoria.valor);
		result[0]++; //OCUPADO
	}

	if(no->memoria.estado == 2){
		result[2]++; //PARTICIONADO
	}

	return result;
}

//De acordo com a função calculaRelatorioSistema, 
//imprimimos na tela os valores calculados
void relatorioSistema(Arvore* arvore){
	printf("[RELATORIO DE SISTEMA]\n");

	int numOcupados, numLivres, numParticionados, memoriaOcupada, totalMemoria;

	totalMemoria = pot(2, arvore->raiz->memoria.valor);

	int* result = calculaRelatorioSistema(arvore->raiz);
	numOcupados = result[0];
	numLivres = result[1];
	numParticionados = result[2];
	memoriaOcupada = result[3];

	printf("%d Ocupados\n", numOcupados);
	printf("%d Livres\n", numLivres);
	printf("%d Particionados\n", numParticionados);

	float memoriaOcupadaFloat = (float)memoriaOcupada;
	float totalMemoriaFloat = (float)totalMemoria;
	float porcMemoria = (memoriaOcupadaFloat/totalMemoriaFloat);
	float truncado = truncf(porcMemoria*100);

	printf("Memoria utilizada = %1.f / 100\n", truncado);
}

//Imprime informações do nó
void imprimirNo(No* no){
  switch (no->memoria.estado){
    case LIVRE:{
      printf("(L:%d)",pot(2,no->memoria.valor));
    } break;
    case PARTICIONADO:{
      printf("(P:%d)",pot(2,no->memoria.valor));
    } break;
    default:{
      printf("(O:%d/%d[%d])",
        no->memoria.processo->tamanho,
        pot(2,no->memoria.valor),
        no->memoria.processo->codigo
      );
    }
  }
}

//Imprime seguindo a ordem simétrica
//Esq, o próprio nó e dir
void imprimirSimetrico(No* no){
  if (no->esq != NULL)
    imprimirSimetrico(&(*no->esq));
  imprimirNo(&(*no));
  if (no->dir != NULL)
    imprimirSimetrico(&(*no->dir));
}

//Imprime seguindo a pré-ordem
//O próprio nó, esq e dir
void imprimirPreOrdem(No* no){
  imprimirNo(&(*no));
  if (no->esq != NULL)
    imprimirPreOrdem(&(*no->esq));
  if (no->dir != NULL)
    imprimirPreOrdem(&(*no->dir));
}

//Imprime seguindo a ordem pós-ordem
//Esq, dir e o próprio nó
void imprimirPosOrdem(No* no){
  if (no->esq != NULL)
    imprimirPosOrdem(&(*no->esq));
  if (no->dir != NULL)
    imprimirPosOrdem(&(*no->dir));
  imprimirNo(&(*no));
}

//Imprime a árvore nas ordens simétrica, pré-ordem e pós-ordem
void imprimirSementesGeradoras(Arvore* arvore){
  No* raiz = arvore->raiz;
  printf("[SEMENTES GERADORAS]\n");
  printf("Sim = ");
  imprimirSimetrico(&(*raiz));
  printf("\n");
  printf("Pre = ");
  imprimirPreOrdem(&(*raiz));
  printf("\n");
  printf("Pos = ");
  imprimirPosOrdem(&(*raiz));
  printf("\n");
}

//Funções de inicialização
void inicializarNo(No* no, int valor){
  no->esq = NULL;
  no->dir = NULL;
  no->memoria.valor = valor;
  no->memoria.estado = LIVRE;
  no->memoria.fragmentacao = 0;
  no->memoria.processo = NULL;
} 

void inicializarArvore(Arvore* arvore, int valor){
  arvore->raiz = (No*)malloc(sizeof(No));
  inicializarNo(&(*arvore->raiz),valor);
}
