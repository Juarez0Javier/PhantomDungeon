#include "Pila_Dinamica.h"

void crearPilaD(tPilaD* pila)
{
    *pila = NULL;
}

int apilarD(tPilaD* pila, const void* info, unsigned tamInfo)
{
    if(pilaDLlena(pila,tamInfo))
        return 0;

    tNodo* nueNodo = (tNodo*)malloc(sizeof(tNodo));
    void* nueinfo = malloc(tamInfo);

    memcpy(nueinfo,info,tamInfo);

    nueNodo->info = nueinfo;
    nueNodo->tamInfo = tamInfo;
    nueNodo->sig = (*pila);

    (*pila) = nueNodo;

    return 1;
}
int desapilarD(tPilaD* pila, void* info, unsigned tamInfo)
{
    if(pilaDVacia(pila))
        return 0;

    tNodo* auxNodo = *pila;

    memcpy(info,auxNodo->info,MIN(tamInfo,auxNodo->tamInfo));

    (*pila) = (*pila)->sig;

    free(auxNodo->info);
    free(auxNodo);

    return 1;
}

int verTopeD(tPilaD* pila, void* info, unsigned tamInfo)
{
    if(pilaDVacia(pila))
        return 0;

    memcpy(info,(*pila)->info,MIN(tamInfo,(*pila)->tamInfo));

    return 1;
}

int pilaDLlena(tPilaD* pila, unsigned tamInfo)
{
    tNodo* auxNodo = (tNodo*)malloc(sizeof(tNodo));
    void* auxinfo = malloc(tamInfo);

    free(auxNodo);
    free(auxinfo);

    return (!auxNodo || !auxinfo);

}
int pilaDVacia(tPilaD* pila)
{
    return *pila == NULL;
}

void vaciarPilaD(tPilaD* pila)
{
    while ((*pila)!=NULL)
    {
        tNodo* auxNodo = *pila;

        (*pila) = (*pila)->sig;

        free(auxNodo->info);
        free(auxNodo);
    }
}

