#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "btree.h"

// Estrutura para armazenar o índice e a linha no arquivo
typedef struct ArquivoIndex {
    int chave;
    int linha;
} ArquivoIndex;

// Estrutura de um nó da árvore B
typedef struct noArvore {
    ArquivoIndex* chaves[2 * T - 1];  // Array de ponteiros para ArquivoIndex
    struct noArvore* filhos[2 * T];   // Array de ponteiros para filhos
    int numChaves;                    // Número de chaves armazenadas no nó
    bool folha;                       // Indica se o nó é uma folha
} noArvore;

// Estrutura da árvore B
typedef struct BTree {
    noArvore* raiz;
} BTree;

// Cria um novo nó da árvore B
noArvore* criaNo(bool folha) {
    noArvore* no = (noArvore*)malloc(sizeof(noArvore));
    if (no == NULL) {
        perror("Erro ao alocar memória para novo nó");
        exit(EXIT_FAILURE);
    }

    no->folha = folha;
    no->numChaves = 0;
    for (int i = 0; i < 2 * T - 1; i++) {
        no->chaves[i] = NULL;
    }
    for (int i = 0; i < 2 * T; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

// Cria uma nova árvore B
BTree* criaBTree() {
    BTree* arvore = (BTree*)malloc(sizeof(BTree));
    if (arvore == NULL) {
        perror("Erro ao alocar memória para nova B-Tree");
        exit(EXIT_FAILURE);
    }

    arvore->raiz = criaNo(true);
    return arvore;
}

// Divide o filho noFilho de no na posição i
void divideFilho(noArvore *no, int i, noArvore *noFilho) {
    noArvore* novoNo = criaNo(noFilho->folha);
    novoNo->numChaves = T - 1;

    // Copia as chaves do noFilho para o novoNo
    for (int j = 0; j < T - 1; j++) {
        novoNo->chaves[j] = noFilho->chaves[j + T];
    }

    // Se noFilho não for folha, copia os filhos
    if (!noFilho->folha) {
        for (int j = 0; j < T; j++) {
            novoNo->filhos[j] = noFilho->filhos[j + T];
        }
    }

    noFilho->numChaves = T - 1;

    // Cria espaço para o novo filho em no
    for (int j = no->numChaves; j >= i + 1; j--) {
        no->filhos[j + 1] = no->filhos[j];
    }

    no->filhos[i + 1] = novoNo;

    // Move as chaves em no para dar espaço para a nova chave
    for (int j = no->numChaves - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
    }

    no->chaves[i] = noFilho->chaves[T - 1];
    no->numChaves++;
}

// Insere uma chave em um nó que não está cheio
void insereNaoCheio(noArvore *no, ArquivoIndex *chave) {
    int i = no->numChaves - 1;

    if (no->folha) {
        // Move as chaves maiores para frente
        while (i >= 0 && chave->chave < no->chaves[i]->chave) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }

        no->chaves[i + 1] = chave;
        no->numChaves++;
    } else {
        // Encontra o filho que receberá a nova chave
        while (i >= 0 && chave->chave < no->chaves[i]->chave) {
            i--;
        }
        i++;

        // Se o filho está cheio, divida-o
        if (no->filhos[i]->numChaves == 2 * T - 1) {
            divideFilho(no, i, no->filhos[i]);

            // Depois de dividir, a nova chave está em no->chaves[i]
            if (chave->chave > no->chaves[i]->chave) {
                i++;
            }
        }

        insereNaoCheio(no->filhos[i], chave);
    }
}

// Insere uma chave na árvore B
void insere(BTree *arvore, ArquivoIndex *chave) {
    noArvore* raiz = arvore->raiz;

    if (raiz->numChaves == 2 * T - 1) {
        noArvore* novoNo = criaNo(false);
        arvore->raiz = novoNo;
        novoNo->filhos[0] = raiz;

        divideFilho(novoNo, 0, raiz);

        insereNaoCheio(novoNo, chave);
    } else {
        insereNaoCheio(raiz, chave);
    }
}

// Busca uma chave na árvore B
ArquivoIndex* busca(noArvore *no, int chave) {
    int i = 0;
    while (i < no->numChaves && chave > no->chaves[i]->chave) {
        i++;
    }

    if (i < no->numChaves && chave == no->chaves[i]->chave) {
        return no->chaves[i];
    }

    if (no->folha) {
        return NULL;
    } else {
        return busca(no->filhos[i], chave);
    }
}

// Lê o arquivo e insere os registros na B-Tree
void leArquivo(const char *nomeArquivo, BTree *arvore) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int chave, linha;
    while (fscanf(arquivo, "%d %d", &chave, &linha) == 2) {
        ArquivoIndex *indice = (ArquivoIndex *)malloc(sizeof(ArquivoIndex));
        if (indice == NULL) {
            perror("Erro ao alocar memória");
            exit(EXIT_FAILURE);
        }

        indice->chave = chave;
        indice->linha = linha;

        insere(arvore, indice); // Inserir na B-Tree

        free(indice);
    }

    fclose(arquivo);
    printf("Índice criado com sucesso a partir do arquivo %s.\n", nomeArquivo);
}

// Função para buscar uma chave utilizando a B-Tree
int buscaNaBTree(BTree *arvore, const char *chave) {
    int chaveInt = atoi(chave);  // Convertendo a chave para inteiro
    ArquivoIndex *resultado = busca(arvore->raiz, chaveInt);

    return resultado != NULL ? 1 : 0;  // Retorna 1 se a chave foi encontrada, caso contrário, 0
}