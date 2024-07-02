#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_REGISTROS 10000
#define MAX_CHAVE 99999
#define NUM_ATRIBUTOS 4

typedef struct {
    int chave;
    char atributo2[20];
    char atributo3[20];
    char atributo4[20];
} Registro;

void geraRegistroAleatorio(Registro *registro, int chave) {
    registro->chave = chave;
    sprintf(registro->atributo2, "Atributo2_%d", rand() % 1000);
    sprintf(registro->atributo3, "Atributo3_%d", rand() % 1000);
    sprintf(registro->atributo4, "Atributo4_%d", rand() % 1000);
}

int main() {
    FILE *arquivo = fopen("dados.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return 1;
    }

    srand(time(NULL));
    int chavesUsadas[NUM_REGISTROS] = {0};

    for (int i = 0; i < NUM_REGISTROS; i++) {
        Registro registro;
        int chave;

        do {
            chave = rand() % MAX_CHAVE + 1;
        } while (chavesUsadas[chave] == 1);

        chavesUsadas[chave] = 1;
        geraRegistroAleatorio(&registro, chave);

        fprintf(arquivo, "%d,%s,%s,%s\n", registro.chave, registro.atributo2, registro.atributo3, registro.atributo4);
    }

    fclose(arquivo);
    printf("Arquivo gerado com sucesso.\n");
    return 0;
}
