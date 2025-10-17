#ifndef PILA_DINAMICA_H_INCLUDED
#define PILA_DINAMICA_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "../../Comun/comun.h"
#include "../../Comun/nodo.h"

typedef tNodo* tPilaD;

void crearPilaD(tPilaD*);

int apilarD(tPilaD*,const void*, unsigned);
int desapilarD(tPilaD*, void*, unsigned);

int verTopeD(tPilaD*, void*, unsigned);

int pilaDLlena(tPilaD*, unsigned);
int pilaDVacia(tPilaD*);

void vaciarPilaD(tPilaD*);

#endif // PILA_DINAMICA_H_INCLUDED
