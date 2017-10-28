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
void percorrerNoParaExcluir(No* no, int cod);
void excluirNoFolha(Arvore* arvore, int cod);

int iniciarProcesso(No* no, Processo processo); //testado com o teste 1 do susy
void finalizarProcesso(Arvore* arvore, int cod);
int calcularFragmentacao(No* no);  ///começado
void relatorioSistema(Arvore* arvore);
void imprimirSementesGeradoras(Arvore* arvore);
void imprimirProcessos(Arvore* arvore);
int pot(int base, int expoente); //funcionando
void imprimeArvore(No* no, char* espaco); //feito mas printa meio bizarro



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
	  	int cod, size;

	  	scanf("%d", &cod);
	  	scanf("%d", &size);

	  	Processo processo;
	  	processo.codigo = cod;
	  	processo.tamanho = size;

	  	int resp = iniciarProcesso(arvore.raiz, processo);

	  	if(resp == 1){
	  		printf("Processo (%d) de tamanho %d inicializado com sucesso\n", cod, size);
	  	}else{
	  		printf("Memoria insuficiente\n");
	  	}

	  } break;
	  case 2:{

	  } break;
	  case 3:{
	  		int fragmentacao = calcularFragmentacao(arvore.raiz);
	  		printf("Quantidade total de memoria desperdicada por fragmentacao: %d\n",fragmentacao);

	  } break;
	  case 4:{

	  } break;
	  case 5:{

	  } break;
	  case 6:{
	  	char* espaco = malloc(sizeof(char*));
	  	espaco = strcat(espaco, "  ");
	  	imprimeArvore(arvore.raiz, espaco);
	  }break;
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
		*(no->memoria.processo) = processo;

		no->memoria.fragmentacao = pot(2, no->memoria.valor) - processo.tamanho;
		no->memoria.estado = OCUPADO;
		return 1;
	}

	if(no->esq == NULL && no->dir == NULL){
		no->memoria.estado = PARTICIONADO;
	}

	if(no->esq == NULL){
		no->esq = (No*)malloc(sizeof(No));
		inicializarNo(no->esq, no->memoria.valor-1);
	}

	if(no->dir == NULL){
		no->dir = (No*)malloc(sizeof(No));
		inicializarNo(no->dir, no->memoria.valor-1);
	}

	int esqLivre = iniciarProcesso(no->esq, processo);
	int dirLivre;

	if(esqLivre == 0){
		dirLivre = iniciarProcesso(no->dir, processo);

		if(dirLivre==0){
			return 0;
		}
	}

	return 1;
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

void inicializarNo(No* no, int valor){
	//printf("inicializarNo...\n");
  no->esq = NULL;
  no->dir = NULL;
  no->memoria.valor = valor;
  no->memoria.estado = LIVRE;
  no->memoria.fragmentacao = 0;
  no->memoria.processo = malloc(sizeof(Processo));
} 

void inicializarArvore(Arvore* arvore, int valor){
  arvore->raiz = (No*)malloc(sizeof(No));
  inicializarNo(&(*arvore->raiz),valor);
}

void percorrerNoParaExcluir(No* no, int cod){
  if (no->esq == NULL && no->dir == NULL && no->memoria.processo->codigo == cod)
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
