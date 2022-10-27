#ifndef PORTSL_H
#define PORTSL_H


struct nodo_porta
{
    unsigned short porta;
    unsigned int pos;
    
    struct nodo_porta *irmao;
    struct nodo_porta *filho;
};
typedef struct nodo_porta NPORT;


void criar_lista(NPORT* lista);
void insere_porta_lista(unsigned short numporta, unsigned int pos, NPORT* lista);
unsigned int pesq_lista(unsigned short numporta);
void pesq_porta_lista();

#endif