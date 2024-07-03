#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "btree.h"
#include "busca_direta.h"
#include "registro.h"

#define CHAVE_TAMANHO 6     // Definição do tamanho máximo para a chave
#define NUM_ATRIBUTOS 4     // Definição do número máximo de atributos no arquivo
#define MAX_CHAVE 99999     // Definição do valor máximo para a chave
#define NUM_REGISTRO 10000  // Definição do valor máximo de registros
#define NUM_BUSCAS 30       // Definição do número de buscas realizadas no teste

void geraRegistroAleatorio(Registro *registro, int chave) {
    sprintf(registro->chave, "%05d", chave);  // Formata a chave com 5 dígitos
    sprintf(registro->atributo2, "Atributo2_%d", rand() % 1000);
    sprintf(registro->atributo3, "Atributo3_%d", rand() % 1000);
    sprintf(registro->atributo4, "Atributo4_%d", rand() % 1000);
}

void embaralhaArray(int *array, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        int j = rand() % tamanho;
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void geraDadosAleatorios(const char *nomeArquivo, int numRegistros) {
    int chaves[NUM_REGISTRO];
    for (int i = 1; i <= NUM_REGISTRO; i++) {
        chaves[i] = i;
    }

    // Embaralha o array de chaves
    embaralhaArray(chaves, NUM_REGISTRO);

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    Registro registro;
    for (int i = 0; i < numRegistros; i++) {
        geraRegistroAleatorio(&registro, chaves[i]);
        fprintf(arquivo, "%s,%s,%s,%s\n", registro.chave, registro.atributo2, registro.atributo3, registro.atributo4);
    }

    fclose(arquivo);
} 

void criarIndice(BTree *arvore, const char *nomeArquivo) {    
    insereDoArquivo(arvore,nomeArquivo);
}

void procurarElemento(BTree *arvore, const char *nomeArquivo) {
    char chave[CHAVE_TAMANHO];
    printf("Digite a chave de 5 digitos a ser buscada: ");
    scanf("%s", chave);
    chave[CHAVE_TAMANHO-1] = '\0';
    int indice;

    // Busca na B-Tree
    BTreeNode *encontradoBTree = buscaNaBtree(arvore->raiz, chave, &indice);
    if (encontradoBTree != NULL) {
        printf("Chave encontrada na linha: %d\n", encontradoBTree->chaves[indice].linha);
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
    char chave[CHAVE_TAMANHO];
    printf("Digite a chave de 5 digitos a ser removida:");
    scanf("%s", chave);
    chave[CHAVE_TAMANHO-1] = '\0';
    removeChave(arvore, chave);
}

void realizarBuscas(const char *nomeArquivo, BTree *arvore) {
    double temposDireto[NUM_BUSCAS];
    double temposBTree[NUM_BUSCAS];
    double somaDireto = 0.0, somaBTree = 0.0;
    double maxDireto = 0.0, minDireto = 1.0;
    double maxBTree = 0.0, minBTree = 1.0;
    int indice;
    clock_t inicio, fim;

    srand(time(NULL));

    for (int i = 0; i < NUM_BUSCAS; i++) {
        char chave[CHAVE_TAMANHO];
        sprintf(chave, "%05d", rand() % MAX_CHAVE);

        // Função para medir o tempo de execução de uma busca direta no arquivo
        inicio = clock();
        buscaDiretaNoArquivo(nomeArquivo, chave);
        fim = clock();
        temposDireto[i] = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        // Medindo o tempo de execução de uma busca na B-Tree
        inicio = clock();
        buscaNaBtree(arvore->raiz, chave, &indice);
        fim = clock();
        temposBTree[i] = (double)(fim - inicio) / CLOCKS_PER_SEC;

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
    printf("Media: %f segundos\n", mediaDireto);
    printf("Maximo: %f segundos\n", maxDireto);
    printf("Minimo: %f segundos\n", minDireto);

    printf("\nBusca B-Tree:\n");
    printf("Media: %f segundos\n", mediaBTree);
    printf("Maximo: %f segundos\n", maxBTree);
    printf("Minimo: %f segundos\n", minBTree);
}

int main() {
    const char *nomeArquivo = "dados.txt";
    int ordem;

    printf("Inicializando a Btree\n");
    printf("Digite a ordem da Btree:");
    scanf("%d", &ordem);
    BTree *arvore = criaBTree(ordem/2);

    int opcao;
    do {
        printf("\nMenu\n");
        printf("1. Gerar dados aleatorios\n");
        printf("2. Criar indice\n");
        printf("3. Procurar elementos\n");
        printf("4. Remover registro\n");
        printf("5. Realizar testes de buscas com medicao de tempo\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                geraDadosAleatorios(nomeArquivo, NUM_REGISTRO);
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
    } while (opcao != 6);

    // Liberar memória da B-Tree (implementar função de destruição conforme necessário)
    // destruirBTree(arvore);

    return 0;
}