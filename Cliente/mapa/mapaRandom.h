#ifndef MAPARANDOM_H_INCLUDED
#define MAPARANDOM_H_INCLUDED

#include "../configs/configs.h"
#include"../../libs/Pila/Dina/Pila_Dinamica.h"

typedef struct
{
    int x;
    int y;
} tPos;

int generarMapaRandom(ConfigData* configData, Partida* partida, char nombreArch[]);
void generarEntradaYPrimerEspacio(tPos* pos, tPos* dirMov, tPos* posE, int filas, int cols, char** mapa);
void chequearPosiblesMovimientos(int cardBloq[4], tPos pos, int filas, int columnas, char** mapa);
bool ningunMovimientoPosible(int cardBloq[4]);
void posicionarSalidaCandidata(tPos* posS, tPos pos, tPos posE, int filas, int columnas);
void taparRadioRespectoAPosEnMascara(tPos pos, int rango, int filas, int cols, char** mapaMascara);
void generarCaminosSimples(tPos pos, tPos dirMov, tPos posE, int filas, int cols, char** mapa);
int insertarEntidadesEnMapa(tPos posE, int fantasmas, int premios, int vidas, int filas, int cols, char** mapa, Partida* partida);
void insertarFantasmasEnMapaEntidades(int filas, int cols, char** mapa, Vector* fantasmas, Entidad*** mapaEntidades);
void cortarParedesParaMasCaminos(int filas, int cols, char** mapa);

tPos dirRandom (int cardBloq[4]);
tPos posCaminoRandom (int columnas, int filas, char** mapa);

char itemRandom (int* fantasmas, int* premios, int* vidas);

void printMapa(int columnas, int filas, char** mapa);
void printMapaOnTxt(int columnas, int filas, char** mapa, FILE** file);

int vector2ACard(tPos vec);
tPos cardaVector2(int card);

#endif // MAPARANDOM_H_INCLUDED
