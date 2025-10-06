#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#include "../constants.h"

#define JUGADOR 'J'
#define FANTASMA 'F'
#define PREMIO 'P'

typedef struct {
    char tipo;
    unsigned num; // Esto no haria falta
    unsigned x, y, xInicial, yInicial;
    size_t ticksEntreMovs, ticksUltimoMov;
    bool eliminado;
} Entidad;

void crearEntidad(Entidad* e, unsigned x, unsigned y, char tipo, size_t ticksEntreMovs);

int cmpEntidad(void* a, void* b);
int cmpTipoEntidad(void* a, void* b);
int cmpPosEntidad(void* a, void* b);

void imprimirEntidad(Entidad e);
void imprimirEntidadMini(Entidad e);
void copiarEntidad(Entidad* dst, Entidad* src);

Entidad entidadNula();

#endif // ENTIDADES_H_INCLUDED
