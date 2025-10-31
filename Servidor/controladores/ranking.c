#include "./ranking.h"
#include "../../libs/conexion/modelos.h"
#include "../../libs/conexion/transporte.h"
#include <stdio.h>

int _prepararRankings(unsigned* cantObt, tLista* rankings, tArbol* jugadores, unsigned limite, unsigned saltear);

unsigned obtenerRankings(char* actPet, char* respuesta, tArbol* jugadores, tSecuencia* secRank) {

    unsigned char codRespuesta;
    unsigned limite, saltear, cantObt;
    char* actRes = respuesta;

    // Obtiene los parámetros de limite y salteo.
    leerCampo(&limite, sizeof(limite), &actPet);
    leerCampo(&saltear, sizeof(saltear), &actPet);

    codRespuesta = _prepararRankings(&cantObt, &(secRank -> elementos), jugadores, limite, saltear);

    if (codRespuesta != OK) {
        cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
        return actRes - respuesta;
    }

    // Asigna el tamanio correspondiente para leer y enviar rankings completos.
    secRank -> tamElem = sizeof(RankingCompleto);

    // Marca el inicio de la secuencia y copia la cantidad de registros obtenidos.
    codRespuesta = INICIO_SECUENCIA;
    cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
    cargarCampo(&cantObt, sizeof(cantObt), &actRes);

    return actRes - respuesta;
}

// Ordena por puntaje los rankings y los junta con el nombre de su jugador. Incluye paginado.
int _prepararRankings(unsigned* cantObt, tLista* rankings, tArbol* jugadores, unsigned limite, unsigned saltear) {

    FILE* archRankings = fopen(RUTA_RANKINGS_DAT, "rb");
    JugadorIdx jugIdx;
    Ranking rank;
    RankingCompleto rankComp;
    unsigned obt = 0;
    int cod = OK;

    if (!archRankings)
        return ERR_ARCHIVO;

    // Si el salteo es invalido, se descubre con el feof.
    fseek(archRankings, saltear * sizeof(rank), SEEK_SET);
    fread(&rank, sizeof(rank), 1, archRankings);

    // Si se manda un limite de 0, se obtienen todos los que haya luego del salteo.
    while (cod == OK && (limite == 0 || obt < limite) && !feof(archRankings)) {

        // Junta el ranking con el nombre de su jugador.
        jugIdx.id = rank.idJugador;

        // Si lo encuentra, copia al vector con los rankings completos.
        if (buscarPorClaveArbol(jugadores, &jugIdx, sizeof(JugadorIdx), cmpJugadorIdx)) {
            rankComp.idJugador = jugIdx.id;
            strcpy(rankComp.nombre, jugIdx.nombre);
            rankComp.puntTotal = rank.puntTotal;

            cod = ponerEnListaEnOrden(rankings, &rankComp, sizeof(rankComp), cmpPuntRankingsComp, true, NULL, NULL);
            obt++;
            fread(&rank, sizeof(rank), 1, archRankings);
        } else
            cod = NO_ENCONTRADO;
    }

    /*
     * Nota referida a lo que se explica en el archivo de conexion:
       Si los elementos de la secuencia fueran una cola, habria que, luego hacer todo esta carga ordenada en la lista
       descargar la misma para cargar una cola.
    */

    if (cod == OK)
        *cantObt = obt;
    else
        vaciarLista(rankings);

    mostrarLista(rankings, imprimirRankingCompleto);

    fclose(archRankings);
    return cod;
}
