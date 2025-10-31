#ifndef CONEXION_H_INCLUDED
#define CONEXION_H_INCLUDED

#include <winsock2.h>
#include "../../libs/Comun/comun.h"

#define IP_SERVIDOR "127.0.0.1"
#define PUERTO 6502

// Inicializa Winsock
int iniciarWinSock();

// Conecta al servidor y devuelve el socket
SOCKET conectarAlServidor(const char *ipServidor, int puerto);

// Engloba la logica de nueva conexion.
SOCKET abrirConexion();

// Cierra la conexion y limpia Winsock
void cerrarConexion(SOCKET sock);

#endif // CONEXION_H_INCLUDED
