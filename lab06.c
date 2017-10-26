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
  int valor;
  Estado estado;
  int fragmentacao;
  Processo* processo;
} Memoria;

typedef struct No{
  Memoria memoria;
  No* esq;
  No* dir;
} No;

typedef struct Arvore{
  No* raiz;
} Arvore;

void inicializarNo(No* no, int valor);
void inicializarArvore(Arvore* arvore, int valor);
void percorrerNoParaExcluir(No* no, int cod);
void excluirNoFolha(Arvore* arvore, int cod);

void iniciarProcesso(Arvore* arvore, Processo processo);
void finalizarProcesso(Arvore* arvore, int cod);
int calcularFragmentacao(Arvore* arvore);
void relatorioSistema(Arvore* arvore);
void imprimirSementesGeradoras(Arvore* arvore)
void imprimirProcessos(Arvore* arvore);

//teste
int main(){
  int potencia;
  scanf("%d",&potencia);

  Arvore arvore;
  inicializarArvore(&arvore,potencia);

  int op;
  while(scanf("%d", &op) != EOF){
    switch(op){
	  case 1:{

	  } break;
	  case 2:{

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

void inicializarNo(No* no, int valor){
  no->esq = null;
  no->dir = null;
  no->memoria.valor = valor;
  no->memoria.estado = LIVRE;
  no->memoria.fragmentacao = 0;
  no->memoria.processo = NULL;
} 

void inicializarArvore(Arvore* arvore, int valor){
  arvore->raiz = (No*)malloc(sizeof(No));
  inicializarNo(&(*arvore->raiz),valor);
}

void percorrerNoParaExcluir(No* no, int cod){
  if (no->esq == NULL && no-dir == NULL && no->memoria.processo->codigo == cod)
    free(no);
  else{
    if (no->esq != NULL)
      percorrerNoParaExcluir(&(*no->esq),cod);
    if (no->dir != NULL)
      percorrerNoParaExcluir(&(*no->dir),cod);
  }
}

void excluirNoFolha(Arvore* arvore, int cod){
  No* raiz = arvore->raiz;
  percorrerNoParaExcluir(&(*arvore->raiz),cod);
}
