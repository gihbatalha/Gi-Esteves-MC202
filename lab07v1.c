#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum Semente{
	PREORDER, INORDER
}Semente;

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
	int tamanhoInOrder;
	char** sementePreOrder;
	int tamanhoPreOrder;
}Arvore;

//Métodos específicos da árvore e do nó
void inicializaNo(No* no);
void inicializaArvore(Arvore* arvore);
void adiciona(No* no);
void retiraNo(No* arvore);

//Métodos de gerenciamento dos programas
Pasta* instalaProgramaNaArvore(Arvore* arvore, char* programa);
Pasta* instalaPrograma(No* no, char* programa);
void desinstalaPrograma(Arvore* arvore, char* programa);
void testaVelocidade(Arvore* arvore, char* programa, int tempo);
void otimizarCapacidadeDeResposta(Arvore* arvore);

void criaSementes(Arvore* arvore);
void criaSementesInOrder(Arvore* arvore, No* no);
void criaSementesPreOrder(Arvore* arvore, No* no);
void adicionaSemente(Arvore* arv, char** semente, int* tamanhoSemente, char* novoValor, Semente tipoSemente);
void imprimeSementes(char** sementes, int tamanhoSemente);
void imprimeSementesArvore(Arvore* arv);

No* restaura(No* no, char** pre, char** in, int *preTamanho, int inTamanho);
//void imprimeProgramas(Arvore* arvore);

void imprimeProgramasDaArvore(Arvore* arvore);
void imprimeProgramas(No* no, No* raiz);
void imprimeCaminhoPrograma(No* no, char* programa);


void imprimeCaminhoPrograma(No* no, char* programa){
  if (no != NULL){
	  printf("%s/",no->pasta.nome);
	  int comparacao = strcmp(programa, no->pasta.programa);
	  if(comparacao < 0){
	    imprimeCaminhoPrograma(no->esq,programa);
	  }
	  else
	  	if (comparacao > 0){
	  	  imprimeCaminhoPrograma(no->dir,programa);
	  	}
	  	else
	  	  printf("%s.exe\n",no->pasta.programa);
  }
}

void imprimeProgramas(No* no, No* raiz){
  
  if (no->esq != NULL)
    imprimeProgramas(no->esq,raiz);

  printf("C:/");
  imprimeCaminhoPrograma(raiz,no->pasta.programa);

  if (no->dir != NULL)
    imprimeProgramas(no->dir,raiz);
}

void imprimeProgramasDaArvore(Arvore* arvore){
  printf("[PATHS]\n");
  if (arvore->raiz != NULL)
    imprimeProgramas(arvore->raiz,arvore->raiz);
  else
  	printf("arvore->raiz nula\n");
}

void inicializaNo(No* no){
	no->dir = NULL;
	no->esq = NULL;
	no->pai = NULL;

	no->pasta.nome = malloc(30*sizeof(char));
	no->pasta.programa = malloc(30*sizeof(char));
}

