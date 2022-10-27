#include "indexar.h"
#include "registro.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int pesq_binaria(unsigned int dado, const char* indice) {

    //retorna a posicao encontrada no indice

    //arquivo de indice
    FILE *idx;
    idx = fopen(indice, "rb");

    //calcular tamanho do arquivo e numero de registros
    long int tament = sizeof(ENTRADA);

    fseek(idx, 0L, SEEK_END); //SEEK_END vai no fim do arquivo
    long int tamarq = ftell(idx); //posição atual

    ///////////////////

    ENTRADA ent;

    //numreg é o registro atual que será lido
    //min e max são os limites inferior e superior de pesquisa
    int nument = ((tamarq / tament) / 2) - 1;
    int min = 0;
    int max = tamarq / tament;

    unsigned int result = __UINT32_MAX__;

    while (1) {

        fseek(idx, (nument * tament), SEEK_SET); //SEEK_SET parte do começo do arquivo
        fread(&ent, tament, 1, idx);

        printf("Registro atual: %u \n", ent.dado);

        if (ent.dado == dado) {
            result = ent.pos;
            break;
        }

        if (nument == min) {
            break;
        }

        if (ent.dado < dado) {
            //procurar na parte superior do arq, aumentando o min
            min = nument;
        } else {
            //parte inferior, diminuindo o max
            max = nument;
        }

        nument = min + ((max - min) / 2);

    }

    fclose(idx);    

    return result;

}

void pesq_chave() {

    //ler o valor da chave 
    unsigned int chave;
    printf("\nDigite uma chave a ser pesquisada: \n");
    scanf("%u", &chave);

    //////////////////

    //procurar a chave lida no indice

    unsigned int pos = pesq_binaria(chave, CHAVE_IDX);

    if (pos == __UINT32_MAX__) {
        printf("Chave nao encontrada no indice.\n");
        return;
    }

    printf("Encontrei a chave no indice. Vou ler o arquivo binario na posicao %u\n", pos);

    print_registro_pos(pos);

}

void pesq_ip_src() {

    //ler o valor da chave 
    unsigned int ip;
    char strip[20];

    printf("\nDigite um IP a ser pesquisado: (formato x.x.x.x)\n");
    scanf("%s", &strip);

    ip = ip_to_int(strip);

    //////////////////

    //procurar o ip lido no indice de segundo nivel

    unsigned int pos = pesq_binaria(ip, IP2_IDX);

    if (pos == __UINT32_MAX__) {
        printf("IP nao encontrado no indice.\n");
        return;
    }

    //ler no indice de primeiro nivel
    FILE *idx1;
    idx1 = fopen(IP1_IDX, "rb");

    ENTRADA ent;

    fseek(idx1, pos, 0);
    fread(&ent, sizeof(ENTRADA), 1, idx1);

    printf("Encontrei o endereco IP no indice de segundo nivel.\nO primeiro sendo na posicao %u\n", ent.pos);

    print_registro_pos(ent.pos);

    fclose(idx1);

}