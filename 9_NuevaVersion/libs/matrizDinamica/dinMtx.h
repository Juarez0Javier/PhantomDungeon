#ifndef DINMTX_H_INCLUDED
#define DINMTX_H_INCLUDED

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

/*
void** newDinMtx(unsigned rows, unsigned cols, unsigned len);
void destroyDinMtx(unsigned rows, unsigned cols, unsigned len, void** mtx);
*/
void** crearMatriz(unsigned rows, unsigned cols, unsigned len);
void destruirMatriz(unsigned rows, void** mtx);

#endif // DINMTX_H_INCLUDED
