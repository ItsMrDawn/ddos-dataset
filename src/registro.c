#include "registro.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned int ip_to_int(char* ip) {

    char ipmat[4][4];
    int pedaco = 0, posant = 0, i, comp;

    comp = strlen(ip);

    //separar os pedacos do ip em 4 strings

    for (i = 0; i <= comp; i++) {

        if ((ip[i] == '.') || (comp == i)){
            memcpy(&ipmat[pedaco][0], &ip[posant], (i - posant));
            ipmat[pedaco][i-posant] = '\0';
            posant = i+1;
            pedaco++;
        }

    }

    int intmat[4];

    for (i = 0; i < 4; i++) {
        intmat[i] = atoi(&ipmat[i][0]);
    }
	
	return (intmat[0] << 24) | (intmat[1] << 16) | (intmat[2] << 8) | intmat[3];
	
}

void int_to_ip(unsigned int ip, char* outstr) {
	

    int intmat[4];

    intmat[0] = (ip & 0xFF000000) >> 24;
    intmat[1] = (ip & 0x00FF0000) >> 16;
    intmat[2] = (ip & 0x0000FF00) >> 8;
    intmat[3] =  ip & 0x000000FF;

    sprintf(outstr, "%d.%d.%d.%d", intmat[0], intmat[1], intmat[2], intmat[3]);
	
}

//printar um registro
void print_registro(REGISTRO reg) {

    char ip_src[20];
    char ip_dest[20];

    int_to_ip(reg.ip_src,  &ip_src);
    int_to_ip(reg.ip_dest, &ip_dest);

    printf("\nChave: %u \nIP Origem: %s \nPorta Origem: %hu \nIP Destino: %s \nPorta Destino: %hu \n\n", reg.chave, ip_src, reg.port_src, ip_dest, reg.port_dest);

}

//printar um registro, dado uma posicao do arquivo binario
void print_registro_pos(unsigned int pos) {

    FILE *arq;
    arq = fopen(ARQBIN, "rb");

    REGISTRO reg;
    fseek(arq, pos, 0);
    fread(&reg, sizeof(REGISTRO), 1, arq);

    print_registro(reg);

    fclose(arq);    

}
