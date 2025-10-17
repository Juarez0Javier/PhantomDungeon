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
void dirRandom (tVector2* dir, int cardInval[4]);

void printMapa(int columnas, int filas, char mapa[filas][columnas]);
void printMapaOnTxt(int columnas, int filas, char mapa[filas][columnas], FILE** file);

int vector2ACard(tVector2 vec);
tVector2 cardaVector2(int card);

#endif // MAPARANDOM_H_INCLUDED
