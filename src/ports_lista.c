#include "registro.h"
#include "defs.h"
#include "ports_lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insere_porta_lista(unsigned short numporta, unsigned int pos, NPORT* lista) {

    NPORT* novo = (NPORT*)malloc(sizeof(NPORT));
    novo->porta = numporta;
    novo->pos   = pos;

    NPORT* nodo = lista;
    NPORT* nodoant = lista;

    while (nodo) {

        //percorrer todos "irmaos" ate encontrar a porta
        if (nodo->porta == numporta) {
            
            //inserir no meio da lista, substituindo o pai
            novo->irmao = nodo->irmao;
            novo->filho = nodo;

            //redirecionar o anterior, a nao ser que seja o 1o da lista
            if (nodoant->porta != nodo->porta) {
                nodoant->irmao = novo;
            }
            
            return;

        }
        
        nodoant = nodo;
        nodo = nodo->irmao;
    }

    //nao encontrou, insere no final da lista
    nodoant->irmao = novo;
    novo->irmao = NULL;
    

}

void criar_lista(NPORT* lista) {


    //arquivo binario.bin
    FILE *arqbin;

    arqbin = fopen(ARQBIN, "rb");    

    //calcular tamanho do arquivo e numero de registros
    fseek(arqbin, 0L, SEEK_END); //SEEK_END vai no fim do arquivo
    int numreg = ftell(arqbin) / sizeof(REGISTRO); //ftell == pos atual

    //voltar pro começo do arquivo
    rewind(arqbin);

    REGISTRO reg;
    int i;

    lista->filho = NULL;
    lista->irmao = NULL;
    lista->porta = 0;

    //ler os registros e inserir os nodos conforme aparecem as portas
    for (i = 0; i < numreg; i++) {

        fread(&reg, sizeof(REGISTRO), 1, arqbin);
        
        insere_porta_lista(reg.port_dest, ftell(arqbin) - sizeof(REGISTRO), lista);

    }

    fclose(arqbin);

    return;

}

unsigned int pesq_lista(unsigned short numporta) {

    static NPORT* lista = NULL;

    if (!lista) {
        printf("\nCriando indice em lista encadeada das portas...\n");
        lista = (NPORT*)malloc(sizeof(NPORT));
        criar_lista(lista);
    }

    NPORT* nodo = lista;

    while (nodo) {

        //percorrer todos "irmaos" ate encontrar a porta
        if (nodo->porta == numporta) {
            
            //percorrer todos os filhos
            while (nodo->filho) {
                nodo = nodo->filho;
            }
        
            return nodo->pos;

        }

        nodo = nodo->irmao;

    }

    return __UINT32_MAX__;

}


void pesq_porta_lista() {

    //ler numero da porta 
    unsigned short porta;
    printf("\nDigite uma porta destino a ser pesquisada: \n");
    scanf("%hu", &porta);

    //////////////////

    //procurar a porta lida no indice
    unsigned int pos = pesq_lista(porta);

    if (pos == __UINT32_MAX__) {
        printf("Porta nao encontrada no indice.\n");
        return;
    }

    printf("Encontrei a porta no indice em memoria. A ultima ocorrencia na posicao %u\n", pos);

    print_registro_pos(pos);


}