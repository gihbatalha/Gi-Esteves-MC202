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
No* buscar(No* no, char* programa);
No* buscarDescendente(No* no);
Pasta* remover(Arvore* arvore, char* programa);
void removerEntreNos(Arvore* arvore, No* filho, No* no, No* pai);
void removerFolha(Arvore* arvore, No* folha);

//Métodos de gerenciamento dos programas
Pasta* instalaProgramaNaArvore(Arvore* arvore, char* programa);
Pasta* instalaPrograma(No* no, char* programa);
void testaVelocidade(Arvore* arvore, char* programa, int tempo);
void otimizarCapacidadeDeResposta(Arvore* arvore);
void criaSementesInOrder(Arvore* arvore);
void restaura(Arvore* arvore);
void imprimeCaminhoPrograma(No* no, char* programa);
void imprimeProgramasDaArvore(Arvore* arvore);
void imprimeProgramas(No* no, No* raiz);

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

        strcat(aux,descendente->pasta.programa);
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

			} break;
			case 4:{

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