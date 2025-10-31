#ifndef TRANSPORTE_H_INCLUDED
#define TRANSPORTE_H_INCLUDED

#include <winsock2.h>

unsigned recibirExacto(SOCKET sock, void* buffer, unsigned tam);
unsigned enviarExacto(SOCKET sock, const void* buffer, unsigned tam);
void cargarCampoStr(const char* campo, char** buffer);
void cargarCampo(const void* campo, unsigned tamCampo, char** buffer);
void leerCampo(void* campo, unsigned tamCampo, char** buffer);
void leerCampoStr(char* campo, char** buffer);

#endif // TRANSPORTE_H_INCLUDED
