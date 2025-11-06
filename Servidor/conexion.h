#ifndef CONEXION_H_INCLUDED
#define CONEXION_H_INCLUDED

#include "./constantes.h"

// Inicializa Winsock
int iniciarWinSock();

// Crea el socket del servidor
SOCKET crearSocket();

// Ejecuta el bucle principal del servidor
void iniciarServidor();

// Si alguno de los archivos binarios para persistencia no existe, lo crea nuevamente vacio.
bool verificarYRegenerarArchivos();

#endif // CONEXION_H_INCLUDED
