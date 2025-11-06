#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./conexion.h"
#include "./controladores/jugador.h"
#include "./controladores/partida.h"
#include "../libs/conexion/modelos.h"
#include "../libs/archBin/archBin.h"
#include "./peticiones.h"

void iniciarServidor() {

    SOCKET socketServidor, socketCliente;
    struct sockaddr_in dirCliente;
    int tamDirCliente = sizeof(dirCliente);

    tIndice jugadores;

    // Si alguno de los archivos binarios para persistencia no existe, lo crea nuevamente vacio.
    if (!verificarYRegenerarArchivos()) {
        printf("Error al regenerar los archivos.\n");
        system("pause");
        return;
    }

    crearIndice(&jugadores);

    // Carga el indice de jugadores. Si no se puede, intenta generar. Si falla, no inicia la conexión.
    if (cargarIndice(&jugadores, RUTA_JUGADORES_IDX_DAT, sizeof(JugadorIdx)) != OK) {

        generarIndice(
            &jugadores,
            RUTA_JUGADORES_DAT,
            RUTA_JUGADORES_IDX_DAT,
            sizeof(Jugador),
            sizeof(JugadorIdx),
            jugadorAIdx,
            cmpJugadorIdx
        );

        if (cargarIndice(&jugadores, RUTA_JUGADORES_IDX_DAT, sizeof(JugadorIdx)) != OK) {
            printf("Error al generar el indice de jugadores.\n");
            system("pause");
            return;
        }
    }

    mostrarIndice(&jugadores, imprimirJugadorIdx);

    if (iniciarWinSock() != 0) {
        printf("Error al inicializar Winsock.\n");
        system("pause");
        return;
    }

    socketServidor = crearSocket();

    if (socketServidor == INVALID_SOCKET) {
        printf("Error al crear socket del servidor.\n");
        system("pause");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puerto %d...\n", PUERTO);

    socketCliente = accept(socketServidor, (struct sockaddr *)&dirCliente, &tamDirCliente);

    if (socketCliente == INVALID_SOCKET) {
        printf("Error en accept()\n");
        system("pause");
        closesocket(socketServidor);
        WSACleanup();
        return;
    }

    printf("Cliente conectado.\n");

    // Recepcion y manejo de peticiones.
    recibirPeticiones(socketCliente, &jugadores);

    if (guardarIndice(&jugadores, RUTA_JUGADORES_IDX_DAT, sizeof(JugadorIdx)) != OK) {
        printf("Error al guardar los indices.\n");
        system("pause");
    }

    vaciarIndice(&jugadores);

    printf("Conexion cerrada.\n");

    closesocket(socketCliente);
    closesocket(socketServidor);
    WSACleanup();
}

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

bool verificarYRegenerarArchivos() {

    /*
     * Tanto las partidas como los rankings dependen de los jugadores. Si el archivo de jugadores
     * no existiese o estuviese corrupto, deberian reiniciarse tanto las partidas como los rankings.
     * De la misma forma, no puede haber indices si ya no hay jugadores.
    */
    if (!existeArchivoBin(RUTA_JUGADORES_DAT)) {
        if (crearArchivoBinVacio(RUTA_JUGADORES_DAT) == ERR_ARCHIVO)
            return false;

        if (crearArchivoBinVacio(RUTA_JUGADORES_IDX_DAT) == ERR_ARCHIVO)
            return false;

        if (crearArchivoBinVacio(RUTA_PARTIDAS_DAT) == ERR_ARCHIVO)
            return false;

        if (crearArchivoBinVacio(RUTA_RANKINGS_DAT) == ERR_ARCHIVO)
            return false;
    }

    if (!existeArchivoBin(RUTA_PARTIDAS_DAT)) {
        if (crearArchivoBinVacio(RUTA_PARTIDAS_DAT) == ERR_ARCHIVO)
            return false;
    }
    
    if (!existeArchivoBin(RUTA_RANKINGS_DAT)) {
        if (crearArchivoBinVacio(RUTA_RANKINGS_DAT) == ERR_ARCHIVO)
            return false;
    }

    return true;
}