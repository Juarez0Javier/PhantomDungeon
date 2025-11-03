#include "./jugador.h"
#include "../../libs/archBin/archBin.h"
#include "../../libs/conexion/modelos.h"
#include "../../libs/conexion/transporte.h"
#include "../peticiones.h"
#include "../../libs/string/string.h"
#include <stdio.h>
#include <string.h>

int _guardarJugador(unsigned* id, const char* nombre, tArbol* jugadores);

unsigned crearJugador(char* actPet, char* respuesta, tArbol* jugadores) {

    unsigned char codRespuesta;
    unsigned id;
    char* actRes = respuesta;

    char nombre[TAM_NOMBRE +1];

    leerCampoStr(nombre, &actPet);

    // Validacion de campos.
    if (!validarCampoStr(nombre, TAM_NOMBRE))
        return ERR_FORMATO;

    trimStr(nombre);

    codRespuesta = _guardarJugador(&id, nombre, jugadores);

    cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
    cargarCampo(&id, sizeof(id), &actRes);

    return actRes - respuesta;
}

// Busca al jugador segun su ID o nombre, depende del parametro opcion.
unsigned buscarJugador(char* actPet, char* respuesta, tArbol* jugadores) {

    FILE* archJugadores;
    JugadorIdx jugIdx;
    Jugador jugador;
    char* actRes = respuesta;
    unsigned char codRespuesta = OK, opcion;

    leerCampo(&opcion, sizeof(opcion), &actPet);

    if (opcion < OPCION_ID || opcion > OPCION_NOMBRE) {
        codRespuesta = ERR_FORMATO;
        *actRes = codRespuesta;
        actRes++;
        return actRes - respuesta;
    }

    if (opcion == OPCION_ID) {

        leerCampo(&jugIdx.id, sizeof(jugIdx.id), &actPet);

        if (!buscarPorClaveArbol(jugadores, &jugIdx, sizeof(JugadorIdx), cmpJugadorIdx)) {
            codRespuesta = NO_ENCONTRADO;
            cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
            return actRes - respuesta;
        }

    } else {

        leerCampoStr(jugIdx.nombre, &actPet);
        trimStr(jugIdx.nombre);

        if (!buscarPorNoClaveArbol(jugadores, &jugIdx, sizeof(JugadorIdx), cmpNombreJugadorIdx)) {
            codRespuesta = NO_ENCONTRADO;
            cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
            return actRes - respuesta;
        }
    }

    archJugadores = fopen(RUTA_JUGADORES_DAT, "rb");

    if (!archJugadores) {
        codRespuesta = ERR_ARCHIVO;
        cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
        return actRes - respuesta;
    }

    fseek(archJugadores, jugIdx.pos * sizeof(Jugador), SEEK_SET);
    fread(&jugador, sizeof(Jugador), 1, archJugadores);
    fclose(archJugadores);

    cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
    cargarCampo(&jugador, sizeof(Jugador), &actRes);

    return actRes - respuesta;
}

// Crea y guarda a un jugador en la base de datos. Devuelve su ID si tiene exito.
int _guardarJugador(unsigned* id, const char* nombre, tArbol* jugadores) {

    FILE *archJugadores, *archJugadoresIdx, *archRankings;
    Jugador jugador;
    JugadorIdx idx;
    Ranking ranking;
    int cod = OK;

    // Copia el nombre para buscarlo en el arbol.
    strcpy(idx.nombre, nombre);

    // Si encuentra otro jugador con el mismo nombre, devuelve duplicado.
    // Esto en una situacion normal no va a pasar porque el cliente es quien llama para verificarlo, pero no esta de mas.
    if (buscarPorNoClaveArbol(jugadores, &idx, sizeof(JugadorIdx), cmpNombreJugadorIdx))
        return DUPLICADO;

    // Procede a la apertura a los archivos.
    archJugadores = fopen(RUTA_JUGADORES_DAT, "ab");

    if (!archJugadores)
        return ERR_ARCHIVO;

    archJugadoresIdx = fopen(RUTA_JUGADORES_IDX_DAT, "r+b");

    if (!archJugadoresIdx) {
        fclose(archJugadores);
        return ERR_ARCHIVO;
    }

    archRankings = fopen(RUTA_RANKINGS_DAT, "ab");

    if (!archJugadoresIdx) {
        fclose(archJugadores);
        fclose(archJugadoresIdx);
        return ERR_ARCHIVO;
    }

    // Obtencion del ultimo id.
    if (!buscarClaveMayorArbol(jugadores, &jugador, sizeof(Jugador)))
        jugador.id = 0;

    jugador.id++;

    // Crea al jugador.
    strcpy(jugador.nombre, nombre);

    // Crea el indice.
    fseek(archJugadoresIdx, 0, SEEK_END);
    idx.id = jugador.id;
    idx.pos = ftell(archJugadoresIdx) / sizeof(JugadorIdx);

    // Crea y guarda el ranking.
    ranking.idJugador = jugador.id;
    ranking.puntTotal = 0;

    cod = ponerEnArbolI(jugadores, &idx, sizeof(JugadorIdx), cmpJugadorIdx, NULL, NULL);

    if (cod == OK) {
        // Guarda todo en los archivos correspondientes.
        fwrite(&jugador, sizeof(Jugador), 1, archJugadores);
        fwrite(&idx, sizeof(JugadorIdx), 1, archJugadoresIdx);
        fwrite(&ranking, sizeof(Ranking), 1, archRankings);

        *id = jugador.id;
    }

    fclose(archJugadores);
    fclose(archJugadoresIdx);
    fclose(archRankings);

    return cod;
}
