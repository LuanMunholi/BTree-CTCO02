#ifndef TRAB_2_BTREE_H
#define TRAB_2_BTREE_H

#include <stdbool.h>

#define T 3  // Define a ordem mínima da árvore B (T > 1)

typedef struct ArquivoIndex ArquivoIndex;
typedef struct noArvore noArvore;
typedef struct BTree BTree;

// Funções para manipulação da árvore B
noArvore* criaNo(bool folha);
BTree* criaBTree();
void divideFilho(noArvore *no, int i, noArvore *noFilho);
void insereNaoCheio(noArvore *no, ArquivoIndex *chave);
void insere(BTree *arvore, ArquivoIndex *chave);
ArquivoIndex* busca(noArvore *no, int chave);
void leArquivo(const char *nomeArquivo, BTree *arvore);
int buscaNaBTree(BTree *arvore, const char *chave);  // Adicione este protótipo

#endif // TRAB_2_BTREE_H