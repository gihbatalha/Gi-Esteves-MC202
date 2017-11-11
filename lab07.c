#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct Pasta{
	char* nome;
	char* programa;
}Pasta;

typedef struct NoArv{
	struct NoArv* esq;
	struct NoArv* dir;
	struct NoArv* pai;

	struct Pasta pasta;
}No;

typedef struct arvoreBinaria{
	No* raiz;
	char** sementeInOrder; //vetor de strings
	char** sementePreOrder;
}Arvore;

//Métodos específicos da árvore e do nó
void inicializaNo(No* no);
void inicializaArvore(Arvore* arvore, int numProgramas);
void adiciona(No* no);
void retiraNo(No* arvore);

//Métodos de gerenciamento dos programas
Pasta* instalaProgramaNaArvore(Arvore* arvore, char* programa);
Pasta* instalaPrograma(No* no, char* programa);
void desinstalaPrograma(Arvore* arvore, char* programa);
void testaVelocidade(Arvore* arvore, char* programa, int tempo);
void otimizarCapacidadeDeResposta(Arvore* arvore);
void criaSementesInOrder(Arvore* arvore);
void restaura(Arvore* arvore);
void imprimeProgramas(Arvore* arvore);

void inicializaNo(No* no){
	no->dir = NULL;
	no->esq = NULL;
	no->pai = NULL;

	no->pasta.nome = malloc(30*sizeof(char));
	no->pasta.programa = malloc(30*sizeof(char));
}

void inicializaArvore(Arvore* arvore, int numProgramas){
	arvore->raiz = NULL;

	arvore->sementeInOrder = malloc(numProgramas*sizeof(char*));
	arvore->sementePreOrder = malloc(numProgramas*sizeof(char*));
}

Pasta* instalaProgramaNaArvore(Arvore* arvore, char* programa){
  if(arvore->raiz == NULL){
    arvore->raiz = malloc(sizeof(No));
	inicializaNo(arvore->raiz);

	strcat(arvore->raiz->pasta.nome,"raiz");
	strcat(arvore->raiz->pasta.programa,programa);

	return &(arvore->raiz->pasta);
  }

  return instalaPrograma(arvore->raiz,programa);
}

Pasta* instalaPrograma(No* no, char* programa){

	if(strcmp(programa, no->pasta.programa) < 0){
	  if(no->esq == NULL){
	    no->esq = malloc(sizeof(No));
		inicializaNo(no->esq);
		no->esq->pai = no;

        strcat(no->esq->pasta.programa,programa);
		strcat(no->esq->pasta.nome, no->pasta.programa);
		strcat(no->esq->pasta.nome, "_esq");

		return &(no->esq->pasta);
	  }	
	  return instalaPrograma(no->esq, programa);
	}

    if(no->dir == NULL){
	  no->dir = malloc(sizeof(No));
	  inicializaNo(no->dir);
	  no->dir->pai = no;

      strcat(no->dir->pasta.programa,programa);
      strcat(no->dir->pasta.nome, no->pasta.programa);
	  strcat(no->dir->pasta.nome, "_dir");

	  return &(no->dir->pasta);
	}
	return instalaPrograma(no->dir, programa);
}

int main(){
	int p, op; //número de programas

	scanf("%d", &p);
	
	Arvore arvore;
	inicializaArvore(&arvore,p);

	//Leitura das sementes geradoras.
	// for (int i = 0; i < p; ++i){
		
	// }
	//adicionar a partir das sementes os programas na árvore

	while(scanf("%d", &op) != EOF){
		switch(op){

			case 1:{
			  char* programa = malloc(30*sizeof(char));
			  scanf("%s", programa);					
			  Pasta* pasta = instalaProgramaNaArvore(&arvore, programa);
			  printf("[INSTALL] Programa %s.exe instalado com sucesso na pasta %s\n",pasta->programa,pasta->nome);
			}break;
			case 2:	
					break;
			case 3:
					break;
			case 4:
					break;
			case 5:
					break;
			case 6:
					break;
			case 7:
					break;
		}
	}
}