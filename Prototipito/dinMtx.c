#include "dinMtx.h"

void** newDinMtx(unsigned rows, unsigned cols, unsigned len) {
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

void destroyDinMtx(unsigned rows, unsigned cols, unsigned len, void** mtx) {
    for (unsigned i=0; i<rows; i++) {
        free(*((char**)mtx+i));
    }
    free(mtx);
}

void** newDinMtxSimple(unsigned rows, unsigned cols, unsigned len) {
    void** rv = malloc(rows*cols*len);
    if (!rv)
        return NULL;
    return rv;
}
