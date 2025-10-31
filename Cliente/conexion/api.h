#ifndef API_H_INCLUDED
#define API_H_INCLUDED

#include "../../libs/Comun/comun.h"
#include "../../libs/conexion/transporte.h"
#include "../../libs/conexion/modelos.h"

bool apiCrearJugador(SOCKET sock, unsigned* id, const char* nombre, const char* email, const char* contrasenia);
bool apiBuscarJugador(SOCKET sock, Jugador* jug, unsigned char opcion, unsigned id, const char* nombre);
bool apiCrearPartida(SOCKET sock, unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs);
bool apiObtenerRankings(SOCKET sock, unsigned limite, unsigned saltear, Acc procesarRanking);

#endif // API_H_INCLUDED
