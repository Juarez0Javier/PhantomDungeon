#ifndef PROTOCOLO_H_INCLUDED
#define PROTOCOLO_H_INCLUDED

#define TAM_BUFFER 1024

#include <winsock2.h>
#include "../../libs/Comun/comun.h"

bool enviarPeticion(SOCKET sock, unsigned cantBytes, const char *peticion, char *respuesta);
int iniciarSecuencia(SOCKET sock, char* respuesta, unsigned *cantObj);
int siguienteSecuencia(SOCKET sock, void *elem, unsigned tamElem);

#endif // PROTOCOLO_H_INCLUDED
