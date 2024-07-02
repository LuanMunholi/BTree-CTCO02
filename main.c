#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "btree.h"
#include "busca_direta.h"

#define NUM_BUSCAS 30
#define CHAVE_TAMANHO 6
#define MAX_CHAVE 99999

// Função para medir o tempo de execução
double medirTempoBusca(int (*func)(const char *, const char *), const char *nomeArquivo, const char *chave) {
    clock_t inicio, fim;
    inicio = clock();
    func(nomeArquivo, chave);
    fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

void realizarBuscas(const char *nomeArquivo, BTree *arvore) {
    double temposDireto[NUM_BUSCAS];
    double temposBTree[NUM_BUSCAS];
    double somaDireto = 0.0;
    double somaBTree = 0.0;
    double maxDireto = 0.0;
    double minDireto = 1.0;
    double maxBTree = 0.0;
    double minBTree = 1.0;

    srand(time(NULL));

    for (int i = 0; i < NUM_BUSCAS; i++) {
        char chave[CHAVE_TAMANHO];
        sprintf(chave, "%05d", rand() % MAX_CHAVE + 1);

        temposDireto[i] = medirTempoBusca(buscaDiretaNoArquivo, nomeArquivo, chave);
        temposBTree[i] = medirTempoBusca((int (*)(const char *, const char *))buscaNaBTree, (const char *)arvore, chave);

        somaDireto += temposDireto[i];
        somaBTree += temposBTree[i];

        if (temposDireto[i] > maxDireto) maxDireto = temposDireto[i];
        if (temposDireto[i] < minDireto) minDireto = temposDireto[i];

        if (temposBTree[i] > maxBTree) maxBTree = temposBTree[i];
        if (temposBTree[i] < minBTree) minBTree = temposBTree[i];
    }

    double mediaDireto = somaDireto / NUM_BUSCAS;
    double mediaBTree = somaBTree / NUM_BUSCAS;

    printf("Busca Direta:\n");
    printf("Média: %f segundos\n", mediaDireto);
    printf("Máximo: %f segundos\n", maxDireto);
    printf("Mínimo: %f segundos\n", minDireto);

    printf("\nBusca B-Tree:\n");
    printf("Média: %f segundos\n", mediaBTree);
    printf("Máximo: %f segundos\n", maxBTree);
    printf("Mínimo: %f segundos\n", minBTree);
}

int main() {
    const char *nomeArquivo = "dados.txt";

    // Criar B-Tree e carregar o arquivo (implementação de exemplo, ajuste conforme necessário)
    BTree *arvore = criaBTree();
    int count;
    leArquivo(nomeArquivo, &count);

    // Realizar buscas e medir o tempo
    realizarBuscas(nomeArquivo, arvore);

    // Liberar memória da B-Tree (implementar função de destruição conforme necessário)
    // destruirBTree(arvore);

    return 0;
}