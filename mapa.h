#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "dinMtx.h"
#include "Vector.h"
#include "entidades.h"

typedef struct {
    int filas;
    int cols;
    char** data;
} Mapa;

typedef struct {
    int filas;
    int cols;
    Entidad** data;
} MapaEntidades;

int crearMapa(Mapa* mapa, int filas, int cols);
void mostrarMapa(Mapa* mapa, Entidad* jugador, MapaEntidades* mapaEntidades);
void vaciarMapa(Mapa* mapa);
int cargarMapaDeArchivo(Mapa* mapa, char* rutaArch, Entidad* jugador, Vector* entidades);
char colisionConMapa(Entidad* entidad, Mapa* mapa, char direccion);

int moverEntidad(Entidad* entidad, Mapa* mapa, char direccion);

int crearMapaEntidadesVacio(MapaEntidades* mapaEntidades, int filas, int cols);
void mostrarMapaEntidades(MapaEntidades* mapaEntidades);
void llenarMapaConEntidades(MapaEntidades* mapaEntidades, Vector* entidades);
Entidad* colisionConEntidad(Entidad* entidad, MapaEntidades* mapaEntidades, char direccion);

// TODO entities will be in vectors !!!



int cambioSaltoLineaPorFinCadena(char* cadena);


#endif // MAPA_H_INCLUDED
