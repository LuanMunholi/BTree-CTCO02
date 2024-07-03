#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "btree.c"
#include "busca_direta.h"

#define CHAVE_TAMANHO 6     // Definição do tamanho máximo para a chave
#define MAX_CHAVE 99999     // Definição do valor máximo para a chave
#define NUM_REGISTRO 10000  // Definição do valor máximo de registros
#define NUM_BUSCAS 30       // Definição do número de buscas realizadas no teste

// Função para medir o tempo de execução de uma busca direta no arquivo
double medirTempoBusca(int (*func)(const char *, const char *), const char *nomeArquivo, const char *chave) {
    clock_t inicio, fim;
    inicio = clock();
    func(nomeArquivo, chave);
    fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

// Função para medir o tempo de execução de uma busca na B-Tree
double medirTempoBuscaBTree(int (*func)(BTree *, const char *), BTree *arvore, const char *chave) {
    clock_t inicio, fim;
    inicio = clock();
    func(arvore, chave);
    fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

void criarIndice(BTree *arvore, const char *nomeArquivo) {
    int count;
    leArquivo(nomeArquivo, &count);
    printf("Índice criado com sucesso.\n");
}

void procurarElemento(BTree *arvore, const char *nomeArquivo) {
    char chave[CHAVE_TAMANHO];
    printf("Digite a chave a ser buscada: ");
    scanf("%s", chave);

    // Busca na B-Tree
    int encontradoBTree = buscaNaBTree(arvore, chave);
    if (encontradoBTree) {
        printf("Chave %s encontrada na B-Tree.\n", chave);
    } else {
        printf("Chave %s não encontrada na B-Tree.\n", chave);
    }

    // Busca direta no arquivo
    int encontradoDireto = buscaDiretaNoArquivo(nomeArquivo, chave);
    if (encontradoDireto) {
        printf("Chave %s encontrada no arquivo.\n", chave);
    } else {
        printf("Chave %s não encontrada no arquivo.\n", chave);
    }
}

void removerRegistro(BTree *arvore) {
    printf("Remoção de registro ainda não implementada.\n");
}

void realizarBuscas(const char *nomeArquivo, BTree *arvore) {
    double temposDireto[NUM_BUSCAS];
    double temposBTree[NUM_BUSCAS];
    double somaDireto = 0.0, somaBTree = 0.0;
    double maxDireto = 0.0, minDireto = 1.0;
    double maxBTree = 0.0, minBTree = 1.0;

    srand(time(NULL));

    for (int i = 0; i < NUM_BUSCAS; i++) {
        char chave[CHAVE_TAMANHO];
        sprintf(chave, "%05d", rand() % MAX_CHAVE + 1);

        temposDireto[i] = medirTempoBusca(buscaDiretaNoArquivo, nomeArquivo, chave);
        temposBTree[i] = medirTempoBuscaBTree(buscaNaBTree, arvore, chave);

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
    BTree *arvore = criaBTree();

    int opcao;
    do {
        printf("\nMenu\n");
        printf("1. Gerar dados aleatórios\n");
        printf("2. Criar índice\n");
        printf("3. Procurar elementos\n");
        printf("4. Remover registro\n");
        printf("5. Realizar testes de buscas com medição de tempo\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                geraDadosAleatorios(nomeArquivo, NUM_REGISTRO, MAX_CHAVE);
                break;
            case 2:
                criarIndice(arvore, nomeArquivo);
                break;
            case 3:
                procurarElemento(arvore, nomeArquivo);
                break;
            case 4:
                removerRegistro(arvore);
                break;
            case 5:
                realizarBuscas(nomeArquivo, arvore);
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 5);

    // Liberar memória da B-Tree (implementar função de destruição conforme necessário)
    // destruirBTree(arvore);

    return 0;
}