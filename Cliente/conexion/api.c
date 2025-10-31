#include "./api.h"
#include "./protocolo.h"
#include "../../libs/conexion/modelos.h"
#include "../../libs/Comun/conexion.h"
#include <stdio.h>

bool apiCrearJugador(SOCKET sock, unsigned* id, const char* nombre, const char* email, const char* contrasenia) {

    char peticion[TAM_BUFFER], respuesta[TAM_BUFFER];
    char *actPet = peticion, *actRes = respuesta;
    unsigned tamPeticion;
    unsigned char codPet = CREAR_JUGADOR, codRes;

    // Carga el codigo de operacion y los parametros.
    cargarCampo(&codPet, sizeof(codPet), &actPet);
    cargarCampoStr(nombre, &actPet);
    cargarCampoStr(email, &actPet);
    cargarCampoStr(contrasenia, &actPet);
    tamPeticion = actPet - peticion;

    if (!enviarPeticion(sock, tamPeticion, peticion, respuesta))
        return false;

    leerCampo(&codRes, sizeof(codRes), &actRes);

    if (codRes != OK)
        return false;

    leerCampo(id, sizeof(*id), &actRes);

    return true;
}

bool apiBuscarJugador(SOCKET sock, Jugador* jug, unsigned char opcion, unsigned id, const char* nombre) {

    char peticion[TAM_BUFFER], respuesta[TAM_BUFFER];
    char *actPet = peticion, *actRes = respuesta;
    unsigned tamPeticion;
    unsigned char codPet = BUSCAR_JUGADOR, codRes;

    // Carga el codigo de operacion y los parametros.
    cargarCampo(&codPet, sizeof(codPet), &actPet);
    cargarCampo(&opcion, sizeof(opcion), &actPet);

    if (opcion == OPCION_ID)
        cargarCampo(&id, sizeof(id), &actPet);
    else
        cargarCampoStr(nombre, &actPet);

    tamPeticion = actPet - peticion;

    if (!enviarPeticion(sock, tamPeticion, peticion, respuesta))
        return false;

    leerCampo(&codRes, sizeof(codRes), &actRes);

    if (codRes != OK)
        return false;

    leerCampo(jug, sizeof(*jug), &actRes);

    return true;
}

bool apiCrearPartida(SOCKET sock, unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs) {

    char peticion[TAM_BUFFER], respuesta[TAM_BUFFER];
    char *actPet = peticion, *actRes = respuesta;
    unsigned tamPeticion;
    unsigned char codPet = CREAR_PARTIDA, codRes;

    // Carga el codigo de operacion y los parametros.
    cargarCampo(&codPet, sizeof(codPet), &actPet);
    cargarCampo(&idJugador, sizeof(idJugador), &actPet);
    cargarCampo(&puntuacion, sizeof(puntuacion), &actPet);
    cargarCampo(&cMovs, sizeof(cMovs), &actPet);
    tamPeticion = actPet - peticion;

    if (!enviarPeticion(sock, tamPeticion, peticion, respuesta))
        return false;

    leerCampo(&codRes, sizeof(codRes), &actRes);

    if (codRes != OK)
        return false;

    leerCampo(id, sizeof(*id), &actRes);

    return true;
}

bool apiObtenerRankings(SOCKET sock, unsigned limite, unsigned saltear, Acc procesarRanking) {

    char peticion[TAM_BUFFER], respuesta[TAM_BUFFER];
    char *actPet = peticion;
    unsigned cantElem, tamPeticion;
    unsigned char codPet = OBTENER_RANKINGS;
    RankingCompleto rankComp;

    // Carga el codigo de operacion y los parametros.
    cargarCampo(&codPet, sizeof(codPet), &actPet);
    cargarCampo(&limite, sizeof(limite), &actPet);
    cargarCampo(&saltear, sizeof(saltear), &actPet);
    tamPeticion = actPet - peticion;

    printf("Antes de enviar peticion\n");

    // Si no puede enviar la peticion, sale.
    if (!enviarPeticion(sock, tamPeticion, peticion, respuesta))
        return false;

    printf("Despues de enviar peticion\n");

    // Si no puede iniciar la recepcion de la secuencia, sale.
    if (!iniciarSecuencia(sock, respuesta, &cantElem))
        return false;

    // Si no hay registros, sale.
    if (cantElem == 0) {
        return true;
    }

    while (siguienteSecuencia(sock, &rankComp, sizeof(rankComp)))
        procesarRanking(&rankComp, NULL);

    return true;
}
