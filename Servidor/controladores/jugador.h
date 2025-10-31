#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include "../constantes.h"
#include "./comun.h"

unsigned crearJugador(char* actPet, char* respuesta, tArbol* jugadores);
unsigned buscarJugador(char* actPet, char* respuesta, tArbol* jugadores);

#endif // JUGADOR_H_INCLUDED
