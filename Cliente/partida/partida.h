#include <SDL.h>
#include "../mapa/mapa.h"
#include "./entidad.h"
#include "../../Vector/Vector.h"
#include "../../Cola/Cola.h"

#ifndef PARTIDA_H
#define PARTIDA_H

typedef struct {
    bool pausado;
    unsigned vidasRestantes, premiosObt, puntuacion;
    Mapa mapa;
    Entidad jugador;
    Vector fantasmas;
    tCola movs;
    //tLista regMovs;
} Partida;

typedef struct {
    unsigned x, y;
    Entidad* ent;
} Movimiento;

void calcularMovJugador (Entidad* jugador, Mapa* mapa, char direccion, tCola* movs);
void calcularMovFantasmas (Vector* fantasmas, Mapa* mapa, tCola* movs);
void resolverMovimientos (Partida* partida, Mapa* mapa, tCola* movs, int* seccion);
char teclaAWASD (SDL_KeyCode tecla);

#endif