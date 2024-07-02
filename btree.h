#ifndef TRAB_2_BTREE_H
#define TRAB_2_BTREE_H

#include <stdbool.h>

#define T 3  // Define a ordem mínima da árvore B (T > 1)

// Estrutura para armazenar o índice e a linha no arquivo
typedef struct {
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
typedef struct {
    noArvore* raiz;
} BTree;

// Funções para manipulação da árvore B
noArvore* criaNo(bool folha);
BTree* criaBTree();
void divideFilho(noArvore *no, int i, noArvore *noFilho);
void insereNaoCheio(noArvore *no, ArquivoIndex *chave);
void insere(BTree *arvore, ArquivoIndex *chave);
ArquivoIndex* busca(noArvore *no, int chave);
void leTodosIndices(const char *nomeArquivo);
void leArquivo(const char *nomeArquivo, int *count);

#endif // TRAB_2_BTREE_H