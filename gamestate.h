#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "dinMtx.h"
#include "Vector.h"
#include "mapa.h"

typedef struct {
    Mapa mapa;
    MapaEntidades mapaEntidades;
    Entidad jugador;
    Vector entidades;
    size_t ticks;
} GameState;

void nullGame(GameState* game);
void printGame(GameState* game);

#endif // GAMESTATE_H_INCLUDED
