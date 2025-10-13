#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "../../Utilities/Utix.h"

#include "../Nodo.h"

#define MIN(x,y) ((x)?((x)<(y)):(y))

typedef tNodo *tListaS;

void crearListaS (tListaS*);

int ponerPriListaS (tListaS*, const void*, size_t);
int sacarPriListaS (tListaS*, void*, size_t);
int verPriListaS (tListaS*, void*, size_t);

int ponerUltiListaS (tListaS*, const void*, size_t);
int sacarUltiListaS (tListaS*, void*, size_t);
int verUltiListaS (tListaS*, void*, size_t);

int listaSLlena (const tListaS*, size_t);
int listaSVacia (const tListaS*);

void vaciarListaS (tListaS*);

int ponerOrdenListaS(tListaS*, const void*, size_t, int (*Cmp)(const void*,const void*));
int ordenarListaS(tListaS*, int (*Cmp)(const void*,const void*));

void listaSMap(const tListaS*, void (*Act)(const void*,void*),void*);
int listaSReduce(const tListaS*,void*, void (*Act)(const void*,void*,void*),void*);
tListaS* listaSFilter(tListaS*, int(*Act)(void*,void*),void*);


#endif // LISTA_SIMPLE_H_INCLUDED
