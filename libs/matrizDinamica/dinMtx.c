#include "dinMtx.h"

void** crearMatriz(unsigned rows, unsigned cols, unsigned len) {
    void** rv = calloc(rows, sizeof(void*));
    if (!rv) {
        printf("\nMalloc Error. There is no memory for the field.");
        return NULL;
    }
    for (unsigned i=0; i<rows; i++) {
        *((char**)rv+i) = calloc(cols, len);
        if (!*(rv+i)) {
            printf("\nMalloc Error. There is no memory for the field.");
            for (unsigned j=0; j<i; j++)
                free(*((char**)rv+j));
            free(rv);
            return NULL;
        }
    }
    return rv;
}

void destruirMatriz(unsigned rows, void** mtx) {
    for (unsigned i=0; i<rows; i++) {
        free(*((char**)mtx+i));
    }
    free(mtx);
}

void copiarMatriz(void** dst, void** src, unsigned filas, unsigned cols, unsigned len) {
    for (unsigned i=0; i<filas; i++)
        for (unsigned j=0; j<cols; j++)
            memcpy(dst[i]+j, src[i]+j, len);
}

