#include "gera_dados.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ATRIBUTOS 4

typedef struct {
    char chave[6];  // Tamanho 5 para a chave e 1 para o caractere nulo
    char atributo2[20];
    char atributo3[20];
    char atributo4[20];
} Registro;

void geraRegistroAleatorio(Registro *registro, int chave) {
    sprintf(registro->chave, "%05d", chave);  // Formata a chave com 5 dígitos
    sprintf(registro->atributo2, "Atributo2_%d", rand() % 1000);
    sprintf(registro->atributo3, "Atributo3_%d", rand() % 1000);
    sprintf(registro->atributo4, "Atributo4_%d", rand() % 1000);
}

void geraDadosAleatorios(const char *nomeArquivo, int numRegistros, int maxChave) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    srand(time(NULL));
    int *chavesUsadas = (int *)calloc(maxChave + 1, sizeof(int));
    if (chavesUsadas == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivo);
        return;
    }

    for (int i = 0; i < numRegistros; i++) {
        Registro registro;
        int chave;

        do {
            chave = rand() % maxChave + 1;
        } while (chavesUsadas[chave] == 1);

        chavesUsadas[chave] = 1;
        geraRegistroAleatorio(&registro, chave);

        fprintf(arquivo, "%s,%s,%s,%s\n", registro.chave, registro.atributo2, registro.atributo3, registro.atributo4);
    }

    free(chavesUsadas);
    fclose(arquivo);
    printf("Arquivo gerado com sucesso: %s\n", nomeArquivo);
}