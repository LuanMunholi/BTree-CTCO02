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

void removeChaveRecursivo(BTreeNode *no, const char *chave, int ordem) {
    int indice = 0;
    while (indice < no->numChaves && strcmp(chave, no->chaves[indice].chave) > 0) {
        indice++;
    }

    if (indice < no->numChaves && strcmp(chave, no->chaves[indice].chave) == 0) {
        // Caso 1: A chave está presente no nó folha
        if (no->folha) {
            // Move todas as chaves após a chave removida uma posição para trás
            for (int i = indice; i < no->numChaves - 1; i++) {
                strcpy(no->chaves[i].chave, no->chaves[i + 1].chave);
                no->chaves[i].linha = no->chaves[i + 1].linha;
            }
            no->numChaves--;
        } else {
            // Caso 2: A chave está presente em um nó não folha
            // Encontra o nó sucessor que contém a chave a ser removida
            BTreeNode *sucessor = no->filhos[indice + 1];
            while (!sucessor->folha) {
                sucessor = sucessor->filhos[0];
            }

            // Troca a chave a ser removida pelo sucessor
            strcpy(no->chaves[indice].chave, sucessor->chaves[0].chave);
            no->chaves[indice].linha = sucessor->chaves[0].linha;

            // Remove a chave do sucessor recursivamente
            removeChaveRecursivo(sucessor, chave, ordem);
        }
    } else {
        // Caso 3: A chave não está presente neste nó
        if (no->folha) {
            printf("Chave '%s' não encontrada na árvore.\n", chave);
            return;
        }

        // Decide qual filho onde a chave deve estar
        int filho = indice;
        if (indice < no->numChaves && strcmp(chave, no->chaves[indice].chave) > 0) {
            filho = indice + 1;
        }

        // Verifica se o filho precisa ser reestruturado antes de prosseguir
        if (no->filhos[filho]->numChaves < ordem) {
            // Caso 4a: Combinação do filho com o seu irmão esquerdo
            if (filho > 0 && no->filhos[filho - 1]->numChaves >= ordem) {
                BTreeNode *filhoEsq = no->filhos[filho - 1];
                BTreeNode *filhoAtual = no->filhos[filho];

                // Move todas as chaves do filhoAtual uma posição para a frente
                for (int i = filhoAtual->numChaves - 1; i >= 0; i--) {
                    strcpy(filhoAtual->chaves[i + 1].chave, filhoAtual->chaves[i].chave);
                    filhoAtual->chaves[i + 1].linha = filhoAtual->chaves[i].linha;
                }

                // Move o último filho do filhoEsq como o primeiro filho do filhoAtual
                if (!filhoAtual->folha) {
                    for (int i = filhoAtual->numChaves; i >= 0; i--) {
                        filhoAtual->filhos[i + 1] = filhoAtual->filhos[i];
                    }
                    filhoAtual->filhos[0] = filhoEsq->filhos[filhoEsq->numChaves];
                }

                // Move a chave do pai para o filhoAtual
                strcpy(filhoAtual->chaves[0].chave, no->chaves[filho - 1].chave);
                filhoAtual->chaves[0].linha = no->chaves[filho - 1].linha;

                // Move a chave do filhoEsq para o pai
                strcpy(no->chaves[filho - 1].chave, filhoEsq->chaves[filhoEsq->numChaves - 1].chave);
                no->chaves[filho - 1].linha = filhoEsq->chaves[filhoEsq->numChaves - 1].linha;

                // Atualiza o número de chaves
                filhoAtual->numChaves++;
                filhoEsq->numChaves--;
            }
            // Caso 4b: Combinação do filho com o seu irmão direito
            else if (filho < no->numChaves && no->filhos[filho + 1]->numChaves >= ordem) {
                BTreeNode *filhoAtual = no->filhos[filho];
                BTreeNode *filhoDir = no->filhos[filho + 1];

                // Move a chave do pai para o filhoAtual
                strcpy(filhoAtual->chaves[filhoAtual->numChaves].chave, no->chaves[filho].chave);
                filhoAtual->chaves[filhoAtual->numChaves].linha = no->chaves[filho].linha;
                filhoAtual->numChaves++;

                // Move a primeira chave do filhoDir para o pai
                strcpy(no->chaves[filho].chave, filhoDir->chaves[0].chave);
                no->chaves[filho].linha = filhoDir->chaves[0].linha;

                // Move o primeiro filho do filhoDir para o último filho do filhoAtual
                if (!filhoAtual->folha) {
                    filhoAtual->filhos[filhoAtual->numChaves] = filhoDir->filhos[0];
                }

                // Move todas as chaves do filhoDir uma posição para a esquerda
                for (int i = 1; i < filhoDir->numChaves; i++) {
                    strcpy(filhoDir->chaves[i - 1].chave, filhoDir->chaves[i].chave);
                    filhoDir->chaves[i - 1].linha = filhoDir->chaves[i].linha;
                }

                // Move todos os filhos do filhoDir uma posição para a esquerda
                if (!filhoDir->folha) {
                    for (int i = 1; i <= filhoDir->numChaves; i++) {
                        filhoDir->filhos[i - 1] = filhoDir->filhos[i];
                    }
                }

                // Atualiza o número de chaves
                filhoDir->numChaves--;
            }
            // Caso 4c: Fusão dos filhos
            else {
                BTreeNode *filhoAtual = no->filhos[filho];

                // Caso 4c.1: Fusão com o filho à direita
                if (filho < no->numChaves) {
                    BTreeNode *filhoDir = no->filhos[filho + 1];

                    // Move a chave do pai para o filhoAtual
                    strcpy(filhoAtual->chaves[filhoAtual->numChaves].chave, no->chaves[filho].chave);
                    filhoAtual->chaves[filhoAtual->numChaves].linha = no->chaves[filho].linha;
                    filhoAtual->numChaves++;

                    // Move todas as chaves do filhoDir para o filhoAtual
                    for (int i = 0; i < filhoDir->numChaves; i++) {
                        strcpy(filhoAtual->chaves[filhoAtual->numChaves].chave, filhoDir->chaves[i].chave);
                        filhoAtual->chaves[filhoAtual->numChaves].linha = filhoDir->chaves[i].linha;
                        filhoAtual->numChaves++;
                    }

                    // Move todos os filhos do filhoDir para o filhoAtual
                    if (!filhoDir->folha) {
                        for (int i = 0; i <= filhoDir->numChaves; i++) {
                            filhoAtual->filhos[filhoAtual->numChaves] = filhoDir->filhos[i];
                            filhoAtual->numChaves++;
                        }
                    }

                    // Libera o espaço ocupado pelo filhoDir
                    free(filhoDir);

                    // Move todas as chaves do pai uma posição para a esquerda
                    for (int i = filho + 1; i < no->numChaves; i++) {
                        strcpy(no->chaves[i - 1].chave, no->chaves[i].chave);
                        no->chaves[i - 1].linha = no->chaves[i].linha;
                    }

                    // Move todos os filhos do pai uma posição para a esquerda
                    for (int i = filho + 2; i <= no->numChaves; i++) {
                        no->filhos[i - 1] = no->filhos[i];
                    }

                    // Atualiza o número de chaves do pai
                    no->numChaves--;
                }
                // Caso 4c.2: Fusão com o filho à esquerda
                else {
                    BTreeNode *filhoEsq = no->filhos[filho - 1];

                    // Move a chave do pai para o filhoEsq
                    strcpy(filhoEsq->chaves[filhoEsq->numChaves].chave, no->chaves[filho - 1].chave);
                    filhoEsq->chaves[filhoEsq->numChaves].linha = no->chaves[filho - 1].linha;
                    filhoEsq->numChaves++;

                    // Move todas as chaves do filhoAtual para o filhoEsq
                    for (int i = 0; i < filhoAtual->numChaves; i++) {
                        strcpy(filhoEsq->chaves[filhoEsq->numChaves].chave, filhoAtual->chaves[i].chave);
                        filhoEsq->chaves[filhoEsq->numChaves].linha = filhoAtual->chaves[i].linha;
                        filhoEsq->numChaves++;
                    }

                    // Move todos os filhos do filhoAtual para o filhoEsq
                    if (!filhoAtual->folha) {
                        for (int i = 0; i <= filhoAtual->numChaves; i++) {
                            filhoEsq->filhos[filhoEsq->numChaves] = filhoAtual->filhos[i];
                            filhoEsq->numChaves++;
                        }
                    }

                    // Libera o espaço ocupado pelo filhoAtual
                    free(filhoAtual);

                    // Move todas as chaves do pai uma posição para a esquerda
                    for (int i = filho; i < no->numChaves - 1; i++) {
                        strcpy(no->chaves[i].chave, no->chaves[i + 1].chave);
                        no->chaves[i].linha = no->chaves[i + 1].linha;
                    }

                    // Move todos os filhos do pai uma posição para a esquerda
                    for (int i = filho + 1; i <= no->numChaves; i++) {
                        no->filhos[i - 1] = no->filhos[i];
                    }

                    // Atualiza o número de chaves do pai
                    no->numChaves--;
                }
            }
        }

        // Chama recursivamente a remoção na subárvore apropriada
        removeChaveRecursivo(no->filhos[filho], chave, ordem);
    }
}

void removeChave(BTree *arvore, const char *chave) {
    removeChaveRecursivo(arvore->raiz, chave, arvore->ordem);
}