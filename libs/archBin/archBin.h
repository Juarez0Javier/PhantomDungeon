#ifndef ARCHBIN_H_INCLUDED
#define ARCHBIN_H_INCLUDED

#include <stdio.h>
#include "../Comun/comun.h"

bool existeArchivoBin(const char* nomArch);
int crearArchivoBinVacio(const char* nomArch);
void mostrarArchivoBin(FILE* arch, unsigned tamReg, Imp imp);
int buscarEnArchivoBin(FILE* arch, void* busq, unsigned tamReg, Cmp cmp);

#endif // ARCHBIN_H_INCLUDED
