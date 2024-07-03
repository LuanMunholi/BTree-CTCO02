#ifndef TRAB_2_BTREE_H
#define TRAB_2_BTREE_H

typedef struct ArquivoIndex ArquivoIndex;

typedef struct BTreeNode BTreeNode;

typedef struct BTree BTree;

BTreeNode* criaNo(int ordem, int folha);

BTree* criaBTree(int ordem);

BTreeNode* buscaNaBtree(BTreeNode *no, const char *chave, int *indice);

void divideNo(BTreeNode *pai, int i, BTreeNode *cheio, int ordem);

void insereNaoCheio(BTreeNode *no, ArquivoIndex *chave, int ordem);

void insere(BTree *arvore, ArquivoIndex *chave);

void insereDoArquivo(BTree *arvore, const char *nomeArquivo);

void imprimeEspacos(int numEspacos);

void imprimeChaves(BTreeNode *no);

void imprimeBTreeRecursivo(BTreeNode *no, int nivel, int ordem);

void imprimeBTree(BTree *arvore);

void removeChaveRecursivo(BTreeNode *no, const char *chave, int ordem);

void removeChave(BTree *arvore, const char *chave);

BTreeNode* getRaiz(BTree *arvore);

int getLinha(BTreeNode *no, int indice);

#endif // TRAB_2_BTREE_H