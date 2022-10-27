#include "indexar.h"
#include "pesq_idx.h"
#include "select.h"
#include "ports_lista.h"
#include "ports_arvore.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    verificar_arquivos();
 
    char opcao = '\0';

    while (opcao != '9') {

        printf("\nEscolha uma opcao:\n");

        printf("0) Pesquisar chave (pesquisa binaria no indice em arquivo)\n");
        printf("1) Pesquisar IP origem (indice de dois niveis em arquivo)\n");
        printf("2) Pesquisar porta origem (AVL em memoria)\n");
        printf("3) Pesquisar porta destino (lista encadeada em memoria)\n");
        printf("4) Quais sao as portas com maior incidencia?\n");
        printf("5) Quais sao os IP origem com maior incidencia? (usando indice)\n");
        printf("9) Sair\n\n");

        scanf(" %c", &opcao);

        switch (opcao) {
            case '0': pesq_chave(); break;
            case '1': pesq_ip_src(); break;
            case '2': pesq_porta_arvore(); break;
            case '3': pesq_porta_lista(); break;
            case '4': rank_portas(); break;
            case '5': rank_enderecos(); break;
        }
    
    }


    return 0;

};