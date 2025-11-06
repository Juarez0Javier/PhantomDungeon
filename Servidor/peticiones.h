#ifndef PETICIONES_H_INCLUDED
#define PETICIONES_H_INCLUDED

#include "./constantes.h"

typedef struct {
    unsigned tamElem;
    tLista elementos;
} tSecuencia;

void recibirPeticiones(SOCKET socketCliente, tIndice* jugadores);
unsigned procesarPeticion(const char *peticion, char *respuesta, tIndice* jugadores, tSecuencia* sec);

#endif // PETICIONES_H_INCLUDED
