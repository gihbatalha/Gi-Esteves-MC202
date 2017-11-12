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
	int tamanho;
}Arvore;

//Métodos específicos da árvore e do nó
void inicializaNo(No* no);
void inicializaArvore(Arvore* arvore, int numProgramas);
No* buscar(No* no, char* programa);
No* buscarDescendente(No* no);
Pasta* remover(Arvore* arvore, char* programa);
void removerEntreNos(Arvore* arvore, No* filho, No* no, No* pai);
void removerFolha(Arvore* arvore, No* folha);
void liberarArvore(No* no);

//Métodos de gerenciamento dos programas
Pasta* instalaProgramaNaArvore(Arvore* arvore, char* programa);
Pasta* instalaPrograma(No* no, char* programa);
int getVelocidade(No* no, char* programa);
void otimizarCapacidadeDeResposta(Arvore* arvore);
void criaSementesInOrder(Arvore* arvore);
void restaura(Arvore* arvore);
void imprimeCaminhoPrograma(No* no, char* programa);
void imprimeProgramasDaArvore(Arvore* arvore);
void imprimeProgramas(No* no, No* raiz);
void criaSementesInOrdem(No* no, char** sementesInOrdem, int* tamanho);
void balancearArvore(Arvore* arvore);
No* balancear(char** sementesInOrdem, int inicio, int fim);

// Método que libera todos os nós da árvore de forma recursiva pré ordem.
void liberarArvore(No* no){
  if (no->esq != NULL)
    liberarArvore(no->esq);
  if (no->dir != NULL)
    liberarArvore(no->dir);
  free(no);
}

void inicializaNo(No* no){
	no->dir = NULL;
	no->esq = NULL;
	no->pai = NULL;

	no->pasta.nome = malloc(30*sizeof(char));
	no->pasta.programa = malloc(30*sizeof(char));
}

void inicializaArvore(Arvore* arvore, int numProgramas){
	arvore->raiz = NULL;
    arvore->tamanho = 0;
	arvore->sementeInOrder = malloc(numProgramas*sizeof(char*));
	arvore->sementePreOrder = malloc(numProgramas*sizeof(char*));
}

No* buscar(No* no, char* programa){
  if (no == NULL)
    return NULL;

  int comparacao = strcmp(programa, no->pasta.programa);
  if (comparacao < 0)
    return buscar(no->esq,programa);
  if (comparacao > 0)
    return buscar(no->dir,programa);
  return no;
}

Pasta* instalaProgramaNaArvore(Arvore* arvore, char* programa){
  if(arvore->raiz == NULL){
    arvore->raiz = malloc(sizeof(No));
	inicializaNo(arvore->raiz);

	strcpy(arvore->raiz->pasta.nome,"raiz");
	strcat(arvore->raiz->pasta.programa,programa);

    arvore->tamanho++;
	return &(arvore->raiz->pasta);
  }

  arvore->tamanho++;
  return instalaPrograma(arvore->raiz,programa);
}

Pasta* instalaPrograma(No* no, char* programa){

	if(strcmp(programa, no->pasta.programa) < 0){
	  if(no->esq == NULL){
	    no->esq = malloc(sizeof(No));
		inicializaNo(no->esq);
		no->esq->pai = no;

        strcpy(no->esq->pasta.programa,programa);
		strcpy(no->esq->pasta.nome, no->pasta.programa);
		strcat(no->esq->pasta.nome, "_esq");

		return &(no->esq->pasta);
	  }	
	  return instalaPrograma(no->esq, programa);
	}

    if(no->dir == NULL){
	  no->dir = malloc(sizeof(No));
	  inicializaNo(no->dir);
	  no->dir->pai = no;

      strcpy(no->dir->pasta.programa,programa);
      strcpy(no->dir->pasta.nome, no->pasta.programa);
	  strcat(no->dir->pasta.nome, "_dir");

	  return &(no->dir->pasta);
	}
	return instalaPrograma(no->dir, programa);
}

No* buscarDescendente(No* no){
  if (no->dir == NULL)
    return  no;
  return buscarDescendente(no->dir);   
}

void removerEntreNos(Arvore* arvore,No* filho, No* no, No* pai){
  if (pai == NULL){
    arvore->raiz = filho;
    strcpy(arvore->raiz->pasta.nome,"raiz");
  }
  else{
	  strcpy(filho->pasta.nome,pai->pasta.programa);
	  if (pai->esq == no){
	    pai->esq = filho;
	    strcat(filho->pasta.nome,"_esq");
	  }
	  else{
	  	pai->dir = filho;
	  	strcat(filho->pasta.nome,"_dir");
	  }
	  filho->pai = pai;
  }
  free(no);
}

void removerFolha(Arvore* arvore, No* folha){
  No* pai = folha->pai;
  if (pai == NULL){
    free(arvore->raiz);
    arvore->raiz = NULL;
  }
  else{
	  if (pai->esq == folha)
	  	pai->esq = NULL;
	  else
	  	if (pai->dir == folha)
	  	  pai->dir = NULL;
	  free(folha);
  }
}

