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

    SOCKET socketServidor, socketCliente;
    struct sockaddr_in dirCliente;
    int tamDirCliente = sizeof(dirCliente);

    tArbol jugadores;
    int cod;

    // Lo primero que se hace es crear el arbol para el indice de jugadores. Si no se puede, no inicia la conexión.
    crearArbol(&jugadores);

    cod = cargarArbolDeArchBin(&jugadores, RUTA_JUGADORES_IDX_DAT, sizeof(JugadorIdx), cmpJugadorIdx, NULL, NULL);

    if (cod != OK) {
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

    // Recepcion y manejo de peticiones.
    recibirPeticiones(socketCliente, &jugadores);

    // Nota: No se sobreescribe el idx de jugadores ya que este ya se guarda ordenado. Ver funcion guardarJugador.
    vaciarArbol(&jugadores);

    printf("Conexion cerrada.\n");
    
    closesocket(socketCliente);
    closesocket(socketServidor);
    WSACleanup();
}
