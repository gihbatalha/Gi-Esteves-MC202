/*
  Nome: Matheus Esteves Zanoto   RA: 184256
  Nome: Giovanna Batalha         RA: 

  Objetivos:

  Entradas:

  Saídas:
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
  int valor; //Guarda a potencia exemplo 10 é como se fosse 2^10
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
int iniciarProcesso(No* no, Processo processo); //testado com o teste 1 do susy

int calcularFragmentacao(No* no);  ///funcionando
void relatorioSistema(Arvore* arvore); //Gi fazendooo
int* calculaRelatorioSistema(No* no); //Método auxiliar 
void imprimirNo(No* no);
void imprimirSimetrico(No* no);
void imprimirPreOrdem(No* no);
void imprimirPosOrdem(No* no);
void imprimirSementesGeradoras(Arvore* arvore);
void imprimirProcessos(Arvore* arvore);
int pot(int base, int expoente); //funcionando
void imprimeArvore(No* no, char* espaco); //feito mas printa meio bizarro
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
	  	No* raiz = arvore.raiz;
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
	  	if(!imprimeProcessos(arvore.raiz)){	
	  		printf("Nenhum processo presente\n");
	  	}
	  }break;
	  default:{
        char* espaco = malloc(sizeof(char*));
	  	espaco = strcat(espaco, "  ");
	  	imprimeArvore(arvore.raiz, espaco);
	  }
    }
  }	
}

void imprimeArvore(No* no, char* espaco){
	if(espaco == NULL){
		printf("espaco nullllll\n");
	}
	espaco = strcat(espaco, "  ");

	printf("{\n");

	if(no == NULL){
		printf("}\n");
		return;

	}else{
		int potencia = pot(2, no->memoria.valor);
		printf("%s valor %d\n",espaco, potencia);

		int estado = no->memoria.estado;
		char* estadoString = malloc(sizeof(char*));
		if(estado == 0){
			estadoString = "LIVRE";
		}

		if(estado == 1){
			estadoString = "OCUPADO";
		}

		if(estado == 2){
			estadoString = "PARTICIONADO";
		}

		printf("%s estado %s \n", espaco, estadoString);
		printf("%s fragmentacao: %d\n", espaco, no->memoria.fragmentacao);
		printf("%s processo: cod: %d size: %d\n", espaco, no->memoria.processo->codigo, no->memoria.processo->tamanho);

		printf("%s esq:", espaco);
		imprimeArvore(no->esq, espaco);

		printf("%s dir:", espaco);
		imprimeArvore(no->dir, espaco);

		printf("%s \n", espaco);
	}
}

int imprimeProcessos(No* no){
	No* atual = no;
	int dir, esq, printouNo;

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

	if(!esq && !dir && !printouNo){
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
int iniciarProcesso(No* no, Processo processo){
	int metadeAtual = pot(2, no->memoria.valor-1);
	int valorAtual = pot(2, no->memoria.valor);

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

	if(no->esq == NULL && no->dir == NULL){
		no->memoria.estado = PARTICIONADO;
	}

	if(no->esq == NULL){
		no->esq = malloc(sizeof(No));
		inicializarNo(&(*no->esq), no->memoria.valor-1);
	}

	if(no->dir == NULL){
		no->dir = malloc(sizeof(No));
		inicializarNo(&(*no->dir), no->memoria.valor-1);
	}

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

int finalizarProcesso(No* no, int cod){
  if (no->memoria.estado == OCUPADO && no->memoria.processo->codigo == cod){
    free(no->memoria.processo);
    no->memoria.fragmentacao = 0;
    no->memoria.estado = LIVRE;
    return 1;
  }
  if (no->memoria.estado == PARTICIONADO){
    int finalizouEsq = finalizarProcesso(&(*no->esq),cod);
    int finalizouDir = finalizarProcesso(&(*no->dir),cod);

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

int calcularFragmentacao(No* raiz){
	if(raiz == NULL){
		return 0;
	}

	int result = raiz->memoria.fragmentacao;
	int fragDir = calcularFragmentacao(raiz->dir);
	int fragEsq = calcularFragmentacao(raiz->esq);

	return result + fragEsq + fragDir;
}

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

	printf("Memoria utilizada = %1.f / 100\n", porcMemoria * 100);
}

void imprimirNo(No* no){
  switch (no->memoria.estado){
    case LIVRE:{
      printf("(L:%d)",pot(2,no->memoria.valor));
    } break;
    case PARTICIONADO:{
      printf("(P:%d)",pot(2,no->memoria.valor));
    } break;
    default:{
      printf("(O:%d/%d[%d]",
        no->memoria.processo->tamanho,
        pot(2,no->memoria.valor),
        no->memoria.processo->codigo
      );
    }
  }
}

void imprimirSimetrico(No* no){
  if (no->esq != NULL)
    imprimirSimetrico(&(*no->esq));
  imprimirNo(&(*no));
  if (no->dir != NULL)
    imprimirSimetrico(&(*no->dir));
}

void imprimirPreOrdem(No* no){
  imprimirNo(&(*no));
  if (no->esq != NULL)
    imprimirPreOrdem(&(*no->esq));
  if (no->dir != NULL)
    imprimirPreOrdem(&(*no->dir));
}

void imprimirPosOrdem(No* no){
  if (no->esq != NULL)
    imprimirPosOrdem(&(*no->esq));
  if (no->dir != NULL)
    imprimirPosOrdem(&(*no->dir));
  imprimirNo(&(*no));
}

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
