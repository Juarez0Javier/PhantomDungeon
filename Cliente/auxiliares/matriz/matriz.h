#ifndef MATRIZ_H_INCLUDED
#define MATRIZ_H_INCLUDED

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

void** crearMatriz(unsigned rows, unsigned cols, unsigned len);
void destruirMatriz(unsigned rows, void** mtx);

#endif // MATRIZ_H_INCLUDED
