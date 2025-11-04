#ifndef DINMTX_H_INCLUDED
#define DINMTX_H_INCLUDED

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void** crearMatriz(unsigned rows, unsigned cols, unsigned len);
void destruirMatriz(unsigned rows, void** mtx);
void copiarMatriz(void** dst, void** src, unsigned filas, unsigned cols, unsigned len);

#endif // DINMTX_H_INCLUDED
