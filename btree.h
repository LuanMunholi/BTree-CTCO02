#ifndef TRAB_2_BTREE_H
#define TRAB_2_BTREE_H

typedef struct ArquivoIndex {
    char chave[6]; // Utilizamos uma string para armazenar os 5 caracteres da chave
    int linha;
} ArquivoIndex;

typedef struct BTreeNode {
    int numChaves;
    ArquivoIndex *chaves;
    struct BTreeNode **filhos;
    int folha;
} BTreeNode;

typedef struct BTree {
    BTreeNode *raiz;
    int ordem;
} BTree;

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

#endif // TRAB_2_BTREE_H