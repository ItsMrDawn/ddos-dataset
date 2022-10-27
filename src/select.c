#include "indexar.h"
#include "registro.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rank_portas() {

    unsigned int* port_src  = calloc(65536, sizeof(unsigned int));
    unsigned int* port_dest = calloc(65536, sizeof(unsigned int));

    //arquivo binario.bin
    FILE *arqbin;

    arqbin = fopen(ARQBIN, "rb");    

    //calcular tamanho do arquivo e numero de registros
    fseek(arqbin, 0L, SEEK_END); //SEEK_END vai no fim do arquivo
    int numreg = ftell(arqbin) / sizeof(REGISTRO); //ftell == pos atual

    //voltar pro começo do arquivo
    rewind(arqbin);

    REGISTRO reg;
    int i, j;

    //ler os registros e contar quantas vezes cada porta aparece
    for (i = 0; i < numreg; i++) {
    
        fread(&reg, sizeof(REGISTRO), 1, arqbin);

        port_src[reg.port_src]++;
        port_dest[reg.port_dest]++;

    }

    fclose(arqbin);


    //vetor onde vao ser armazenados os resultados
    ENTRADA* rank_src  = calloc(QTDRANK, sizeof(ENTRADA));
    ENTRADA* rank_dest = calloc(QTDRANK, sizeof(ENTRADA));

    unsigned int maior_src, maior_dest, porta_src, porta_dest;

    for (j = 0; j < QTDRANK; j++) {

        maior_src  = 0;
        maior_dest = 0;
        porta_src  = 0;
        porta_dest = 0;

        //procurar a porta com maior numero
        for (i = 0; i < 65536; i++) {

            if (port_src[i] > maior_src) {
                maior_src = port_src[i];
                porta_src = i;
                port_src[i] = 0;
            }

            if (port_dest[i] > maior_dest) {
                maior_dest = port_dest[i];
                porta_dest = i;
                port_dest[i] = 0;
            }            

        }

        //inserir no vetor de resultados
        rank_src[j].dado = porta_src;
        rank_src[j].pos  = maior_src;

        rank_dest[j].dado = porta_dest;
        rank_dest[j].pos  = maior_dest;

    }

    free(port_src);
    free(port_dest);

    printf("\nTop %d portas com maior incidencia no dataset\n\nOrigem:\n", QTDRANK);

    for (j = 0; j < QTDRANK; j++) {
        printf("Qtd: %-10u    Porta: %u \n", rank_src[j].pos, rank_src[j].dado);
    }

    printf("\nDestino:\n");

    for (j = 0; j < QTDRANK; j++) {
        printf("Qtd: %-10u    Porta: %u \n", rank_dest[j].pos, rank_dest[j].dado);
    }  

    free(rank_src);
    free(rank_dest);

    return;

}

void rank_enderecos() {

    //usar o indice de segundo nivel de endereços ip para calcular quais tem mais entradas no indice de nivel 1
    FILE *arqidx;
    arqidx = fopen(IP2_IDX, "rb");    

    //calcular tamanho do arquivo e numero de entradas
    fseek(arqidx, 0L, SEEK_END); //SEEK_END vai no fim do arquivo
    int nument = ftell(arqidx) / sizeof(ENTRADA); //ftell == pos atual

    //um vetor grande o suficiente pra pelo menos todos os elementos
    ENTRADA* ips = malloc(nument * sizeof(ENTRADA));

    //voltar pro começo do arquivo
    rewind(arqidx);
    
    //ja que vou alocar um vetor de qualquer jeito, ja aproveito pra botar conteudo do indice que ajuda
    fread(&ips[0], sizeof(ENTRADA), nument, arqidx);

    fclose(arqidx);

    //calcular o tamanho total do indice 1
    arqidx = fopen(IP1_IDX, "rb");  
    fseek(arqidx, 0L, SEEK_END); //SEEK_END vai no fim do arquivo
    long int tamidx1 = ftell(arqidx); //ftell == pos atual 
    fclose(arqidx);


    int i;
    ENTRADA ent;
    

    for (i = 0; i < nument - 1; i++) {

        //quantas entradas tem entre esse elemento do indice e o proximo
        ent = ips[i];
        ent.pos = (ips[i+1].pos - ips[i].pos) / sizeof(ENTRADA);
        ips[i] = ent;

    }

    ent = ips[i];
    ent.pos = (tamidx1 - ips[i].pos) / sizeof(ENTRADA);
    ips[i] = ent;


    //ordenar
    qsort(ips, nument, sizeof(ENTRADA), comp_entrada_pos_desc);

    char ip_str[20];

    printf("\nTop %d enderecos IP origem com maior incidencia no dataset:\n", QTDRANK);

    for (i = 0; i < QTDRANK; i++) {  

        int_to_ip(ips[i].dado, &ip_str);

        printf("Qtd: %-10u    IP: %s \n", ips[i].pos, ip_str);

    }  

    free(ips);

}