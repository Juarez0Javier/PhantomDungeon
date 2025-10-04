#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include "../constants.h"
#include "../mapa/mapa.h"

typedef struct {
    bool pausado;
    unsigned vidasRestantes, premiosObt, puntuacion;
    Mapa mapa;
    //MapaEntidades mapaEntidades;
    Entidad jugador;
    Vector fantasmas;
    tCola movs;
} Partida;

typedef Partida GameState; // just in case

bool crearPartida (Partida* partida);
void destruirPartida (Partida* partida);

#endif // GAMESTATE_H_INCLUDED
