#include "./conexion.h"
#include <stdio.h>
#include <string.h>

int iniciarWinSock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET conectarAlServidor(const char *ipServidor, int puerto) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in dirServidor;
    dirServidor.sin_family = AF_INET;
    dirServidor.sin_port = htons(puerto);
    dirServidor.sin_addr.s_addr = inet_addr(ipServidor);

    if (sock == INVALID_SOCKET)
        return INVALID_SOCKET;

    if (connect(sock, (struct sockaddr *)&dirServidor, sizeof(dirServidor)) < 0) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

SOCKET abrirConexion() {

    SOCKET sock;

    if (iniciarWinSock() != 0) {
        printf("Error al inicializar Winsock.\n");
        return INVALID_SOCKET;
    }

    sock = conectarAlServidor(IP_SERVIDOR, PUERTO);

    if (sock == INVALID_SOCKET) {
        printf("No se pudo conectar al servidor.\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    printf("Conectado al servidor.\n");

    return sock;
}

void cerrarConexion(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}
