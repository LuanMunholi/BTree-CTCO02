#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "btree.h"


BTreeNode* criaNo(int ordem, int folha) {
    BTreeNode *novoNo = (BTreeNode*)malloc(sizeof(BTreeNode));
    novoNo->chaves = (ArquivoIndex*)malloc((2 * ordem - 1) * sizeof(ArquivoIndex));
    novoNo->filhos = (BTreeNode**)malloc(2 * ordem * sizeof(BTreeNode*));
    novoNo->numChaves = 0;
    novoNo->folha = folha;
    return novoNo;
}

BTree* criaBTree(int ordem) {
    BTree *novaArvore = (BTree*)malloc(sizeof(BTree));
    novaArvore->ordem = ordem;
    novaArvore->raiz = criaNo(ordem, 1);
    return novaArvore;
}

BTreeNode* buscaNaBtree(BTreeNode *no, const char *chave, int *indice) {
    int i = 0;
    while (i < no->numChaves && strcmp(chave, no->chaves[i].chave) > 0) {
        i++;
    }

    if (i < no->numChaves && strcmp(chave, no->chaves[i].chave) == 0) {
        *indice = i;
        return no;
    }

    if (no->folha) {
        return NULL;
    }

    return buscaNaBtree(no->filhos[i], chave, indice);
}

void divideNo(BTreeNode *pai, int i, BTreeNode *cheio, int ordem) {
    BTreeNode *novoNo = criaNo(ordem, cheio->folha);
    novoNo->numChaves = ordem - 1;

    for (int j = 0; j < ordem - 1; j++) {
        strcpy(novoNo->chaves[j].chave, cheio->chaves[j + ordem].chave);
        novoNo->chaves[j].linha = cheio->chaves[j + ordem].linha;
    }

    if (!cheio->folha) {
        for (int j = 0; j < ordem; j++) {
            novoNo->filhos[j] = cheio->filhos[j + ordem];
        }
    }

    cheio->numChaves = ordem - 1;

    for (int j = pai->numChaves; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[i + 1] = novoNo;

    for (int j = pai->numChaves - 1; j >= i; j--) {
        strcpy(pai->chaves[j + 1].chave, pai->chaves[j].chave);
        pai->chaves[j + 1].linha = pai->chaves[j].linha;
    }

    strcpy(pai->chaves[i].chave, cheio->chaves[ordem - 1].chave);
    pai->chaves[i].linha = cheio->chaves[ordem - 1].linha;
    pai->numChaves++;
}

void insereNaoCheio(BTreeNode *no, ArquivoIndex *chave, int ordem) {
    int i = no->numChaves - 1;

    if (no->folha) {
        while (i >= 0 && strcmp(chave->chave, no->chaves[i].chave) < 0) {
            strcpy(no->chaves[i + 1].chave, no->chaves[i].chave);
            no->chaves[i + 1].linha = no->chaves[i].linha;
            i--;
        }
        strcpy(no->chaves[i + 1].chave, chave->chave);
        no->chaves[i + 1].linha = chave->linha;
        no->numChaves++;
    } else {
        while (i >= 0 && strcmp(chave->chave, no->chaves[i].chave) < 0) {
            i--;
        }
        i++;
        if (no->filhos[i]->numChaves == 2 * ordem - 1) {
            divideNo(no, i, no->filhos[i], ordem);
            if (strcmp(chave->chave, no->chaves[i].chave) > 0) {
                i++;
            }
        }
        insereNaoCheio(no->filhos[i], chave, ordem);
    }
}

void insere(BTree *arvore, ArquivoIndex *chave) {
    BTreeNode *raiz = arvore->raiz;

    if (raiz->numChaves == 2 * arvore->ordem - 1) {
        BTreeNode *novoNo = criaNo(arvore->ordem, 0);
        novoNo->filhos[0] = raiz;
        divideNo(novoNo, 0, raiz, arvore->ordem);
        insereNaoCheio(novoNo, chave, arvore->ordem);
        arvore->raiz = novoNo;
    } else {
        insereNaoCheio(raiz, chave, arvore->ordem);
    }
}

void insereDoArquivo(BTree *arvore, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Não foi possível abrir o arquivo");
        return;
    }

    char linha[256];
    int numeroLinha = 1;
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Extrai os 5 primeiros caracteres como a chave
        char chaveStr[6];
        strncpy(chaveStr, linha, 5);
        chaveStr[5] = '\0'; // Adiciona o caractere nulo ao final

        // Cria um ArquivoIndex com a chave e a linha
        ArquivoIndex *novoIndex = (ArquivoIndex*)malloc(sizeof(ArquivoIndex));
        strcpy(novoIndex->chave, chaveStr);
        novoIndex->linha = numeroLinha;

        // Insere na B-tree
        insere(arvore, novoIndex);

        numeroLinha++;
    }

    fclose(arquivo);
}

void imprimeEspacos(int numEspacos) {
    for (int i = 0; i < numEspacos; i++) {
        printf(" ");
    }
}

void imprimeChaves(BTreeNode *no) {
    for (int i = 0; i < no->numChaves; i++) {
        printf("%s ", no->chaves[i].chave);
    }
}

void imprimeBTreeRecursivo(BTreeNode *no, int nivel, int ordem) {
    if (no != NULL) {
        int i;
        for (i = 0; i < no->numChaves; i++) {
            if (!no->folha) {
                imprimeBTreeRecursivo(no->filhos[i], nivel + 1, ordem);
            }
            imprimeEspacos(nivel * 10);
            printf("%s\n", no->chaves[i].chave);
        }
        if (!no->folha) {
            imprimeBTreeRecursivo(no->filhos[i], nivel + 1, ordem);
        }
    }
}

void imprimeBTree(BTree *arvore) {
    imprimeBTreeRecursivo(arvore->raiz, 0, arvore->ordem);
}