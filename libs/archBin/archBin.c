#include <stdlib.h>
#include <string.h>
#include "./archBin.h"


bool existeArchivoBin(const char* nomArch) {
    FILE* arch = fopen(nomArch, "rb");

    if (!arch)
        return false;

    fclose(arch);
    return true;
}

int crearArchivoBinVacio(const char* nomArch) {

    FILE* arch = fopen(nomArch, "wb");

    if (!arch)
        return ERR_ARCHIVO;

    fclose(arch);

    return OK;
}

void mostrarArchivoBin(FILE* arch, unsigned tamReg, Imp imp) {

    void* reg = malloc(tamReg);

    rewind(arch);

    if (reg == NULL)
        return;

    fseek(arch, 0, SEEK_SET);

    while (fread(reg, tamReg, 1, arch))
        imp(reg);

    free(reg);
}

int buscarEnArchivoBin(FILE* arch, void* busq, unsigned tamReg, Cmp cmp) {
    int pos = -1;
    void* reg = malloc(tamReg);

    if (reg == NULL)
        return -1;

    fseek(arch, 0, SEEK_SET);

    while (pos == -1 && fread(reg, tamReg, 1, arch)) {
        if (cmp(busq, reg) == 0)
            pos = (ftell(arch) / tamReg) -1;
    }

    if (pos != -1)
        memcpy(busq, reg, tamReg);

    free(reg);
    return pos;
}
