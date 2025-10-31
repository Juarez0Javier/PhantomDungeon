#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./conexion.h"
#include "./controladores/jugador.h"
#include "./controladores/partida.h"
#include "../libs/conexion/modelos.h"
#include "./peticiones.h"

int iniciarWinSock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET crearSocket() {
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == INVALID_SOCKET)
        return INVALID_SOCKET;

    struct sockaddr_in dirServidor;
    dirServidor.sin_family = AF_INET;
    dirServidor.sin_addr.s_addr = INADDR_ANY;
    dirServidor.sin_port = htons(PUERTO);

    if (bind(s, (struct sockaddr *)&dirServidor, sizeof(dirServidor)) == SOCKET_ERROR) {
        closesocket(s);
        return INVALID_SOCKET;
    }

    if (listen(s, 1) == SOCKET_ERROR) {
        closesocket(s);
        return INVALID_SOCKET;
    }

    return s;
}

void iniciarServidor() {

    char peticion[BUFFER_TAM];
    char respuesta[BUFFER_TAM];
    int bytesRecibidos;
    unsigned tamRespuesta;
    bool err = false;

    SOCKET socketServidor, socketCliente;
    struct sockaddr_in dirCliente;
    int tamDirCliente = sizeof(dirCliente);

    tArbol jugadores;
    tCola peticiones;
    tSecuencia sec;

    // Lo primero que se hace es crear el arbol para el indice de jugadores. Si no se puede, no inicia la conexión.
    crearArbol(&jugadores);

    err = cargarArbolDeArchBin(
        &jugadores,
        RUTA_JUGADORES_IDX_DAT,
        sizeof(JugadorIdx),
        cmpJugadorIdx,
        NULL,
        NULL
    ) != OK;

    if (err) {
        printf("Error al crear el arbol de jugadores.\n");
        return;
    }

    if (iniciarWinSock() != 0) {
        printf("Error al inicializar Winsock.\n");
        return;
    }

    socketServidor = crearSocket();

    if (socketServidor == INVALID_SOCKET) {
        printf("Error al crear socket del servidor.\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puerto %d...\n", PUERTO);

    socketCliente = accept(socketServidor, (struct sockaddr *)&dirCliente, &tamDirCliente);

    if (socketCliente == INVALID_SOCKET) {
        printf("Error en accept()\n");
        closesocket(socketServidor);
        WSACleanup();
        return;
    }

    printf("Cliente conectado.\n");

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

            tamRespuesta = procesarPeticion(peticion, respuesta, &jugadores, &sec);

            // Si la peticion genera una secuencia de elementos a enviar, se procesan.
            if (!listaVacia(&sec.elementos)) {

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
    vaciarArbol(&jugadores);

    // Nota: No se sobreescribe el idx de jugadores ya que este ya se guarda ordenado. Ver funcion guardarJugador.

    printf("Conexion cerrada.\n");
    
    closesocket(socketCliente);
    closesocket(socketServidor);
    WSACleanup();
}
