#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

//Grau minimo (a ordem da arvore eh 2T - 1)
#define T 2 
//tamanho do nome do .txt: 20 characters + 1 for the null terminator
#define MAX_NAME_LENGTH 21

//criar um novo no da arvoire B
struct noArvore *criaNo(bool folha) {
  noArvore *novoNo = (noArvore *)malloc(sizeof(noArvore));
  novoNo->folha = folha;
  novoNo->n = 0;
  for (int i = 0; i < 2 * T; i++) {
    novoNo->filho[i] = NULL;
  }
  return novoNo;
}

//criar uma nova arvore B
BTree *criaBTree() {
  BTree *tree = (BTree*)malloc(sizeof(BTree));
  tree->raiz = criaNo(true);
  return tree;
}

//dividir o filho y do no x em dois nos
void divideFilho(noArvore *no, int i, noArvore *noFilho) {
  noArvore *novoNo = criaNo(noFilho->folha);
  novoNo->n = T - 1;

  for (int j = 0; j < T - 1; j++) {
    novoNo->arquivoIndex[j] = noFilho->arquivoIndex[j + T];
  }

  if (!noFilho->folha) {
    for (int j = 0; j < T; j++) {
      novoNo->filho[j] = noFilho->filho[j + T];
    }
  }

  noFilho->n = T - 1;

  for (int j = no->n; j >= i + 1; j--) {
    no->filho[j + 1] = no->filho[j];
  }

  no->filho[i + 1] = novoNo;

  for (int j = no->n - 1; j >= i; j--) {
    no->arquivoIndex[j + 1] = no->arquivoIndex[j];
  }

  no->arquivoIndex[i] = noFilho->arquivoIndex[T - 1];
  no->n += 1;
}

//inserir uma nova chave no nó nao cheio
void insereNaoCheio(noArvore *no, ArquivoIndex *chave) {
  int i = no->n - 1;

  if (no->folha) {
    while (i >= 0 && chave->index < no->arquivoIndex[i]->index) {
      no->arquivoIndex[i + 1] = no->arquivoIndex[i];
      i--;
    }

    no->arquivoIndex[i + 1] = chave;
    no->n += 1;
  } else {
    while (i >= 0 && chave->index < no->arquivoIndex[i]->index) {
      i--;
    }
    i++;

    if (no->filho[i]->n == 2 * T - 1) {
      divideFilho(no, i, no->filho[i]);

      if (chave->index > no->arquivoIndex[i]->index) {
        i++;
      }
    }
    insereNaoCheio(no->filho[i], chave);
  }
}

//inserir uma nova chave na arvore B
void insere(BTree *tree, ArquivoIndex *chave) {
  noArvore *r = tree->raiz;

  if (r->n == 2 * T - 1) {
    noArvore *novoNo = criaNo(false);
    tree->raiz = novoNo;
    novoNo->filho[0] = r;
    divideFilho(novoNo, 0, r);
    insereNaoCheio(novoNo, chave);
  } else {
    insereNaoCheio(r, chave);
  }
}

//buscar uma chave na arvore B
ArquivoIndex *busca(noArvore *no, int chave) {
  int i = 0;

  while (i < no->n && chave > no->arquivoIndex[i]->index) {
    i++;
  }

  if (i < no->n && chave == no->arquivoIndex[i]->index) {
    return no->arquivoIndex[i];
  }

  if (no->folha) {
    return NULL;
  } else {
    return busca(no->filho[i], chave);
  }
}

//funcao para ler todos inteiros do index (primeiro item de cada linha)
void leTodosIndices(const char *nomeArquivo) {
  FILE *file = fopen(nomeArquivo, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    exit(EXIT_FAILURE);
  }

  int number;
  char line[256];
  int line_number = 1;

  // Leitura linha por linha
  while (fgets(line, sizeof(line), file)) {
    // Tenta ler um número inteiro no início da linha
    if (sscanf(line, "%d", &number) == 1) {
      printf("Linha %d: %d\n", line_number, number);
    }
    line_number++;
  }

  fclose(file);
}

//carrega os dados do arquivo para a memória RAM através de um vetor auxiliar chamado Registros
void leArquivo(const char *nomeArquivo, Registro **registros, int *count) {
  FILE *file = fopen(nomeArquivo, "r");
  if (file == NULL) {
    perror("Erro ao abrir arquivo");
    exit(EXIT_FAILURE);
  }

  // Tamanho inicial dos arrays de registro
  int size = 10;
  *registros = (Registro *)malloc(size * sizeof(Registro));
  if (*registros == NULL) {
    perror("Erro ao alocal memória");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  *count = 0;
  while (fscanf(file, "%d %20s %d %d", &(*registros)[*count].index,
                (*registros)[*count].nome, &(*registros)[*count].id,
                &(*registros)[*count].isActive) == 4) {
    (*count)++;

    // Realoca memória, se necessário
    if (*count >= size) {
      size *= 2;
      *registros = (Registro *)realloc(*registros, size * sizeof(Registro));
      if (*registros == NULL) {
        perror("Error reallocating memory");
        fclose(file);
        exit(EXIT_FAILURE);
      }
    }
  }

  fclose(file);
}
