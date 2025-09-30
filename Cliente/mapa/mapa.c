#include <stddef.h>
#include <stdio.h>

#include "./mapa.h"
#include "../partida/partida.h"
#include "../auxiliares/matriz/matriz.h"
#include "../constantes.h"
#include "../../Comun/comun.h"
#include "../../Vector/Vector.h"

bool crearMapa (Mapa* mapa, int filas, int cols) {
    mapa -> terreno = (char**) crearMatriz(filas, cols, sizeof(char));

    if (!mapa -> terreno)
        return false;

    mapa -> entidades = (Entidad***) crearMatriz(filas, cols, sizeof(Entidad*));

    if (!mapa -> entidades)
        return false;

    mapa -> filas = filas;
    mapa -> cols = cols;

    return true;
}

bool cargarMapaDeArchivo (Mapa* mapa, Entidad* jugador, Vector* fantasmas, const char* nomArch) {

    FILE* archTxt = fopen(nomArch, "rt");
    Entidad fantasma, *pf;
    VectorIterador it;
    int i = 0, j;
    char casilla;

    if (!archTxt)
        return false;

    while ((casilla = fgetc(archTxt)) != EOF) {

        j = 0;

        while (casilla != '\n') {

            mapa -> entidades[i][j] = NULL;

            switch (casilla) {
                case ENTRADA:
                    jugador -> x = j;
                    jugador -> y = i;
                    jugador -> xInicial = j;
                    jugador -> yInicial = i;
                    jugador -> tipo = JUGADOR;
                    jugador -> eliminado = false;

                    mapa -> entidades[i][j] = jugador;
                    break;

                case FANTASMA:
                    fantasma.x = j;
                    fantasma.y = i;
                    fantasma.xInicial = j;
                    fantasma.yInicial = i;
                    fantasma.tipo = FANTASMA;
                    fantasma.eliminado = false;

                    if (vectorInsertarAlFinal(fantasmas, &fantasma) != OK)
                        printf("Error al insertar fantasma, memoria insuficiente");

                    casilla = CAMINO;
                    break;
            }

            mapa -> terreno[i][j] = casilla;
            casilla = fgetc(archTxt);
            j++;
        }

        i++;
    }

    fclose(archTxt);

    // Carga al mapa de entidades los fantasmas.
    vectorIteradorCrear(&it, fantasmas);

    pf = (Entidad*) vectorIteradorPrimero(&it);

    while (pf) {
        mapa -> entidades[pf -> y][pf -> x] = pf;
        pf = (Entidad*) vectorIteradorSiguiente(&it);
    }

    return true;
}

void mostrarMapa (Mapa* mapa) {
    int i, j;

    for (i = 0; i < mapa -> filas; i++) {
        for (j = 0; j < mapa -> cols; j++) {

            if (mapa -> entidades[i][j])
                printf("%c ", (mapa -> entidades[i][j]) -> tipo);
            else
                printf("%c ", mapa -> terreno[i][j]);
        }
        printf("\n");
    }
}