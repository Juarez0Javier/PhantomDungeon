#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <winsock2.h>
#include <SDL.h>
#include "./partida/partida.h"
#include "./configs/configs.h"
#include "../libs/graphics/graphics.h"
#include "../libs/conexion/modelos.h"

typedef struct {
    ConfigData configData;
    Partida partida;
    int seccion;
    unsigned deltaTime;
    SOCKET socket;
    unsigned idJugador;
    unsigned salteoRankings;
    unsigned cantRankings;
} tContextoGlobal;

#endif // GLOBAL_H_INCLUDED