Pasta* remover(Arvore* arvore, char* programa){
  if (arvore->raiz == NULL)
    return NULL;

  No* noParaExcluir = buscar(arvore->raiz,programa);
  if (noParaExcluir == NULL)
    return NULL;

  Pasta pasta;
  pasta.programa = malloc(30*sizeof(char));
  pasta.nome = malloc(30*sizeof(char));
  strcpy(pasta.programa,noParaExcluir->pasta.programa);
  strcpy(pasta.nome,noParaExcluir->pasta.nome);
  Pasta* ponteiroPasta = &pasta;

  if (noParaExcluir->esq == NULL && noParaExcluir->dir == NULL)
    removerFolha(arvore, noParaExcluir);
  else
    if (noParaExcluir->dir == NULL)
      removerEntreNos(arvore,noParaExcluir->esq,noParaExcluir,noParaExcluir->pai);
    else
      if (noParaExcluir->esq == NULL)
        removerEntreNos(arvore,noParaExcluir->dir,noParaExcluir,noParaExcluir->pai);
      else{
        char* aux = malloc(30*sizeof(char));
        No* descendente = buscarDescendente(noParaExcluir->esq);

        strcpy(aux,descendente->pasta.programa);
        strcpy(descendente->pasta.programa, noParaExcluir->pasta.programa);
        strcpy(noParaExcluir->pasta.programa, aux);

        if (descendente->esq != NULL)
          removerEntreNos(arvore,descendente->esq,descendente,descendente->pai);
        else
          removerFolha(arvore,descendente);

        No* novoEsq = noParaExcluir->esq;
        No* novoDir = noParaExcluir->dir;
        if (novoEsq != NULL){
          strcpy(novoEsq->pasta.nome,noParaExcluir->pasta.programa);
          strcat(novoEsq->pasta.nome,"_esq");
        }
        if (novoDir != NULL){
          strcpy(novoDir->pasta.nome,noParaExcluir->pasta.programa);
          strcat(novoDir->pasta.nome,"_dir");
        }
      }

  arvore->tamanho--;
  return ponteiroPasta;
}

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
}

void criaSementesInOrdem(No* no, char** sementesInOrdem, int* tamanho){
    if (no->esq != NULL)
     criaSementesInOrdem(no->esq,sementesInOrdem,tamanho);

    sementesInOrdem[*tamanho] = malloc(30*sizeof(char));
 	strcpy(sementesInOrdem[*tamanho],no->pasta.programa);
 	(*tamanho)++;

 	if (no->dir != NULL)
 	  criaSementesInOrdem(no->dir,sementesInOrdem,tamanho);
}

void balancearArvore(Arvore* arvore){
  if (arvore->raiz != NULL){
	  char** sementesInOrdem = malloc(arvore->tamanho * sizeof(char*));
	  int tamanho = 0;
	  criaSementesInOrdem(arvore->raiz,sementesInOrdem,&tamanho);
      liberarArvore(arvore->raiz);
      arvore->raiz = balancear(sementesInOrdem,0,arvore->tamanho-1);
      strcpy(arvore->raiz->pasta.nome,"raiz");
  }
}

No* balancear(char** sementesInOrdem, int inicio, int fim){
  int mediana = (inicio + fim)/2;
  No* raiz =  malloc(sizeof(No));
  inicializaNo(raiz);
  strcpy(raiz->pasta.programa,sementesInOrdem[mediana]);

  if (mediana > inicio){
    No* esq = balancear(sementesInOrdem,inicio,mediana-1);
    esq->pai = raiz;
	strcpy(esq->pasta.nome, raiz->pasta.programa);
	strcat(esq->pasta.nome, "_esq");
	raiz->esq = esq;
  }

  if (mediana < fim){
    No* dir = balancear(sementesInOrdem,mediana+1,fim);
    dir->pai = raiz;
    strcpy(dir->pasta.nome, raiz->pasta.programa);
	strcat(dir->pasta.nome, "_dir");
	raiz->dir = dir;
  }

  return raiz;
}

int getVelocidade(No* no, char* programa){
  char* programaComparado = no->pasta.programa;
  if (strcmp(programa,programaComparado) == 0)
    return 0;
  if (strcmp(programa,programaComparado) < 0)
    return 1 + getVelocidade(no->esq,programa);
  return 1 + getVelocidade(no->dir,programa);
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
			} break;
			case 2:{
              char* programa = malloc(30*sizeof(char));
              scanf("%s",programa);
              Pasta* pasta = remover(&arvore, programa);
              if (pasta == NULL)
                printf("[UNINSTALL] Nao foi encontrado no sistema nenhum programa com nome %s\n",programa);
              else
                printf("[UNINSTALL] Programa %s.exe desinstalado com sucesso\n",pasta->programa);
			} break;
			case 3:{
              char* programa = malloc(30*sizeof(char));
              int tempo;
              scanf("%s %d",programa,&tempo);
              int tempoAcesso = getVelocidade(arvore.raiz,programa);
              if (tempoAcesso > tempo)
                printf("[DELAY][FAIL] O acesso ao programa %s.exe ultrapassou o limite de %d segundo\n",programa,tempo);
              else
                printf("[DELAY][OK] O acesso ao programa %s.exe foi concluido em %d segundos\n",programa,tempoAcesso);
			} break;
			case 4:{
              balancearArvore(&arvore);
              printf("[OPTIMIZE] O sistema de acesso a programas foi otimizado\n");
			} break;
			case 5:{

			} break;
			case 6:{

			} break;
			default:{
              imprimeProgramasDaArvore(&arvore);
			}
		}
	}
}