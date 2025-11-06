#include "./jugador.h"
#include "../../libs/archBin/archBin.h"
#include "../../libs/conexion/modelos.h"
#include "../../libs/conexion/transporte.h"
#include "../peticiones.h"
#include "../../libs/string/string.h"
#include <stdio.h>
#include <string.h>

int _guardarJugador(unsigned* id, const char* nombre, tIndice* jugadores);

unsigned crearJugador(char* actPet, char* respuesta, tIndice* jugadores) {

    unsigned char codRespuesta;
    unsigned id;
    char* actRes = respuesta;

    char nombre[TAM_NOMBRE +1];

    leerCampoStr(nombre, &actPet);

    // Validacion de campos.
    if (!validarCampoStr(nombre, TAM_NOMBRE)) {
        codRespuesta = ERR_FORMATO;
        cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
        return actRes - respuesta;
    }

    trimStr(nombre);

    codRespuesta = _guardarJugador(&id, nombre, jugadores);

    cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
    cargarCampo(&id, sizeof(id), &actRes);

    return actRes - respuesta;
}

// Busca al jugador segun su ID o nombre, depende del parametro opcion.
unsigned buscarJugador(char* actPet, char* respuesta, tIndice* jugadores) {

    FILE* archJugadores;
    JugadorIdx idx;
    Jugador jugador;
    char* actRes = respuesta;
    unsigned char codRespuesta = OK, opcion;

    leerCampo(&opcion, sizeof(opcion), &actPet);

    if (opcion < OPCION_ID || opcion > OPCION_NOMBRE) {
        codRespuesta = ERR_FORMATO;
        cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
        return actRes - respuesta;
    }

    if (opcion == OPCION_ID) {

        leerCampo(&idx.id, sizeof(idx.id), &actPet);

        if (!buscarIndice(jugadores, &idx, sizeof(JugadorIdx), cmpJugadorIdx)) {
            codRespuesta = NO_ENCONTRADO;
            cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
            return actRes - respuesta;
        }

    } else {

        leerCampoStr(idx.nombre, &actPet);
        trimStr(idx.nombre);

        if (!buscarNoClaveIndice(jugadores, &idx, sizeof(JugadorIdx), cmpNombreJugadorIdx)) {
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

    fseek(archJugadores, idx.pos * sizeof(Jugador), SEEK_SET);
    fread(&jugador, sizeof(Jugador), 1, archJugadores);
    fclose(archJugadores);

    cargarCampo(&codRespuesta, sizeof(codRespuesta), &actRes);
    cargarCampo(&jugador, sizeof(Jugador), &actRes);

    return actRes - respuesta;
}

// Crea y guarda a un jugador en la base de datos. Devuelve su ID si tiene exito.
int _guardarJugador(unsigned* id, const char* nombre, tIndice* jugadores) {

    FILE *archJugadores, *archRankings;
    Jugador jugador;
    JugadorIdx idx;
    Ranking ranking;
    int cod = OK;

    // Copia el nombre para buscarlo en el arbol.
    strcpy(idx.nombre, nombre);

    // Si encuentra otro jugador con el mismo nombre, devuelve duplicado.
    // Esto en una situacion normal no va a pasar porque el cliente es quien llama para verificarlo, pero no esta de mas.
    if (buscarNoClaveIndice(jugadores, &idx, sizeof(JugadorIdx), cmpNombreJugadorIdx))
        return DUPLICADO;

    // Procede a la apertura a los archivos.
    archJugadores = fopen(RUTA_JUGADORES_DAT, "r+b");

    if (!archJugadores)
        return ERR_ARCHIVO;

    archRankings = fopen(RUTA_RANKINGS_DAT, "ab");

    if (!archRankings) {
        fclose(archJugadores);
        return ERR_ARCHIVO;
    }

    // Obtencion del ultimo id.
    if (buscarClaveMayorIndice(jugadores, &idx, sizeof(JugadorIdx)))
        strcpy(idx.nombre, nombre); // Copia nuevamente el nombre porque la busqueda trae el nombre del ultimo jugador.
    else
        idx.id = 0;

    idx.id++;

    // Crea al jugador.
    jugador.id = idx.id;
    strcpy(jugador.nombre, nombre);

    // Crea el indice.
    fseek(archJugadores, 0l, SEEK_END);
    idx.pos = ftell(archJugadores) / sizeof(Jugador);

    // Crea y guarda el ranking.
    ranking.idJugador = jugador.id;
    ranking.puntTotal = 0;

    // Crea el indice.
    cod = ponerEnIndice(jugadores, &idx, sizeof(JugadorIdx), cmpJugadorIdx);

    if (cod == OK) {
        // Guarda todo en los archivos correspondientes.
        fwrite(&jugador, sizeof(Jugador), 1, archJugadores);
        fwrite(&ranking, sizeof(Ranking), 1, archRankings);

        *id = jugador.id;
    }

    fclose(archJugadores);
    fclose(archRankings);

    return cod;
}
