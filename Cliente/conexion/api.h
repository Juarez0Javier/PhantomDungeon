#ifndef API_H_INCLUDED
#define API_H_INCLUDED

#include "../../libs/Comun/comun.h"
#include "../../libs/conexion/transporte.h"
#include "../../libs/conexion/modelos.h"

int apiCrearJugador(SOCKET sock, unsigned* id, const char* nombre, const char* email, const char* contrasenia);
int apiBuscarJugador(SOCKET sock, Jugador* jug, unsigned char opcion, unsigned id, const char* nombre);
int apiCrearPartida(SOCKET sock, unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs);
int apiObtenerRankings(SOCKET sock, unsigned* cantObj, unsigned limite, unsigned saltear, Acc procesarRanking, void* extraAcc);

#endif // API_H_INCLUDED
