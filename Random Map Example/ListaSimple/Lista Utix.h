#ifndef LISTA_UTIX_H_INCLUDED
#define LISTA_UTIX_H_INCLUDED

#include "Lista Simple.h"

void Print_Int(int *Dat,void *Par);

void Map_Lista(const t_lista *PL, void Act(const void*,void*),void* Par);
int Reduce_Lista(const t_lista *PL,void* Ret, void Act(const void*,void*,void*),void* Par);
int Filter_Lista(t_lista *PL, int Act(void*,void*),void *Par);


#endif // LISTA_UTIX_H_INCLUDED
