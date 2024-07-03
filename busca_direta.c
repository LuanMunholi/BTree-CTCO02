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