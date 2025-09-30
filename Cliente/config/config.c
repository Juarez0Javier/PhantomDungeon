#include "./config.h"
#include "../constantes.h"
#include "../partida/entidad.h"
#include "../../Vector/Vector.h"
#include "../../Cola/Cola.h"
#include "../auxiliares/matriz/matriz.h"


bool crearConfigPartida (Partida* partida) {

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

void destruirConfigPartida (Partida* partida) {
    vaciarCola(&partida -> movs);
    vectorDestruir(&partida -> fantasmas);
    destruirMatriz(partida -> mapa.filas, (void*) partida -> mapa.terreno);
    destruirMatriz(partida -> mapa.filas, (void*) partida -> mapa.entidades);
}
