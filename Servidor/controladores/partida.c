#include "./partida.h"
#include "./comun.h"
#include "../../libs/archBin/archBin.h"
#include "../../libs/conexion/modelos.h"
#include "../../libs/conexion/transporte.h"

int _guardarPartida(unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs, tIndice* jugadores);
void _actualizarRankings(FILE* archRankings, unsigned idJugador, unsigned puntuacion);

unsigned crearPartida(char* actPet, char* respuesta, tIndice* jugadores) {

    unsigned char codRespuesta;
    unsigned id, idJugador, puntuacion, cMovs;
    char* actRes = respuesta;

    leerCampo(&idJugador, sizeof(idJugador), &actPet);
    leerCampo(&puntuacion, sizeof(puntuacion), &actPet);
    leerCampo(&cMovs, sizeof(cMovs), &actPet);

    codRespuesta = _guardarPartida(&id, idJugador, puntuacion, cMovs, jugadores);

    cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);

    if (codRespuesta == OK)
        cargarCampo(&id, sizeof(id), &actRes);

    return actRes - respuesta;
}

int _guardarPartida(unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs, tIndice* jugadores) {
    FILE *archPartidas, *archRankings;
    RegistroPartida partida;
    JugadorIdx idx;
    unsigned ultimoId = 0;

    idx.id = idJugador;

    // Si no encuentra al jugador del ID, devuelve no encontrado. Mantiene una especie de integridad referencial.
    // Esto en una situacion normal no va a pasar porque desde el cliente se usa al id del jugador obtenido.
    if (!buscarIndice(jugadores, &idx, sizeof(JugadorIdx), cmpJugadorIdx))
        return NO_ENCONTRADO;

    archPartidas = fopen(RUTA_PARTIDAS_DAT, "r+b");

    if (!archPartidas)
        return ERR_ARCHIVO;

    archRankings = fopen(RUTA_RANKINGS_DAT, "r+b");

    if (!archRankings) {
        fclose(archPartidas);
        return ERR_ARCHIVO;
    }

    // Busca el último id.
    fseek(archPartidas, (long) -sizeof(RegistroPartida), SEEK_END);

    if (fread(&partida, sizeof(RegistroPartida), 1, archPartidas) > 0)
        ultimoId = partida.id;

    ultimoId++;

    // Crea y guarda la partida.
    partida.id = ultimoId;
    partida.idJugador = idJugador;
    partida.puntuacion = puntuacion;
    partida.cMovs = cMovs;

    fseek(archPartidas, 0, SEEK_END);
    fwrite(&partida, sizeof(RegistroPartida), 1, archPartidas);

    // Solo actualiza si la puntuacion realmente cambio.
    if (puntuacion > 0)
        _actualizarRankings(archRankings, idJugador, puntuacion);

    fclose(archPartidas);
    fclose(archRankings);

    *id = ultimoId;

    return OK;
}

// Actualiza el ranking, acomodando las posiciones de los jugadores.
void _actualizarRankings(FILE* archRankings, unsigned idJugador, unsigned puntuacion) {

    tLista rankings;
    Ranking ranking;

    crearLista(&rankings);

    fread(&ranking, sizeof(ranking), 1, archRankings);

    /*
     * No hay duplicados por ID en el archivo de rankings, se generan junto a los usuarios.
       En caso de que hubiese alguno, se unifica y se acumulan las puntuaciones.
    */
    while (!feof(archRankings)) {
        ponerEnListaEnOrden(
            &rankings,
            &ranking,
            sizeof(ranking),
            cmpJugRankings,
            false,
            acumularPuntuacion,
            &ranking.puntTotal
        );
        fread(&ranking, sizeof(ranking), 1, archRankings);
    }

    ranking.idJugador = idJugador;
    ranking.puntTotal = puntuacion;

    // Acumula la puntuacion al usuario. Si por alguna razon no existiese el ranking del usuario, se crea.
    ponerEnListaEnOrden(
        &rankings,
        &ranking,
        sizeof(ranking),
        cmpJugRankings,
        false,
        acumularPuntuacion,
        &ranking.puntTotal
    );

    // Deja la lista ordenada por puntuacion (decreciente) nuevamente.
    ordenarLista(&rankings, cmpPuntRankings, SELECCION);

    rewind(archRankings);

    while (sacarDeListaAlInicio(&rankings, &ranking, sizeof(ranking)))
        fwrite(&ranking, sizeof(ranking), 1, archRankings);
}
