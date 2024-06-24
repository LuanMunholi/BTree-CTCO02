#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "btree.c"

int main() {
  BTree *tree = criaBTree();
  //array com todods os registros. linha do txt = registros[i+1]
  Registro *registros;
  int count_registros;

  //inicializando pre-processamento da carga.
  //Pode ser melhorado no futuro caso
  //seja descoberto um meio melhor de fazer acesso
  //direto por linha no .txt
  leArquivo("entrada.txt", &registros, &count_registros);

  //Printando todas as linhas pra verificar
  for (int i = 0; i < count_registros; i++) {
    printf("Index: %d, Nome: %s, ID: %d, IsActive: %d\n", registros[i].index,
           registros[i].nome, registros[i].id, registros[i].isActive);
  }

  for(int i=0; i<count_registros; i++){
    ArquivoIndex *fp1 = (ArquivoIndex *)malloc(sizeof(struct ArquivoIndex));
    fp1->index = registros[i].index;
    fp1->line = i+1;
    printf("insere: %d - %d\n", fp1->index, fp1->line);
    insere(tree, fp1);
  }

  //Buscar um index na Btree
  int indiceProcurado = 299;
  ArquivoIndex *resultado = busca(tree->raiz, indiceProcurado);

  if (resultado != NULL) {
    int line = resultado->line;
    printf("Encontrou index %d na linha %d\n", resultado->index, line);
    printf("Recuperando data da linha %d:\n [index] %d\n [nome] %s\n [id] %d\n [isActive] %d\n", line, registros[line-1].index, registros[line-1].nome, registros[line-1].id, registros[line-1].isActive);
  } else {
    printf("Indice %d não encontrado\n", indiceProcurado);
  }

  //Liberar memoria alocada
  free(registros);
  return 0;

}
