#ifndef AVL_H
#define AVL_H


struct TNodoA
{
    unsigned short porta;
    unsigned int pos;
    int FB;
    struct TNodoA *esq;
    struct TNodoA *dir;
};
typedef struct TNodoA NODO;


unsigned int consulta(NODO* a, unsigned short chave);

int contagem(NODO *a);

int altura (NODO *a);

int fator (NODO *a);

void fatorarvore(NODO *a, int *fatormax);

NODO* insereavl(NODO *a, unsigned short porta, unsigned int pos, int *ok);

NODO* caso1(NODO *a, int *ok);

NODO* caso2(NODO *a, int *ok);

NODO* rotacao_direita(NODO* pt);

NODO* rotacao_esquerda(NODO *pt);

NODO* rotacao_dupla_direita (NODO* pt);

NODO* rotacao_dupla_esquerda (NODO *pt);

#endif