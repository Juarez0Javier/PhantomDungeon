#include <stdlib.h>
#include <string.h>
#include "./archBin.h"

void mostrarArchivoBin(FILE* arch, unsigned tamReg, Imp imp) {

    void* reg = malloc(tamReg);

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
