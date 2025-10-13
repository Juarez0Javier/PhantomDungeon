#include "Cola_Dinamica.h"

void crearColaD(tColaD* cola)
{
    cola->prim = cola->ulti = NULL;
}

int ponerEnColaD(tColaD* cola, const void* dato, size_t tam)
{
    tNodo* nueNodo = (tNodo*)malloc(sizeof(tNodo));

    if(!nueNodo)
        return 0;

    nueNodo->dato = malloc(tam);

    if(!nueNodo->dato)
    {
        free(nueNodo);
        return 0;
    }

    memcpy(nueNodo->dato,dato,tam);

    nueNodo->tam = tam;
    nueNodo->sig = NULL;

    if(cola->ulti)
        cola->ulti->sig = nueNodo;
    else
        cola->prim = nueNodo;
    cola->ulti = nueNodo;

    return 1;
}

int sacarDeColaD(tColaD* cola, void* dato, size_t tam)
{
    tNodo* auxNodo = cola->prim;

    if((cola->prim == NULL) && (cola->ulti == NULL))
        return 0;

    memcpy(dato,auxNodo->dato,MIN(auxNodo->tam,tam));

    cola->prim = auxNodo->sig;

    if(cola->prim == NULL)
        cola->ulti = NULL;

    free(auxNodo->dato);
    free(auxNodo);

    return 1;
}

int verPrimeroColaD(tColaD* cola, void* dato, size_t tam)
{
    if(colaDVacia(cola))
        return 0;

    memcpy(dato,cola->prim->dato,MIN(tam,cola->prim->tam));

    return 1;
}

int colaDLlena(tColaD* cola, size_t tam)
{
    tNodo* auxNodo = (tNodo*)malloc(sizeof(tNodo));
    void* auxDato = malloc(tam);

    free(auxNodo);
    free(auxDato);

    return (!auxNodo || !auxDato);
}
int colaDVacia(tColaD* cola)
{
  return (cola->prim == NULL) && (cola->ulti == NULL);
}

void vaciarColaD(tColaD* cola)
{
    while (cola->prim!=NULL)
    {
        tNodo* auxNodo = cola->prim;

        cola->prim = cola->prim->sig;

        free(auxNodo->dato);
        free(auxNodo);
    }

    cola->ulti = NULL;
}
