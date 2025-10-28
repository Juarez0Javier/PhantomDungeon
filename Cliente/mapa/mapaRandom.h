#ifndef MAPARANDOM_H_INCLUDED
#define MAPARANDOM_H_INCLUDED

#include "../configs/configs.h"


#include"../../libs/Pila/Dina/Pila_Dinamica.h"

typedef struct
{
    int x;
    int y;

} tVector2;

int generarMapaRandom(ConfigData* configData, char nombreArch[]);

tVector2 dirRandom (int cardBloq[4]);
tVector2 posCaminoRandom (int columnas, int filas, char mapa[filas][columnas]);

char itemRandom (int* fantasmas, int* premios, int* vidas);

void printMapa(int columnas, int filas, char mapa[filas][columnas]);
void printMapaOnTxt(int columnas, int filas, char mapa[filas][columnas], FILE** file);

int vector2ACard(tVector2 vec);
tVector2 cardaVector2(int card);

#endif // MAPARANDOM_H_INCLUDED
