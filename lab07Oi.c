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
	int tamanho;
}Arvore;

//Métodos específicos da árvore e do nó
void inicializaNo(No* no);
void inicializaArvore(Arvore* arvore);
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
void imprimeCaminhoPrograma(No* no, char* programa);
void imprimeProgramasDaArvore(Arvore* arvore);
void imprimeProgramas(No* no, No* raiz);
void balancearArvore(Arvore* arvore);
No* balancear(char** sementesInOrdem, int inicio, int fim);

void criaSementes(Arvore* arvore);
void criaSementesInOrder(Arvore* arvore, No* no);
void criaSementesPreOrder(Arvore* arvore, No* no);
void adicionaSemente(Arvore* arv, char** semente, int* tamanhoSemente, char* novoValor, Semente tipoSemente);
void imprimeSementes(char** sementes, int tamanhoSemente);
void imprimeSementesArvore(Arvore* arv);

No* restaura(No* no, No* anterior, char** pre, char** in, int *preTamanho, int inTamanho, int eEsq);

// Método que libera todos os nós da árvore de forma recursiva pré ordem.
void liberarArvore(No* no){

  if (no->esq != NULL)
    liberarArvore(no->esq);
  if (no->dir != NULL)
    liberarArvore(no->dir);

  if(no != NULL)
  free(no);
}

void inicializaNo(No* no){
	no->dir = NULL;
	no->esq = NULL;
	no->pai = NULL;

	no->pasta.nome = malloc(30*sizeof(char));
	no->pasta.programa = malloc(30*sizeof(char));
}

void inicializaArvore(Arvore* arvore){
	arvore->raiz = malloc(sizeof(No*));
  arvore->tamanho = 0;
	arvore->sementeInOrder = malloc(sizeof(char*));
	arvore->sementePreOrder = malloc(sizeof(char*));
  arvore->tamanhoInOrder = 0;
  arvore->tamanhoPreOrder = 0;
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
    printf("tamanho atual da semente: %d\n", *tamanho);
    if (no->esq != NULL){
     criaSementesInOrdem(no->esq,sementesInOrdem,tamanho);
    }



    printf("oie - tamanho: %d\n", *tamanho);
    //sementesInOrdem[*tamanho] = malloc(30*sizeof(char));
    printf("sementesInOrdem: %p\n",sementesInOrdem);
    sementesInOrdem[*tamanho] = malloc(sizeof(char*));

    printf("oie 2\n");
 	  strcpy(sementesInOrdem[*tamanho],no->pasta.programa);
    printf("oie 3 - %s\n",sementesInOrdem[*tamanho]);
 	  (*tamanho)++;
    printf("oie 4 = tamanho: %d\n", *tamanho);

 	if (no->dir != NULL){
 	  criaSementesInOrdem(no->dir,sementesInOrdem,tamanho);
  }
}

void balancearArvore(Arvore* arvore){
  printf("balancear arvore...\n");
  printf("arvore->raiz: %p\n", arvore->raiz);
  if (arvore->raiz != NULL){
    printf("nao eh nulo\n");
    printf("arvore->tamanho: %d\n", arvore->tamanho);

	  char** sementesInOrdem = malloc(arvore->tamanho * sizeof(char*));

    if(sementesInOrdem == NULL){
      printf("eeeeii semente nula\n");
    }
	  int tamanho = 0;

    printf("deu malloc\n");
	  criaSementesInOrdem(arvore->raiz,sementesInOrdem,&tamanho);

      printf("oi..\n");
      //liberarArvore(arvore->raiz);
      arvore->raiz = balancear(sementesInOrdem,0,arvore->tamanho-1);
      strcpy(arvore->raiz->pasta.nome,"raiz");
  }
}

