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
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Tamanho de cada linha, incluindo o caractere de nova linha '\n'
    int tamanho_linha = 48;  // Altere para o tamanho correto

    // Calcular o deslocamento
    long offset = (linha_desejada - 1) * tamanho_linha;

    // Mover o ponteiro do arquivo para a linha desejada
    if (fseek(arquivo, offset, SEEK_SET) != 0) {
        perror("Erro ao mover o ponteiro do arquivo\n");
        fclose(arquivo);
        return;
    }

    // Ler a linha desejada
    char buffer[tamanho_linha + 1]; // +1 para o caractere nulo
    if (fgets(buffer, sizeof(buffer), arquivo) == NULL) {
        perror("Erro ao ler a linha do arquivo\n");
        fclose(arquivo);
        return;
    }

    printf("Linha %d: %s", linha_desejada, buffer);
    fclose(arquivo);
}