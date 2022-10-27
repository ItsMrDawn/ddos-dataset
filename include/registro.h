#ifndef REGISTRO_H
#define REGISTRO_H

#pragma pack(push,1) //diretiva pra não usar padding no struct

struct registro {
    
	unsigned int   chave;
    unsigned int   ip_src;
    unsigned int   ip_dest;
    unsigned short port_src;
    unsigned short port_dest;

    //cada um desses deve ter 16 bytes
	
}; 
typedef struct registro REGISTRO;

#pragma pack(pop) //desfazer a diretiva do padding


//conversoes de IP
unsigned int ip_to_int(char* ip);
void int_to_ip(unsigned int ip, char* outstr);

void print_registro(REGISTRO reg);
void print_registro_pos(unsigned int pos);

#endif