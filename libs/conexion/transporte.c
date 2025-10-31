#include "./transporte.h"

// Recibe del cliente o del servidor los datos que necesita, iterando hasta que los mismos lleguen completos.
unsigned recibirExacto(SOCKET sock, void* buffer, unsigned tam) {
    unsigned total = 0, recibido;

    if (total >= tam)
        return total;

    do {
        recibido = recv(sock, (char*)buffer + total, tam - total, 0);
        total += recibido;
    } while (recibido > 0 && total < tam);

    return total;
}

// Envia al cliente o al servidor los datos que debe, iterando hasta que los mismos lleguen completos.
unsigned enviarExacto(SOCKET sock, const void* buffer, unsigned tam) {
    unsigned total = 0, enviado;

    if (total >= tam)
        return total;

    do {
        enviado = send(sock, (char*)buffer + total, tam - total, 0);
        total += enviado;
    } while (enviado > 0 && total < tam);

    return total;
}

// Carga el tamanio y el string en la peticion o la respuesta.
void cargarCampoStr(const char* campo, char** buffer) {
    unsigned tam = strlen(campo);
    memcpy(*buffer, &tam, sizeof(unsigned));
    *buffer += sizeof(unsigned);
    strncpy(*buffer, campo, tam);
    *buffer += tam;
}

// Carga el campo en la peticion.
void cargarCampo(const void* campo, unsigned tamCampo, char** buffer) {
    memcpy(*buffer, campo, tamCampo);
    *buffer += tamCampo;
}

// Lee un campo de la peticion o la respuesta.
void leerCampo(void* campo, unsigned tamCampo, char** buffer) {
    memcpy(campo, *buffer, tamCampo);
    *buffer += tamCampo;
}

// Lee el tamanio y el string en la peticion o la respuesta.
void leerCampoStr(char* campo, char** buffer) {
    unsigned tam;
    memcpy(&tam, *buffer, sizeof(unsigned));
    *buffer += sizeof(unsigned);
    strncpy(campo, *buffer, tam);
    *buffer += tam;
}
