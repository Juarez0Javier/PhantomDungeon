#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stddef.h>
#include "../Comun/comun.h"
#include "../Comun/nodo.h"

typedef struct {
    tNodo* pri;
    tNodo* ult;
} tCola;

void crearCola(tCola* c);
bool ponerEnCola(tCola* c, const void* info, unsigned cantB);
bool sacarDeCola(tCola* c, void* info, unsigned cantB);
bool verPrimero(const tCola* c, void* info, unsigned cantB);
void vaciarCola(tCola* c);
bool colaVacia(const tCola* c);
bool colaLlena(const tCola* c);

#endif // COLA_H_INCLUDED
