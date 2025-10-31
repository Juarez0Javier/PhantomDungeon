#include "./comun.h"
#include <stdio.h>
#include <string.h>

// Valida que un campo string dado tenga el formato correcto.
bool validarCampoStr(const char* campo, int tam) {
    if (!campo)
        return false;

    if (strlen(campo) > tam)
        return false;

    return true;
}