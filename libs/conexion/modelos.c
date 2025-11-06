#include "./modelos.h"
#include <string.h>
#include <stdio.h>

int cmpJugRankings (const void* r1, const void* r2) {
    Ranking* rank1 = (Ranking*) r1;
    Ranking* rank2 = (Ranking*) r2;

    return rank1 -> idJugador - rank2 -> idJugador;
}

int cmpPuntRankings (const void* r1, const void* r2) {
    Ranking* rank1 = (Ranking*) r1;
    Ranking* rank2 = (Ranking*) r2;

    // Decreciente
    return rank2 -> puntTotal - rank1 -> puntTotal;
}

int cmpPuntRankingsComp (const void* r1, const void* r2) {
    RankingCompleto* rank1 = (RankingCompleto*) r1;
    RankingCompleto* rank2 = (RankingCompleto*) r2;

    // Decreciente
    return rank2 -> puntTotal - rank1 -> puntTotal;
}

void acumularPuntuacion (void* elem, void* punt) {
    Ranking* ranking = (Ranking*) elem;
    unsigned* p = (unsigned*) punt;
    ranking -> puntTotal += *p;
}

void imprimirRanking(const void* elem) {
    Ranking* ranking = (Ranking*) elem;
    printf("%d\t%d\n", ranking -> idJugador, ranking -> puntTotal);
}

void imprimirRankingCompleto(const void* elem) {
    RankingCompleto* ranking = (RankingCompleto*) elem;
    printf("%d\t%s\t%d\n", ranking -> idJugador, ranking -> nombre, ranking -> puntTotal);
}

void imprimirJugador(const void* elem) {
    Jugador* jugador = (Jugador*) elem;
    printf("%d\t%s\n", jugador -> id, jugador -> nombre);
}

void imprimirJugadorIdx(void* elem, void* extra) {
    JugadorIdx* jugador = (JugadorIdx*) elem;
    printf("%d\t%s\t%d\n", jugador -> id, jugador -> nombre, jugador -> pos);
}

bool jugadorAIdx(void* idx, const void* reg, int pos) {
    JugadorIdx* jIdx = (JugadorIdx*) idx;
    Jugador* jug = (Jugador*) reg;

    jIdx -> id = jug -> id;
    strcpy(jIdx -> nombre, jug -> nombre);
    jIdx -> pos = pos;

    return true;
}

int cmpJugadorIdx(const void* e1, const void* e2) {
    JugadorIdx* j1 = (JugadorIdx*) e1;
    JugadorIdx* j2 = (JugadorIdx*) e2;

    return j1 -> id - j2 -> id;
}

int cmpNombreJugadorIdx(const void* e1, const void* e2) {
    JugadorIdx* j1 = (JugadorIdx*) e1;
    JugadorIdx* j2 = (JugadorIdx*) e2;

    return strcmpi(j1 -> nombre, j2 -> nombre);
}

void imprimirPartida(const void* elem) {
    RegistroPartida* partida = (RegistroPartida*) elem;

    printf(
        "%d\t%d\t%d\t%d\n",
        partida -> id, partida -> idJugador, partida -> puntuacion, partida -> cMovs
    );
}
