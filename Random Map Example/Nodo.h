#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

#define TAMDATO 100

typedef struct sNodo
{
    void* dato;
    size_t tam;
    struct sNodo* sig;
} tNodo;

typedef struct sNodoE
{
    char dato[TAMDATO];
    struct sNodo* sig;
} tNodoE;

typedef struct sNodoD
{
    void *Dat;
    size_t Tam;
    struct sNodoD *Next, *Prev;
} tNodoD;

#endif // NODO_H_INCLUDED
