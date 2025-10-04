#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include "../constants.h"
#include "../entidades/entidades.h"

typedef struct {
    int filas;
    int cols;
    char** data;
    Entidad*** entidades;
} Mapa;


bool crearMapa(Mapa* mapa, int filas, int cols);
void mostrarMapa (Mapa* mapa);
void vaciarMapa(Mapa* mapa);
int cargarMapaDeArchivoNoSeguro(Mapa* mapa, char* rutaArch, Entidad* jugador, Vector* entidades);
bool cargarMapaDeArchivo (Mapa* mapa, Entidad* jugador, Vector* fantasmas, const char* nomArch);
char colisionConMapa(Entidad* entidad, Mapa* mapa, char direccion);


int moverEntidad(Entidad* entidad, Mapa* mapa, char direccion);
void reestablecerPosInicial (Entidad* e, Mapa* mapa);

int cambioSaltoLineaPorFinCadena(char* cadena);

void actualizarMapaRender(SDL_Renderer* renderer, Mapa* mapa, GHP_TexturesData* tex, GHP_Mesh mesh);

#endif // MAPA_H_INCLUDED
