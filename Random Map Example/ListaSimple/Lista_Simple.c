#include "Lista_Simple.h"

void crearListaS (tListaS* lista)
{
    (*lista) = NULL;
}

int ponerPriListaS (tListaS* lista, const void* dato, size_t tam)
{
    tNodo *nuevo;

    if(listaSLlena(lista,tam))
        return 0;

    nuevo = (tNodo*)malloc(sizeof(tNodo));
    nuevo->dato = malloc(tam);

    memcpy(nuevo->dato,dato,tam);

    nuevo->tam = tam;
    nuevo->sig = (*lista);

    (*lista) = nuevo;

    return 1;
}
int sacarPriListaS (tListaS* lista, void* dato, size_t tam)
{
    tNodo* aux = (*lista);

    if(aux==NULL)
        return 0;

    memcpy(dato,(*lista)->dato,MIN(tam,aux->tam));

    (*lista) = aux->sig;

    free(aux->dato);
    free(aux);

    return 1;
}
int verPriListaS (tListaS* lista, void* dato, size_t tam)
{
    if(listaSVacia(lista))
        return 0;

    memcpy(dato,(*lista)->dato,MIN(tam,(*lista)->tam));

    return 1;
}

int ponerUltiListaS (tListaS* lista, const void* dato, size_t tam)
{
    tNodo* nuevo;

    if(listaSLlena(lista,tam))
        return 0;

    while (*lista)
        lista = &(*lista)->sig;


    nuevo = (tNodo*)malloc(sizeof(tNodo));
    nuevo->dato = malloc(tam);

    memcpy(nuevo->dato,dato,tam);

    nuevo->tam = tam;
    nuevo->sig = NULL;

    (*lista) = nuevo;
    return 1;
}
int sacarUltiListaS (tListaS* lista, void* dato, size_t tam)
{
    tNodo* aux;

    if(listaSVacia(lista))
        return 0;

    while ((*lista)->sig)
        lista = &((*lista)->sig);

    memcpy(dato,(*lista)->dato,MIN(tam,aux->tam));

    free((*lista)->dato);
    free((*lista));

    return 1;
}
int verUltiListaS (tListaS* lista, void* dato, size_t tam)
{
    if(listaSVacia(lista))
        return 0;

    while ((*lista)->sig)
        lista = &(*lista)->sig;

    memcpy(lista,&(*lista)->tam,MIN(tam,(*lista)->tam));

    return 1;
}

int listaSLlena (const tListaS* lista, size_t tam)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));
    void* dato = malloc(tam);
    free(aux);
    free(dato);
    return !(aux == NULL) || !(dato == NULL);
}
int listaSVacia (const tListaS* lista)
{
    return (*lista) == NULL;
}

void vaciarListaS (tListaS* lista)
{
    while (*lista)
    {
        tNodo *aux = *lista;
        (*lista) = aux->sig;
        free(aux->dato);
        free(aux);
    }
}

/*
int Put_in_Order(tListaS *PL, const void *PD, size_t Tam, int (*Cmp)(const void*,const void*))
{
    t_nodo *New;
    while((*PL) && Cmp((*PL)->Dat,PD)<0)
        PL = &(*PL)->Next;
    if(Cmp((*PL)->Dat,PD)==0)
        return 0;
    if((New = (t_nodo*)malloc(sizeof(t_nodo))) == NULL || (New->Dat = malloc(Tam)) == NULL)
    {
        free(New->Dat);
        free(New);
        return -1;
    }
    memcpy(New->Dat,PD,Tam);
    New->Tam=Tam;
    New->Next=(*PL);
    (*PL) = New;
    return 1;
}

int Order_lista(tListaS *PL, int (*Cmp)(const void*,const void*))
{
    tListaS *Fir=PL;

    if(*PL == NULL)
        return 0;


    while((*PL)->Next)
    {
        if(Cmp((*PL)->Dat,(*PL)->Next->Dat)>0)
        {
            tListaS* Curr = Fir;
            t_nodo* Aux =(*PL)->Next;

            (*PL)->Next=Aux->Next;

            while(Cmp((*Curr)->Dat,Aux->Dat)>0)
                Curr=&(*Curr)->Next;
            Aux->Next=*Curr;
            *Curr=Aux;
        }
        else
            PL=&(*PL)->Next;
    }
    return 1;
}

void Map_Lista(const tListaS *PL, void(*Act)(const void*,void*),void* Par)
{
    while (*PL)
    {
        Act((*PL)->Dat,Par);
        PL=&(*PL)->Next;
    }
}

int Reduce_Lista(const tListaS *PL,void* Ret, void(*Act)(const void*,void*,void*),void* Par)
{
    while (*PL)
    {
        Act((*PL)->Dat,Ret,Par);
        PL=&(*PL)->Next;
    }
    return 1;
}

tListaS* Filter_Lista(tListaS *PL, int(*Act)(void*,void*),void *Par)
{
    t_nodo* Aux = *PL;
    tListaS* New = PL;
    int First=0;
    while (*PL)
    {
        if(Act((*PL)->Dat,Par))
        {
            if(Aux->Next==(*PL))
               Aux->Next=Aux->Next->Next;
            else
            {
                Aux=Aux->Next;
                First++;
            }
            t_nodo* Eli=*PL;
            *PL=Eli->Next;
            if(First!=0)
                New=PL;
            free(Eli->Dat);
            free(Eli);
        }
        else
        {
            PL=&(*PL)->Next;
        }
    }
    return New;
}*/
