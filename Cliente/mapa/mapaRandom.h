#ifndef MAPARANDOM_H_INCLUDED
#define MAPARANDOM_H_INCLUDED

#include "../configs/configs.h"
#include"../../libs/Pila/Dina/Pila_Dinamica.h"
#include"../../libs/Cola/Cola.h"

typedef struct
{
    int x;
    int y;
} tPos;

/*2) Tema Laberinto (Javi)
Si ven el codigo lo modularice e hice algunas mejoras.
Sin embargo todavía falta terminar de pulirlo.
Voy a agregar que ni bien entres al laberinto entres en una "mini-galeria"
(dim 3x3, 2x3, 3x2 o 2x2 después veré que va mejor),
como para aumentar las probabiliades de que el jugador pueda
irse por mas caminos y "amagar" a los fantasmas. Sin embargo con
eso no es suficiente, porque aumenta la probabilidad de que sea
jugable pero no la asegura. Querido Javi, vos que te das mas mania
respecto a este tema laberinto, queria pedirte si tambien podes agregarle
una sub-funcion que le agrege mas bifurcaciones al camino ganador.
Antes de que me pegues, se que el algoritmo actual se las agrega,
pero muchas veces se las agrega a una distancia considerable de la entrada
(cuando el jugador llega a la bifurcacion los fantasmas ya llegaron,
 lo que imposibilita ganar la partida). Para poder identificar el camino
 ganador si queres podes reutilizar parte del codigo de Valen (busqueda BFS o por anchura,
garantiza encontrar el camino ganador mas corto),
ella hizo una parte del codigo que encuentra el camino y despues una parte que lo reconstruye,
reutiliza eso y cuando se "reconstruye" anda abriendole paredes a los costados hacia otros caminos
(es fijarte si del otro lado (x+-2 o j+-2 hay camino y entonces destruir la pared (x+-1 y+-1).
Recorda hacer esto en una sub-funcion y no en el codigo principal de una para mejorar la legibilidad.*/


int generarMapaRandom(ConfigData* configData, Partida* partida, char nombreArch[]);

void generarEntradaYPrimerEspacio(tPos* pos, tPos* dirMov, tPos* posE, int filas, int cols, char** mapa);

void generarCaminosSimples(tPos pos, tPos dirMov, tPos posE, tPos* posS, int filas, int cols, char** mapa);

void chequearPosiblesMovimientos(int cardBloq[4], tPos pos, int filas, int columnas, char** mapa);
bool ningunMovimientoPosible(int cardBloq[4]);

tPos dirRandom (int cardBloq[4]);

void posicionarSalidaCandidata(tPos* posS, tPos pos, tPos posE, int filas, int columnas);

void taparRadioRespectoAPosEnMascara(tPos pos, int rango, int filas, int cols, char** mapaMascara);

void generarGaleria (int galLong, int galAlt, tPos posE, int filas, int cols, char** mapa);

void bifurcacionesCaminoPrincial (int filas, int cols, tPos posE, tPos posS, char** mapa);

void cortarParedesParaMasCaminos(int filas, int cols, char** mapa);

int insertarEntidadesEnMapa(tPos posE, int fantasmas, int premios, int vidas, int filas, int cols, char** mapa, Partida* partida);

tPos posCaminoRandom (int columnas, int filas, char** mapa);
char itemRandom (int* fantasmas, int* premios, int* vidas);

void insertarFantasmasEnMapaEntidades(int filas, int cols, char** mapa, Vector* fantasmas, Entidad*** mapaEntidades);

void printMapa(int columnas, int filas, char** mapa);
void printMapaEnTxt(int columnas, int filas, char** mapa, FILE** file);

tPos cardaVector2(int card);

#endif // MAPARANDOM_H_INCLUDED
