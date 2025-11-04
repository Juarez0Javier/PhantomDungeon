#include <stdio.h>
#include "./peticiones.h"
#include "./controladores/jugador.h"
#include "./controladores/partida.h"
#include "./controladores/ranking.h"
#include "../libs/conexion/transporte.h"

// Recibe las peticiones, las encola para resolver y envia la respuesta.
void recibirPeticiones(SOCKET socketCliente, tArbol* jugadores) {

    char peticion[BUFFER_TAM];
    char respuesta[BUFFER_TAM];
    int bytesRecibidos;
    unsigned tamRespuesta;
    bool err = false;

    tCola peticiones;
    tSecuencia sec;

    // Ya al poder conectarse el cliente si crea la cola de peticiones y la lista de secuencias.
    crearLista(&sec.elementos);
    crearCola(&peticiones);

    bytesRecibidos = recv(socketCliente, peticion, BUFFER_TAM, 0);

    while (!err && bytesRecibidos > 0) {

        err = !ponerEnCola(&peticiones, peticion, bytesRecibidos);

        /*
         * Aca probablemente iria alguna logica con hilos o algo asi si tuviesemos mas clientes,
           pero como no tenemos, lo relleno con un chiste malo sobre redes:
           ¿Que le dijo TCP a UDP?: ¿A donde con tan poca fiabilidad?
           ¿Que le respondio UDP? (No sabemos, su respuesta nunca llego)
        */

        while (sacarDeCola(&peticiones, peticion, bytesRecibidos)) {

            tamRespuesta = procesarPeticion(peticion, respuesta, jugadores, &sec);

            // Si la peticion genera una secuencia de elementos a enviar, se procesan.
            if (*respuesta == INICIO_SECUENCIA) {

                // Envia el mensaje de inicio de secuencia + la cantidad de elementos.
                send(socketCliente, respuesta, tamRespuesta, 0);

                // Marca el envio de datos de la secuencia. Envía hasta quedarse sin elementos.
                *respuesta = ENVIO_SECUENCIA;
                tamRespuesta = 1 + sec.tamElem;

                /*
                * Nota: Dado que la lista de elementos de secuencia se esta manejando como una cola
                se podria reemplazar por una cola, pero el manejo de carga de secuencias seria menos eficiente
                (ver funcion prepararRankings).
                */
                while (sacarDeListaAlInicio(&sec.elementos, respuesta +1, sec.tamElem))
                    send(socketCliente, respuesta, tamRespuesta, 0);

                // Una vez termina, marca el fin de secuencia y termina la transmisión de la misma.
                *respuesta = FIN_SECUENCIA;
                tamRespuesta = 1;
            }

            send(socketCliente, respuesta, tamRespuesta, 0);
        }

        bytesRecibidos = recv(socketCliente, peticion, BUFFER_TAM, 0);
    }

    if (err)
        printf("Error critico, memoria insuficiente en cola de peticiones. Cerrando conexion...\n");

    // Se vacian todas las estructuras de datos.
    vaciarLista(&sec.elementos);
    vaciarCola(&peticiones);
}

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
