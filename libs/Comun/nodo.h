#ifndef NODO_H
#define NODO_H

// Nodos para lista y cola
typedef struct sNodo {
    void* info;
    unsigned tamInfo;
    struct sNodo* sig;
} tNodo;

typedef struct sNodoA {
    void* info;
    unsigned tamInfo;
    struct sNodoA *izq, *der;
} tNodoA;

#endif // NODO_H