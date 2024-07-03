// registro.h

#ifndef REGISTRO_H
#define REGISTRO_H

#define CHAVE_TAMANHO 6
#define MAX_ATRIBUTO 100

typedef struct Registro {
    char chave[CHAVE_TAMANHO];
    char atributo2[MAX_ATRIBUTO];
    char atributo3[MAX_ATRIBUTO];
    char atributo4[MAX_ATRIBUTO];
} Registro;

#endif // REGISTRO_H