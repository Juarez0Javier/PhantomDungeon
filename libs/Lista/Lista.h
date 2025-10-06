#ifndef TDALISTA_H_INCLUDED
#define TDALISTA_H_INCLUDED

#include <stddef.h>
#include "../Comun/comun.h"
#include "../Comun/nodo.h"

typedef tNodo* tLista;
typedef void (*Imp) (const void* elem);
typedef int (*Cmp) (const void* e1, const void* e2);

void crearLista(tLista* pl);
bool ponerEnListaAlPipio(tLista* pl, const void* info, unsigned cantB, bool conDup, Cmp cmp);
bool ponerEnListaAlFinal(tLista* pl, const void* info, unsigned cantB, bool conDup, Cmp cmp);
void ordenarLista(tLista* pl, Cmp cmp, int metodo);
int buscarEnLista(tLista* pl, const void* info, Cmp cmp);
void vaciarLista(tLista* pl);
void mostrarLista(const tLista* pl, Imp imp);
bool listaVacia(const tLista* pl);
bool listaLlena(const tLista* pl);


#endif // TDALISTA_H_INCLUDED
