#ifndef TRAB_2_BTREE_H
#define TRAB_2_BTREE_H
#define MAX_NAME_LENGTH 21
#define T 2

// Estrutura que simulara um registro do .txt
typedef struct Registro{
  int index;
  char nome[MAX_NAME_LENGTH];
  int id;
  int isActive;
} Registro;

// Estrutura ArquivoIndex
typedef struct ArquivoIndex{
  int index;
  int line;
} ArquivoIndex;

// Estrutura de um no da arvore B
typedef struct noArvore {
  ArquivoIndex* arquivoIndex[2*T-1];    //array de ponteiros para ArquivoIndex
  struct noArvore* filho[2*T];          //array de ponteiros para filhos
  int n;                                //numero de chaves armazenadas no nó
  bool folha;                           //indica se o nó é uma folha
} noArvore;

// Estrutura da árvore B
typedef struct BTree{
  noArvore* raiz;
} BTree;

struct noArvore* criaNo(bool folha);
struct BTree* criaBTree();
void divideFilho(noArvore *no, int i, noArvore *noFilho);
void insereNaoCheio(noArvore *no, ArquivoIndex *chave);
void insere(BTree *tree, ArquivoIndex *chave);
struct ArquivoIndex* busca(noArvore *no, int chave);
void leTodosIndices(const char *nomeArquivo);
void leArquivo(const char *nomeArquivo, Registro **registros, int *count);

#endif //TRAB_2_BTREE_H