void inicializaArvore(Arvore* arvore){
	arvore->raiz = NULL;

	arvore->sementeInOrder = malloc(sizeof(char*));
	arvore->sementePreOrder = malloc(sizeof(char*));
	arvore->tamanhoInOrder = 0;
	arvore->tamanhoPreOrder = 0;
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

void adicionaSemente(Arvore* arv, char** semente, int* tamanhoSemente, char* novoValor, Semente tipoSemente){
	if(novoValor == NULL){
		printf("ERRO [adicionaSemente] novoValor da semente NULO\n");
		return;
	}

	if(semente == NULL){
		printf("semente nulaaaa\n");
		return;
	}

	//Aumenta o tamanho do vetor de semente
	(*tamanhoSemente)++;

	char** novaSemente = malloc((*tamanhoSemente)*sizeof(char*));

	//Passa o que tinha no vetor antigo para o novo
	for (int i = 0; i < (*tamanhoSemente)-1; i++){
		novaSemente[i] = semente[i];
	}

	int pos = (*tamanhoSemente) - 1;	
	novaSemente[pos] = novoValor;

	if(tipoSemente == 0){
		arv->sementePreOrder = novaSemente;	
	}else{
		arv->sementeInOrder = novaSemente;
	}	
}

//Imprime seguindo a ordem simétrica
//Esq, o próprio nó e dir
void criaSementesInOrder(Arvore* arv, No* no){
  if (no->esq != NULL){
    criaSementesInOrder(arv, no->esq);
  }

  adicionaSemente(arv, arv->sementeInOrder, &(arv->tamanhoInOrder), no->pasta.programa, INORDER);

  if (no->dir != NULL){
    criaSementesInOrder(arv, no->dir);
  }
}

//Imprime seguindo a pré-ordem
//O próprio nó, esq e dir
void criaSementesPreOrder(Arvore* arv, No* no){
  adicionaSemente(arv, arv->sementePreOrder, &(arv->tamanhoPreOrder), no->pasta.programa, PREORDER);
  if (no->esq != NULL)
    criaSementesPreOrder(arv, no->esq);
  if (no->dir != NULL)
    criaSementesPreOrder(arv, no->dir);
}

void desalocaSementes(Arvore* arvore){
	printf("desalocaSementes...\n");
	printf("tamanhoPreOrder: %d\n", arvore->tamanhoPreOrder);
	printf("tamanhoInOrder: %d\n", arvore->tamanhoInOrder);

	if(arvore->sementePreOrder != NULL){
		printf("sementePreOrder != null\n");
		for (int i = 0; i < arvore->tamanhoPreOrder; ++i){
			free(arvore->sementePreOrder[i]);
		}

		//(arvore->sementePreOrder);
		arvore->tamanhoPreOrder = 0;
	}

	if(arvore->sementeInOrder != NULL){
		printf("sementeInOrder != null\n");
		for (int i = 0; i < arvore->tamanhoInOrder; ++i){
			free(arvore->sementeInOrder[i]);
		}

		//arvore->sementeInOrder);
		arvore->tamanhoInOrder = 0;
	}
}

//Cria as sementes InOrder e PreOrder
void criaSementes(Arvore* arvore){
	desalocaSementes(arvore);

	criaSementesPreOrder(arvore, arvore->raiz);
	criaSementesInOrder(arvore, arvore->raiz);

	imprimeSementesArvore(arvore);
}

void imprimeSementes(char** sementes, int tamanhoSemente){
	//printf("imprimeSementes. tamanho: %d\n", tamanhoSemente);
	int i;

	for (i = 0; i < tamanhoSemente; i++){
		printf("[%d] %s\n", i, sementes[i]);
	}
}

void imprimeSementesArvore(Arvore* arv){
	int i;

	printf("semente pre order:\n");
	for (i = 0; i < arv->tamanhoPreOrder; ++i){
		printf("PreOrder[%d] %s\n", i, arv->sementePreOrder[i]);
	}

	printf("semente in order:\n");
	for (i = 0; i < arv->tamanhoInOrder; ++i){
		printf("InOrder[%d] %s\n", i, arv->sementeInOrder[i]);
	}
}

int indexOf(char* programa, char** vetor, int tamanho){
	int i;
	for (i = 0; i < tamanho; i++){
		if(strcmp(vetor[i], programa) == 0){
			return i;
		}
	}
}

No* restaura(No* no, char** pre, char** in, int* preTamanho, int inTamanho){

	if(pre == NULL){
		return NULL;
	}

	no = malloc(sizeof(No*));
	no->pasta.programa = pre[0];

	printf("pre[0]: %s\n", pre[0]);
	printf("no->pasta.programa: %s\n", no->pasta.programa);

	//achamos o primeiro elemento do pré-order no in-order
	int indexRaiz = indexOf(pre[0], in, inTamanho);

	// printf("atual: %s\n", pre[0]);
	// printf("in\n");
	// imprimeSementes(in, inTamanho);
	// printf("pre\n");
	// imprimeSementes(pre, preTamanho);

	// printf("indexRaiz: %d\n", indexRaiz);

	int tamanhoInDir = inTamanho - indexRaiz - 1;
	int tamanhoInEsq = indexRaiz;
	//int tamanhoNovoPre = preTamanho - 1;

	// printf("tamanhoInDir: %d\n", tamanhoInDir);
	// printf("inTamanho: %d\n", inTamanho);

	//constrói uma lista com os elementos da arvore esquerda da raiz atual baseado no in
	char** inEsq = malloc(indexRaiz*sizeof(char*));
	char** inDir = malloc(tamanhoInDir*sizeof(char*));

	for (int i = 0; i < inTamanho; ++i){
		//Adicionamos na lista in da esq os elementos do zero ao index
		if(i < indexRaiz){
			inEsq[i] = in[i];
		}

		//Adicionamos na lista in da dir os elementos do zero ao index
		if(i > indexRaiz){
			int index = i - indexRaiz-1;
			inDir[index] = in[i];
		}
	}

	//Removemos o primeiro elemento da lista pre order para que o proximo seja analisado na recursão
	for (int j = 1; j < *preTamanho; j++){
		pre[j-1] = pre[j];
	}

	pre[*preTamanho] = NULL;
	free(pre[*preTamanho]);

	(*preTamanho)--;

	// printf("inEsq:\n");
	// imprimeSementes(inEsq, indexRaiz);
	// printf("inDir\n");
	
	// imprimeSementes(inDir, tamanhoInDir);
	// printf("pre\n");
	// imprimeSementes(pre, preTamanho-1);

	if(indexRaiz == 0){
		no->esq = NULL;
		no->dir = NULL;
		return no;
	}

	// printf("tamanhoInDir: %d\n", tamanhoInDir);
	// printf("Restaura esq...\n");

	if(indexRaiz > 0){
		no->esq = restaura(no->esq, pre, inEsq, preTamanho, indexRaiz);
		printf("no->pasta.programa: %s\n", no->pasta.programa);

		 if(no->esq != NULL){

			strcpy(no->esq->pasta.nome, no->pasta.programa);
		//  strcat(no->esq->pasta.nome, "_esq");

		//  printf("no->esq->pasta.programa: %s\n", no->esq->pasta.nome);
		}
		
		no->esq->pai = no;
	}
	

	//printf("Restaura dir... \n");

	if(tamanhoInDir > 0){
		no->dir = restaura(no->dir, pre, inDir, preTamanho, tamanhoInDir);

		if(no->dir != NULL){
		// 	printf("no->pasta.programa: %s\n", no->pasta.programa);
		
			strcpy(no->dir->pasta.nome, no->pasta.programa);

		// 	strcat(no->dir->pasta.nome, "_dir");
		// 	printf("no->dir->pasta.programa: %s\n", no->dir->pasta.nome);
		}
		
		no->dir->pai = no;
	}

	return no;
	
}

void imprime(No* raiz){
	printf("%s\n", raiz->pasta.programa);

	printf("%s", raiz->pasta.programa);
	printf("_esq:");

	imprime(raiz->esq);

	printf("%s", raiz->pasta.programa);
	printf("_dir:");

	imprime(raiz->dir);
}

int main(){
	int p, op; //número de programas

	scanf("%d", &p);
	
	Arvore arvore;
	inicializaArvore(&arvore);

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
			case 5:{
					criaSementes(&arvore);
			}break;
			case 6:{
					Arvore novaArvore;
					inicializaArvore(&novaArvore);

					novaArvore.raiz = restaura(novaArvore.raiz, arvore.sementePreOrder, arvore.sementeInOrder, &(arvore.tamanhoPreOrder), arvore.tamanhoInOrder);
					printf("tamanhoPreOrder: %d\n", arvore.tamanhoPreOrder);
					printf("tamanhoInOrder: %d \n", arvore.tamanhoInOrder);

					printf("Printando nova arvore: \n");
					imprimeProgramasDaArvore(&novaArvore);
					//criaSementes(&novaArvore);

					//desalocar a arvore anterior;
					//arvore = novaArvore;
			}break;
			case 7:{
				imprimeProgramasDaArvore(&arvore);
			}
					break;
		}
	}
}