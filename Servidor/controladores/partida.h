#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "../constantes.h"
#include "./comun.h"

unsigned crearPartida (char* actPet, char* respuesta);
int guardarPartida(unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs);
void imprimirPartida(const void* elem);

#endif // PARTIDA_H_INCLUDED
