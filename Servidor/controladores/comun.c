#include "./comun.h"
#include <stdio.h>
#include <string.h>

// Valida que un campo string dado tenga el formato correcto.
bool validarCampoStr(const char* campo, unsigned tamMax) {

    unsigned tam;

    if (!campo)
        return false;

    tam = strlen(campo);

    if (tam == 0)
        return false;

    if (tam > tamMax)
        return false;

    return true;
}