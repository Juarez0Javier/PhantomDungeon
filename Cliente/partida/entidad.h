#include <stddef.h>
#include "../../Comun/comun.h"

#ifndef ENTIDAD_H
#define ENTIDAD_H

typedef struct {
    char tipo;
    unsigned x, y, xInicial, yInicial;
    size_t ticksEntreMovs, ticksUltimoMov;
    bool eliminado;
} Entidad;

#endif