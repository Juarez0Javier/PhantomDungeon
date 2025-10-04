#include "entidades.h"

int cmpEntidad(void* a, void* b) {
    Entidad* a2 = (Entidad*)a;
    Entidad* b2 = (Entidad*)b;
    return a2->tipo - b2->tipo != 0 ? a2->tipo - b2->tipo : a2->num - b2->num;
}

int cmpTipoEntidad(void* a, void* b) {
    Entidad* a2 = (Entidad*)a;
    Entidad* b2 = (Entidad*)b;
    return a2->tipo - b2->tipo;
}

int cmpPosEntidad(void* a, void* b) {
    Entidad* a2 = (Entidad*)a;
    Entidad* b2 = (Entidad*)b;
    return (a2->x == b2->x && a2->y == b2->y);
}

Entidad entidadNula() {
    Entidad rv = {'\0', -1, -1, -1};
    return rv;
}

void copiarEntidad(Entidad* dst, Entidad* src) {
    memcpy((void*)dst, (void*)src, sizeof(Entidad));
}

void imprimirEntidad(Entidad e) {
    printf("<Entidad %c %d (%d,%d)>", e.tipo, e.num, e.x, e.y);
}

void imprimirEntidadMini(Entidad e) {
    if (e.tipo=='\0')
        printf("<ENT VACIA    >");
    else
        printf("<ENT %c%-2d (%d,%d)>", e.tipo, e.num, e.x, e.y);
}
