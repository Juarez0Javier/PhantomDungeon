#include "./string.h"
#include "../Comun/comun.h"
#include <string.h>

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