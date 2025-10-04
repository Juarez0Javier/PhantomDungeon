#include "partida.h"

bool crearPartida (Partida* partida) {

    crearCola(&partida -> movs);

    if (!vectorCrear(&partida -> fantasmas, sizeof(Entidad))) {
        printf("Error de memoria creando el vector de fantasmas.\n");
        return false;
    }

    if (!crearMapa(&(partida -> mapa), CONFIG_FILAS, CONFIG_COLS)) {
        printf("Error de memoria creando el mapa.\n");
        vectorDestruir(&partida -> fantasmas);
        return false;
    }

    return true;
}

void destruirPartida (Partida* partida) {
    vaciarCola(&partida -> movs);
    vectorDestruir(&partida -> fantasmas);
    destruirMatriz(partida -> mapa.filas, (void*) partida -> mapa.data);
    destruirMatriz(partida -> mapa.filas, (void*) partida -> mapa.entidades);
}

