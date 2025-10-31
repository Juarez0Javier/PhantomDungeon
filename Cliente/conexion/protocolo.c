#include "./protocolo.h"
#include "../../libs/Comun/conexion.h"
#include "../../libs/conexion/transporte.h"
#include <string.h>

// Envia una solicitud y recibe la respuesta.
bool enviarPeticion(SOCKET sock, unsigned cantBytes, const char *peticion, char *respuesta) {
    int bytesRecibidos;

    if (send(sock, peticion, cantBytes, 0) < 0)
        return false;

    bytesRecibidos = recv(sock, respuesta, TAM_BUFFER, 0);

    if (bytesRecibidos <= 0)
        return false;

    return true;
}

bool iniciarSecuencia(SOCKET sock, char* respuesta, unsigned *cantElem) {
    unsigned char cod;
    
    leerCampo(&cod, sizeof(cod), &respuesta);

    if (cod != INICIO_SECUENCIA)
        return false;

    leerCampo(cantElem, sizeof(*cantElem), &respuesta);

    return true;
}

bool siguienteSecuencia(SOCKET sock, void *elem, unsigned tamElem) {
    unsigned char cod;

    // Leo si viene un elemento o fin.
    recibirExacto(sock, &cod, sizeof(cod));

    if (cod == FIN_SECUENCIA)
        return false;

    // Leo el elemento completo.
    recibirExacto(sock, elem, tamElem);

    return true;
}
