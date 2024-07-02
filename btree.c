#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "btree.h"

// Cria um novo nó da árvore B
noArvore* criaNo(bool folha) {
    noArvore* no = (noArvore*)malloc(sizeof(noArvore));
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

// Lê todos os índices de um arquivo (simulação)
void leTodosIndices(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        printf("%s", linha);
    }

    fclose(arquivo);
}

// Lê o arquivo e conta os registros (simulação)
void leArquivo(const char *nomeArquivo, int *count) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    *count = 0;
    while (fgets(linha, sizeof(linha), arquivo)) {
        (*count)++;
    }

    fclose(arquivo);
}
