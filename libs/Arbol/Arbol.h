#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "../comun/comun.h"
#include "../comun/nodo.h"
#include <stddef.h>

#define INORDEN 0
#define PREORDEN 1
#define POSORDEN 2

typedef tNodoA* tArbol;

typedef bool (*Cond) (const void* elem);

void crearArbol(tArbol* pa);
int ponerEnArbolI(tArbol* pa, const void* info, unsigned tamInfo, Cmp cmp, Acc acc, void* accExtra);
int ponerEnArbolR(tArbol* pa, const void* info, unsigned tamInfo, Cmp cmp, Acc acc, void* accExtra);
int cargarArbolDeArchBin(tArbol* pa, const char* nomArch, unsigned tamReg, Cmp cmp, Acc acc, void* accExtra);
void recorrerArbolR(tArbol* pa, int notacion, Acc acc, void* accExtra);
bool buscarPorClaveArbol(const tArbol* pa, void* info, unsigned tamInfo, Cmp cmp);
bool buscarPorNoClaveArbol(const tArbol* pa, void* info, unsigned tamInfo, Cmp cmp);
void vaciarArbol(tArbol* pa);
bool arbolVacio(const tArbol* pa);
unsigned cantNodosArbolR(const tArbol* pa);
unsigned cantHojasArbolR(const tArbol* pa);
unsigned cantNoHojasArbolR(const tArbol* pa);
unsigned cantNodosCond(const tArbol* pa, Cond cond);
bool buscarClaveMayorArbol(const tArbol* pa, void* info, unsigned tamInfo);
bool buscarClaveMenorArbol(const tArbol* pa, void* info, unsigned tamInfo);
int alturaArbol(const tArbol* pa);
int nivelArbol(const tArbol* pa);
bool esArbolCompleto(const tArbol* pa);
bool esArbolBalanceado(const tArbol* pa);
bool esArbolAVL(const tArbol* pa);
bool sacarDeArbol(tArbol* pa, void* info, unsigned tamInfo, Cmp cmp);

#endif // ARBOL_H_INCLUDED
