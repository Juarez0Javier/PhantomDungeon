#ifndef MAPARANDOM_H_INCLUDED
#define MAPARANDOM_H_INCLUDED

#include "../configs/configs.h"
#include"../../libs/Pila/Dina/Pila_Dinamica.h"

typedef struct
{
    int x;
    int y;
} tVector2;

int generarMapaRandom(ConfigData* configData, Partida* partida, char nombreArch[]);
void generarEntradaYPrimerEspacio(tVector2* pos, tVector2* dirMov, tVector2* posE, int filas, int cols, char** mapa);
void chequearPosiblesMovimientos(int cardBloq[4], tVector2 pos, int filas, int columnas, char** mapa);
bool ningunMovimientoPosible(int cardBloq[4]);
void posicionarSalidaCandidata(tVector2* posS, tVector2 pos, tVector2 posE, int filas, int columnas);
void taparRadioRespectoAPosEnMascara(tVector2 pos, int rango, int filas, int cols, char** mapaMascara);
void generarCaminosSimples(tVector2 pos, tVector2 dirMov, tVector2 posE, int filas, int cols, char** mapa);
int insertarEntidades(tVector2 posE, int fantasmas, int premios, int vidas, int filas, int cols, char** mapa, Partida* partida);
void cortarParedesParaMasCaminos(int filas, int cols, char** mapa);

tVector2 dirRandom (int cardBloq[4]);
tVector2 posCaminoRandom (int columnas, int filas, char** mapa);

char itemRandom (int* fantasmas, int* premios, int* vidas);

void printMapa(int columnas, int filas, char** mapa);
void printMapaOnTxt(int columnas, int filas, char** mapa, FILE** file);

int vector2ACard(tVector2 vec);
tVector2 cardaVector2(int card);

#endif // MAPARANDOM_H_INCLUDED