No* balancear(char** sementesInOrdem, int inicio, int fim){
  //printf("balancear...\n");
  int mediana = (inicio + fim)/2;
  //printf("mediana: %d\n", mediana);

  No* raiz =  malloc(sizeof(No));
  inicializaNo(raiz);

  //printf("antes de copiar\n");
  strcpy(raiz->pasta.programa,sementesInOrdem[mediana]);

  //printf("depois de copiar\n");
  if (mediana > inicio){
    //printf("mediana > inicio\n");
    No* esq = balancear(sementesInOrdem,inicio,mediana-1);
    esq->pai = raiz;
	  strcpy(esq->pasta.nome, raiz->pasta.programa);
	  strcat(esq->pasta.nome, "_esq");
	  raiz->esq = esq;
  }

  if (mediana < fim){
    //printf("mediana < fim\n");
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

//Cria semente inorder
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

//Cria sementes pré-order
//O próprio nó, esq e dir
void criaSementesPreOrder(Arvore* arv, No* no){
  adicionaSemente(arv, arv->sementePreOrder, &(arv->tamanhoPreOrder), no->pasta.programa, PREORDER);
  if (no->esq != NULL)
    criaSementesPreOrder(arv, no->esq);
  if (no->dir != NULL)
    criaSementesPreOrder(arv, no->dir);
}

void desalocaSementes(Arvore* arvore){
  arvore->tamanhoPreOrder = 0;
  arvore->tamanhoInOrder = 0;

  arvore->sementePreOrder = NULL;
  arvore->sementeInOrder = NULL;
  // printf("coloquei nulll\n");
  // printf("pre order: %p\n",arvore->sementePreOrder);
  // printf("in order: %p\n",arvore->sementeInOrder);

  arvore->sementeInOrder = malloc(sizeof(char*));
  arvore->sementePreOrder = malloc(sizeof(char*));

  // printf("inicializei\n");
  // printf("pre order: %p\n",arvore->sementePreOrder);
  // printf("in order: %p\n",arvore->sementeInOrder);
}

//Cria as sementes InOrder e PreOrder
void criaSementes(Arvore* arvore){
  desalocaSementes(arvore);

  //printf("LALALALALALLALA\n");
  

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

// PASSA A MAIS O LADO QUE ESTA, PARA PODER GERAR O NOME
No* restaura(No* no, No* anterior, char** pre, char** in, int* preTamanho, int inTamanho, int eEsq){
  if(preTamanho == 0 || inTamanho == 0) {
    //printf("retornou null\n");
    return NULL;
  }

  // Criação do novo nó
  no = malloc(sizeof(No));
  no->pasta.programa = malloc(30*sizeof(char));
  strcpy(no->pasta.programa, pre[0]);
  no->pasta.nome = malloc(30*sizeof(char));
  // RAIZ
  if (anterior == NULL) {
    strcpy(no->pasta.nome, "raiz");
  } else { // NÓ QUE POSSUI UM PAI E SUFIXO NA PASTA
    strcpy(no->pasta.nome, anterior->pasta.programa);
    strcat(no->pasta.nome, eEsq == 1 ? "_esq" : "_dir");
    no->pai = anterior;
  }

  // if(inTamanho == 1){
  //   printf("NOME PASTA: %s - NOME PROGRAMA: %s\n", no->pasta.nome, no->pasta.programa);
  //   return no;
  // }

  //achamos o primeiro elemento do pré-order no in-order
  int indexRaiz = indexOf(pre[0], in, inTamanho);
  int tamanhoInDir = inTamanho - indexRaiz - 1;
  int tamanhoInEsq = indexRaiz;
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

  // Removemos o pre ja utilizado
  pre[*preTamanho] = NULL;
  free(pre[*preTamanho]);

  (*preTamanho)--;

  // Nao é necessario verificar, se for NULL o caso base ira retornar null
  no->esq = restaura(no->esq, no, pre, inEsq, preTamanho, indexRaiz, 1);
  no->dir = restaura(no->dir, no, pre, inDir, preTamanho, tamanhoInDir, 0);
  printf("NOME PASTA: %s - NOME PROGRAMA: %s\n", no->pasta.nome, no->pasta.programa);
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

  printf("p: %d\n",p);

	Arvore arvore;
	inicializaArvore(&arvore);

	//Leitura das sementes geradoras.
  int i;

  //printf("in order...\n");
	for (i = 0; i < p; i++){
		char* atual = malloc(sizeof(char*));
    scanf("%s", atual);
    //printf("atual: %s\n", atual);

    adicionaSemente(&arvore, arvore.sementeInOrder, &arvore.tamanhoInOrder, atual, INORDER);
	}

  int j;
  //printf("pre order....\n");
  for (j = 0; j < p; j++){
    char* atual = malloc(sizeof(char*));
    scanf("%s", atual);
    //printf("atual: %s\n", atual);

    adicionaSemente(&arvore, arvore.sementePreOrder, &arvore.tamanhoPreOrder, atual, PREORDER);
  }

  //printf("OIII\n");
  imprimeSementesArvore(&arvore);  

  //No* teste = malloc(sizeof(No*));

  //adicionar a partir das sementes os programas na árvore
  arvore.raiz =  restaura(arvore.raiz, NULL, arvore.sementePreOrder, arvore.sementeInOrder, &arvore.tamanhoPreOrder, arvore.tamanhoInOrder, 0);
  arvore.tamanho = arvore.tamanhoInOrder;
  printf("tamanho arvore: %d\n", arvore.tamanho);
  //imprimeProgramas(teste, teste);

  //arvore.raiz = teste;
  //imprimeProgramas(arvore8.raiz, arvore.raiz);

  criaSementes(&arvore);
  printf("\n\n");
  imprimeSementesArvore(&arvore);
 

	
	while(scanf("%d", &op) != EOF){
		switch(op){

			case 1:{
			  char* programa = malloc(30*sizeof(char));
			  scanf("%s", programa);					
			  Pasta* pasta = instalaProgramaNaArvore(&arvore, programa);
			  printf("[INSTALL] Programa %s.exe instalado com sucesso na pasta %s\n",pasta->programa,pasta->nome);
        printf("novo tamanho após inclusao: %d\n", arvore.tamanho);
			} break;
			case 2:{
              char* programa = malloc(30*sizeof(char));
              scanf("%s",programa);
              Pasta* pasta = remover(&arvore, programa);
              if (pasta == NULL)
                printf("[UNINSTALL] Nao foi encontrado no sistema nenhum programa com nome %s\n",programa);
              else
                printf("[UNINSTALL] Programa %s.exe desinstalado com sucesso\n",pasta->programa);

              printf("novo tamanho após a remoção: %d\n", arvore.tamanho);
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
              printf("Oi...\n");
              printf("arvore: %p\n", &arvore);
              balancearArvore(&arvore);
              printf("[OPTIMIZE] O sistema de acesso a programas foi otimizado\n");
			} break;
			case 5:{
              criaSementes(&arvore);
              printf("[BACKUP] Configuracao atual do sistema salva com sucesso\n");
			} break;
			case 6:{
              //Arvore novaArvore;
              //inicializaArvore(&novaArvore);
              //novaArvore.raiz = 
              //printf("vai restaurar...\n");
              //No* teste = malloc(sizeof(No*)); 
              //teste = 

              //desalocar os nós antigos da árvore
              arvore.raiz = restaura(arvore.raiz, NULL, arvore.sementePreOrder, arvore.sementeInOrder, &(arvore.tamanhoPreOrder), arvore.tamanhoInOrder, 0);
              arvore.tamanho = arvore.tamanhoInOrder;
              printf("tamanho arvore: %d\n", arvore.tamanho);
              //imprimeProgramasDaArvore(&arvore);
              criaSementes(&arvore);

              printf("[RESTORE] Sistema restaurado para a versao do backup\n");

              //imprimeProgramas(teste, teste);
              //printf("tamanhoPreOrder: %d\n", arvore.tamanhoPreOrder);
              //printf("tamanhoInOrder: %d \n", arvore.tamanhoInOrder);

              //imprimeProgramasDaArvore(&novaArvore);

              //arvore = novaArvore;
			} break;
			default:{
              imprimeProgramasDaArvore(&arvore);
			}
		}
	}
}