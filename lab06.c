/*
  Nome: Matheus Esteves Zanoto   RA: 184256
  Nome: Giovanna Batalha         RA: 

  Objetivos:

  Entradas:

  Saídas:
*/

#include <stdio.h>
#include <stdlib.h>

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

int iniciarProcesso(No* no, Processo processo);
int finalizarProcesso(No* no, int cod);
int calcularFragmentacao(Arvore* arvore);
void relatorioSistema(Arvore* arvore);
void imprimirSementesGeradoras(Arvore* arvore);
void imprimirProcessos(Arvore* arvore);
int pot(int base, int expoente);



//teste eeeee gi
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
        
	  } break;
	  case 4:{

	  } break;
	  case 5:{

	  } break;
	  default:{
        
	  }
    }
  }	
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
		//Chegou no nó que cabe o processo, mas está ocupado
		if(no->memoria.estado == OCUPADO){
			return 0;
		}

		//Chegou no nó que cabe o processo e adicionou o processo
		if (no->memoria.estado == LIVRE){
		  no->memoria.processo = malloc(sizeof(Processo));
		  *(no->memoria.processo) = processo;
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
    no->memoria.estado = LIVRE;
    return 1;
  }
  if (no->memoria.estado == PARTICIONADO){
    int finalizouEsq = finalizarProcesso(&(*no->esq),cod);
    int finalizouDir = finalizarProcesso(&(*no->dir),cod);

    if (no->esq->memoria.estado == LIVRE && no->dir->memoria.estado == LIVRE){
      free(no->esq);
      free(no->dir);
      no->memoria.estado = LIVRE;
      return 1;
    }
    if (finalizouEsq == 1 || finalizouDir == 1)
      return 1;
  }
  return 0;
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
