#include "./peticiones.h"
#include "./controladores/jugador.h"
#include "./controladores/partida.h"
#include "./controladores/ranking.h"
#include "../libs/conexion/transporte.h"

// Procesa las peticiones recibidas. Devuelve el tamaño en bytes de la respuesta.
unsigned procesarPeticion(const char *peticion, char *respuesta, tArbol* jugadores, tSecuencia* sec) {

    char* actPet = (char*) peticion;
    unsigned char codOperacion;
    unsigned tamRespuesta;

    // Devuelve error de formato si no hay codigo de operacion.
    if (!peticion || *peticion == '\0') {
        *respuesta = ERR_FORMATO;
        tamRespuesta = 1;
        return tamRespuesta;
    }

    leerCampo(&codOperacion, sizeof(codOperacion), &actPet);

    switch (codOperacion) {
        case CREAR_JUGADOR:
            tamRespuesta = crearJugador(actPet, respuesta, jugadores);
            break;

        case CREAR_PARTIDA:
            tamRespuesta = crearPartida(actPet, respuesta);
            break;

        case BUSCAR_JUGADOR:
            tamRespuesta = buscarJugador(actPet, respuesta, jugadores);
            break;

        case OBTENER_RANKINGS:
            tamRespuesta = obtenerRankings(actPet, respuesta, jugadores, sec);
            break;

        default:
            *respuesta = OP_DESCONOCIDA;
            tamRespuesta = 1;
    }

    return tamRespuesta;
}