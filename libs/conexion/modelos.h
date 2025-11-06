#ifndef MODELOS_H_INCLUDED
#define MODELOS_H_INCLUDED

#include "../Comun/comun.h"

#define TAM_NOMBRE 25

// Se le pueden aniadir mas campos, pero en nuestro caso, lo dejamos sencillo.
typedef struct {
    unsigned id;
    char nombre[TAM_NOMBRE +1];
} Jugador;

typedef struct {
    unsigned id;
    char nombre[TAM_NOMBRE +1];
    unsigned pos;
} JugadorIdx;

typedef struct {
    unsigned id, idJugador, puntuacion, cMovs;
} RegistroPartida;

typedef struct {
    unsigned idJugador, puntTotal;
} Ranking;

typedef struct {
    unsigned idJugador, puntTotal;
    char nombre[TAM_NOMBRE +1];
} RankingCompleto;

int cmpJugRankings (const void* r1, const void* r2);
int cmpPuntRankings (const void* r1, const void* r2);
int cmpPuntRankingsComp (const void* r1, const void* r2);
void acumularPuntuacion (void* elem, void* punt);
void imprimirRanking(const void* elem);
void imprimirRankingCompleto(const void* elem);
void imprimirJugador(const void* elem);
void imprimirJugadorIdx(void* elem, void* extra);
bool jugadorAIdx(void* idx, const void* reg, int pos);
int cmpJugadorIdx(const void* e1, const void* e2);
int cmpNombreJugadorIdx(const void* e1, const void* e2);
void imprimirPartida(const void* elem);

#endif // MODELOS_H_INCLUDED
