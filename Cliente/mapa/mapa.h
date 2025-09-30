#include "../../Comun/comun.h"
#include "../partida/entidad.h"
#include "../../Vector/Vector.h"

#ifndef MAPA_H
#define MAPA_H

typedef struct {
    int filas;
    int cols;
    char** terreno;
    Entidad*** entidades;
} Mapa;

bool crearMapa (Mapa* mapa, int filas, int cols);
bool cargarMapaDeArchivo (Mapa* mapa, Entidad* jugador, Vector* fantasmas, const char* nomArch);
void mostrarMapa (Mapa* mapa);

#endif