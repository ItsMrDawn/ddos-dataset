#include "registro.h"
#include "defs.h"
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NODO* avlg;

void criar_arvore() {


    //arquivo binario.bin
    FILE *arqbin;

    arqbin = fopen(ARQBIN, "rb");    

    //calcular tamanho do arquivo e numero de registros
    fseek(arqbin, 0L, SEEK_END); //SEEK_END vai no fim do arquivo
    int numreg = ftell(arqbin) / sizeof(REGISTRO); //ftell == pos atual

    //voltar pro começo do arquivo
    rewind(arqbin);

    REGISTRO reg;
    int i, ok = 0;

    //ler os registros e inserir os nodos conforme aparecem as portas
    for (i = 0; i < numreg; i++) {

        fread(&reg, sizeof(REGISTRO), 1, arqbin);
        
        avlg = insereavl(avlg, reg.port_src, ftell(arqbin) - sizeof(REGISTRO), &ok);

    }

    printf("AVL criada com altura %d\n", altura(avlg));

    fclose(arqbin);

    return;

}


void pesq_porta_arvore() {

    //ler numero da porta 
    unsigned short porta;
    printf("\nDigite uma porta origem a ser pesquisada: \n");
    scanf("%hu", &porta);

    //////////////////

    if (!avlg) {
        printf("\nCriando AVL das portas...\n");
        criar_arvore();
    }

    //procurar a porta lida na arvore
    unsigned int pos = consulta(avlg, porta);

    if (pos == __UINT32_MAX__) {
        printf("Porta nao encontrada no indice.\n");
        return;
    }

    printf("Encontrei a porta na arvore em memoria. A primeira ocorrencia na posicao %u\n", pos);

    print_registro_pos(pos);


}