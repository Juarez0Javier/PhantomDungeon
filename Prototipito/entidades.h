#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "constants.h"

#define JUGADOR 'J'
#define FANTASMA 'F'
#define PREMIO 'P'

typedef struct {
    char tipo;
    int num;
    int x;
    int y;
    size_t ultMovTicks;
} Entidad;

int cmpEntidad(void* a, void* b);
int cmpTipoEntidad(void* a, void* b);
int cmpPosEntidad(void* a, void* b);

void imprimirEntidad(Entidad e);
void imprimirEntidadMini(Entidad e);
void copiarEntidad(Entidad* dst, Entidad* src);

Entidad entidadNula();

#endif // ENTIDADES_H_INCLUDED
