#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

unsigned int consulta(NODO *a, unsigned short porta)
{
    while (a != NULL)
    {
        if (a->porta == porta)
            return a->pos; //encontrou
        else if (a->porta > porta)
            a = a->esq;
        else
            a = a->dir;
    }

    return __UINT32_MAX__;
}

void fatorarvore(NODO *a, int *fatormax)
{
    if (a)
    {
        fatorarvore(a->esq, fatormax);
        fatorarvore(a->dir, fatormax);

        if (abs(fator(a)) > abs(*fatormax))
            *fatormax = fator(a);
    }
}

int fator(NODO *a)
{
    return (altura(a->esq) - altura(a->dir));
}

int altura(NODO *a)
{
    int Alt_Esq, Alt_Dir;

    if (a == NULL)
        return 0;
    else
    {
        Alt_Esq = altura(a->esq);
        Alt_Dir = altura(a->dir);

        if (Alt_Esq > Alt_Dir)
            return (1 + Alt_Esq);
        else
            return (1 + Alt_Dir);
    }
}

int contagem(NODO *a)
{
    if (a == NULL)
        return 0;
    else {
        int n = 1;

        if (a->esq)
            n += contagem(a->esq);

        if (a->dir)
            n += contagem(a->dir);

        return n;
    }
}


NODO* insereavl(NODO *a, unsigned short porta, unsigned int pos, int *ok)
{
    if (a == NULL) {

        a = (NODO *)malloc(sizeof(NODO));
        a->porta = porta;
        a->pos = pos;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        *ok = 1;

        return a;
    } 
    
    if (porta < a->porta) {

        a->esq = insereavl(a->esq, porta, pos, ok);
        
        if (*ok) {
            switch (a->FB) {
                case -1:
                    a->FB = 0;
                    *ok = 0;
                    break;
                case 0:
                    a->FB = 1;
                    break;
                case 1:
                    a = caso1(a, ok);
                    break;
            }
        }

    } else {

        a->dir = insereavl(a->dir, porta, pos, ok);

        if (*ok)
        {
            switch (a->FB) {
                case 1:
                    a->FB = 0;
                    *ok = 0;
                    break;
                case 0:
                    a->FB = -1;
                    break;
                case -1:
                    a = caso2(a, ok);
                    break;
            }
        }
    }

    return a;
}

NODO* caso1(NODO *a, int *ok) // Rotacoes direitas
{
    NODO *ptu;
    ptu = a->esq;

    if (ptu->FB == 1){
        a = rotacao_direita(a);
    } else{ 
        a = rotacao_dupla_direita(a);
    }

    a->FB = 0;
    *ok = 0;

    return a;
}

NODO* caso2(NODO *a, int *ok) // Rotacoes esquerdas
{
    NODO *ptu;
    ptu = a->dir;

    if (ptu->FB == -1){
        a = rotacao_esquerda(a);
    } else {
        a = rotacao_dupla_esquerda(a);
    }
    
    a->FB = 0;

    *ok = 0;

    return a;
}

NODO* rotacao_direita(NODO *pt)
{
    NODO *ptu;

    ptu = pt->esq;
    pt->esq = ptu->dir;
    ptu->dir = pt;
    pt->FB = 0;
    pt = ptu;

    return pt;
}

NODO* rotacao_esquerda(NODO *pt)
{
    NODO *ptu;

    ptu = pt->dir;
    pt->dir = ptu->esq;
    ptu->esq = pt;
    pt->FB = 0;
    pt = ptu;

    return pt;
}

NODO* rotacao_dupla_direita(NODO *pt)
{
    NODO *ptu, *ptv;

    ptu = pt->esq;
    ptv = ptu->dir;
    ptu->dir = ptv->esq;
    ptv->esq = ptu;
    pt->esq = ptv->dir;
    ptv->dir = pt;

    if (ptv->FB == 1)
        pt->FB = -1;
    else
        pt->FB = 0;

    if (ptv->FB == -1)
        ptu->FB = 1;
    else
        ptu->FB = 0;

    pt = ptv;

    return pt;
}

NODO *rotacao_dupla_esquerda(NODO *pt) {

    NODO *ptu, *ptv;

    ptu = pt->dir;
    ptv = ptu->esq;
    ptu->esq = ptv->dir;
    ptv->dir = ptu;
    pt->dir = ptv->esq;
    ptv->esq = pt;

    if (ptv->FB == -1)
        pt->FB = 1;

    else
        pt->FB = 0;

    if (ptv->FB == 1)
        ptu->FB = -1;
    else
        ptu->FB = 0;

    pt = ptv;

    return pt;
}
