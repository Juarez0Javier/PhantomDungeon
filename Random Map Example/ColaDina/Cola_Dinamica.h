#ifndef COLA_DINAMICA_H_INCLUDED
#define COLA_DINAMICA_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "../../Utilities/Utix.h"
#include "../Nodo.h"

#define MIN( X , Y ) ( ( X ) <= ( Y ) ? ( X ) : ( Y ) )


typedef struct
{
    tNodo* prim;
    tNodo* ulti;

} tColaD;

void crearColaD(tColaD*);

int ponerEnColaD(tColaD*,const void*, size_t);
int sacarDeColaD(tColaD*, void*, size_t);

int verPrimeroColaD(tColaD*, void*, size_t);

int colaDLlena(tColaD*, size_t);
int colaDVacia(tColaD*);

void vaciarColaD(tColaD*);


#endif // COLA_DINAMICA_H_INCLUDED
