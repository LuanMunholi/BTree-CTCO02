#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "busca_direta.h"
#include "registro.h"

#define CHAVE_TAMANHO 6

// Função para buscar uma chave diretamente no arquivo
int buscaDiretaNoArquivo(const char *nomeArquivo, const char *chave) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Registro registro;
        sscanf(linha, "%5s,%19[^,],%19[^,],%19[^\n]", registro.chave, registro.atributo2, registro.atributo3, registro.atributo4);

        if (strcmp(registro.chave, chave) == 0) {

            fclose(arquivo);
            return 1;  // Chave encontrada
        }
    }

    fclose(arquivo);
    return 0;  // Chave não encontrada
}

void buscaNaLinha(const char *nomeArquivo, int linha_desejada){
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return;
    }

    char buffer[1024];
    int current_line = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_line == linha_desejada) {
            printf("Linha %d: %s", linha_desejada, buffer);
            fclose(file);
            return;
        }
        current_line++;
    }

    printf("O arquivo não possui a linha %d.\n", linha_desejada);
    fclose(file);
}