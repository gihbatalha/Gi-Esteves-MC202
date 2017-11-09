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
Arvore* inicializaArvore(int numProgramas);
void adiciona(No* no);
void retiraNo(No* arvore);

//Métodos de gerenciamento dos programas
void instalaPrograma(No* no, char* programa);
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

	no->pasta.nome = malloc(sizeof(char*));
	no->pasta.programa = malloc(sizeof(char*));
}

Arvore* inicializaArvore(int numProgramas){
	Arvore* arv = malloc(sizeof(Arvore*));
	arv->raiz = NULL;

	arv->sementeInOrder = malloc(numProgramas*sizeof(char*));
	arv->sementePreOrder = malloc(numProgramas*sizeof(char*));

	return arv;
}

void instalaPrograma(No* no, char* programa){
	printf("instalaPrograma:: \n");

	if(no == NULL){
		no = malloc(sizeof(No*));
		inicializaNo(no);
		no->pasta.programa = programa;

		if(no->pai == NULL){ //é a raíz
			no->pasta.nome = "raiz";
		}else{
			printf("paiiiiiiiii nao nulo\n");
			strcat(no->pasta.nome, no->pai->pasta.programa);

			if(no == no->pai->esq){
				strcat(no->pasta.programa, "_esq");
			}else{
				strcat(no->pasta.programa, "_dir");
			}
		}
		
		printf("Pasta criada: %s. Nome do programa: %s\n", no->pasta.nome, no->pasta.programa);
		return;
	}

	printf("lalala\n");

	if(strcmp(programa, no->pasta.programa) < 0){
		if(no->esq == NULL){
			no->esq = malloc(sizeof(No*));
			inicializaNo(no->esq);
			no->esq->pai = no;
		}
		instalaPrograma(no->esq, programa);
	}else{
		if(no->dir == NULL){
			no->dir = malloc(sizeof(No*));
			inicializaNo(no->dir);
			no->dir->pai = no;
		}
		instalaPrograma(no->dir, programa);
	}
}

int main(){
	int p, op; //número de programas

	scanf("%d", &p);
	
	Arvore* arvore = inicializaArvore(p);

	//Leitura das sementes geradoras.
	// for (int i = 0; i < p; ++i){
		
	// }
	//adicionar a partir das sementes os programas na árvore

	while(scanf("%d", &op) != EOF){
		switch(op){

			case 1: ;
					char* programa = malloc(sizeof(char*));
					scanf("%s", programa);					
					printf("Lido: %s\n",programa);

					instalaPrograma(arvore->raiz, programa);
					break;
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