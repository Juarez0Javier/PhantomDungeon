#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED

#include "./../constants.h"
#include "../entidades/entidades.h"
#include "../partida/partida.h"

typedef struct {
    unsigned x, y;
    Entidad* ent;
} Movimiento;

void calcularMovJugador (Entidad* jugador, Mapa* mapa, char direccion, tCola* movs);
void calcularMovFantasmas (Vector* fantasmas, Mapa* mapa, tCola* movs);
void resolverMovimientos (Partida* partida, Mapa* mapa, tCola* movs, int* seccion);

#endif // MOVIMIENTO_H_INCLUDED
