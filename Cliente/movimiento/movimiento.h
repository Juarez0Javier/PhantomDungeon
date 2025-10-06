#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED

#include "./../constants.h"
#include "../entidades/entidades.h"
#include "../partida/partida.h"

typedef struct {
    unsigned x, y;
    Entidad* ent;
} Movimiento;

typedef struct {
    unsigned x, y;
} Coordenada;

void calcularMovJugador (Entidad* jugador, Mapa* mapa, char direccion, tCola* movs);
void calcularMovFantasmas (Vector* fantasmas, Mapa* mapa, tCola* movs, Entidad* jugador, unsigned deltaTime);
void resolverMovimientos (Partida* partida, Mapa* mapa, tCola* movs, int* seccion);
void mostrarCoordenada (const void* coord);

#endif // MOVIMIENTO_H_INCLUDED
