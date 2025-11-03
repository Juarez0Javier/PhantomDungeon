#include "./string.h"
#include "../Comun/comun.h"
#include <string.h>
#include <stdlib.h>

void leerStr(char* str, unsigned tam, FILE* iostream) {
    char* act = str;

    if (str == NULL)
        return;

    // Aniade el caracter para el \0.
    fgets(str, tam +1, iostream);

    while (*act != '\0' && *act != '\n')
        act++;

    *act = '\0';
}

void trimStr(char* str) {

    char* ini = str;
    char* fin;

    if (str == NULL)
        return;

    while (*ini != '\0' && (*ini == ' ' || *ini == '\t'))
        ini++;

    if (*ini == '\0') {
        *str = '\0';
        return;
    }

    fin = str + strlen(str);

    while (fin > ini && (*(fin -1) == ' ' || *(fin -1) == '\t'))
        fin--;

    *fin = '\0';

    memcpy(str, ini, fin - ini +1);
}

bool validarStrNoVacio(const char* str, unsigned tam) {

    char* recortado;
    bool validez;

    if (!str)
        return false;

    recortado = malloc(tam +1);

    if (!recortado)
        return false;

    strncpy(recortado, str, tam);
    recortado[tam] = '\0';

    trimStr(recortado);
    validez = strlen(recortado) > 0;

    free(recortado);
    return validez;
}

void ingrStrNoVacio(char* str, unsigned tam, const char* msjError) {

    if (str == NULL)
        return;

    do {
        leerStr(str, tam, stdin);
        trimStr(str);

        if (*str == '\0' && msjError)
            printf(msjError);

    } while (*str == '\0');

}
