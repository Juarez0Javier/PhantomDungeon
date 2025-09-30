#include "./matriz.h"

void** crearMatriz(unsigned rows, unsigned cols, unsigned len) {
    void** rv = malloc(sizeof(void*) * rows);
    if (!rv) {
        printf("\nMalloc Error. There is no memory for the field.");
        return NULL;
    }
    for (unsigned i=0; i<rows; i++) {
        *((char**)rv+i) = malloc(len * cols);
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