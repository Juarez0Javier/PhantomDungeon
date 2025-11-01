#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <winsock2.h>
#include <SDL.h>
#include "./partida/partida.h"
#include "./configs/configs.h"
#include "../libs/graphics/graphics.h"

typedef struct {
    ConfigData configData;
    Partida partida;
    int seccion;
    unsigned deltaTime;
    SOCKET socket;
    unsigned idJugador;
} tContextoGlobal;

#endif // GLOBAL_H_INCLUDED
