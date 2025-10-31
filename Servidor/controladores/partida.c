#include "./partida.h"
#include "./comun.h"
#include "../../libs/archBin/archBin.h"
#include "../../libs/conexion/modelos.h"
#include "../../libs/conexion/transporte.h"

int _guardarPartida(unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs);
void _actualizarRankings(FILE* archRankings, unsigned idJugador, unsigned puntuacion);

unsigned crearPartida(char* actPet, char* respuesta) {

    unsigned char codRespuesta;
    unsigned id, idJugador, puntuacion, cMovs;
    char* actRes = respuesta;

    leerCampo(&idJugador, sizeof(idJugador), &actPet);
    leerCampo(&puntuacion, sizeof(puntuacion), &actPet);
    leerCampo(&cMovs, sizeof(cMovs), &actPet);

    codRespuesta = _guardarPartida(&id, idJugador, puntuacion, cMovs);

    cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
    cargarCampo(&id, sizeof(id), &actRes);

    return actRes - respuesta;
}

int _guardarPartida(unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs) {
    FILE *archPartidas, *archRankings;
    RegistroPartida partida;
    unsigned ultimoId = 0;

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

    Ranking ranking = { idJugador, 0 };
    int pos;

    // En caso de que por algun error no se hubiese creado el ranking junto con el jugador.
    pos = buscarEnArchivoBin(archRankings, &ranking, sizeof(ranking), cmpJugRankings);

    ranking.puntTotal += puntuacion;

    // Si existe, lo actualiza. Si no, lo aniade.
    if (pos != -1)
        fseek(archRankings, pos * sizeof(ranking), SEEK_SET);
    else
        fseek(archRankings, 0, SEEK_END);

    fwrite(&ranking, sizeof(ranking), 1, archRankings);
}
