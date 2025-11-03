#include "./api.h"
#include "./protocolo.h"
#include "../../libs/conexion/modelos.h"
#include "../../libs/Comun/conexion.h"
#include <stdio.h>

int apiCrearJugador(SOCKET sock, unsigned* id, const char* nombre, const char* email, const char* contrasenia) {

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
        return ERR_CONEXION;

    leerCampo(&codRes, sizeof(codRes), &actRes);

    if (codRes == OK)
        leerCampo(id, sizeof(*id), &actRes);

    return codRes;
}

int apiBuscarJugador(SOCKET sock, Jugador* jug, unsigned char opcion, unsigned id, const char* nombre) {

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
        return ERR_CONEXION;

    leerCampo(&codRes, sizeof(codRes), &actRes);

    if (codRes == OK)
        leerCampo(jug, sizeof(*jug), &actRes);

    return codRes;
}

int apiCrearPartida(SOCKET sock, unsigned* id, unsigned idJugador, unsigned puntuacion, unsigned cMovs) {

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
        return ERR_CONEXION;

    leerCampo(&codRes, sizeof(codRes), &actRes);

    if (codRes == OK)
        leerCampo(id, sizeof(*id), &actRes);

    return codRes;
}

int apiObtenerRankings(SOCKET sock, unsigned* cantObj, unsigned limite, unsigned saltear, Acc procesarRanking, void* extraAcc) {

    char peticion[TAM_BUFFER], respuesta[TAM_BUFFER];
    char *actPet = peticion;
    unsigned tamPeticion;
    unsigned char codPet = OBTENER_RANKINGS, codRes;
    RankingCompleto rankComp;

    // Carga el codigo de operacion y los parametros.
    cargarCampo(&codPet, sizeof(codPet), &actPet);
    cargarCampo(&limite, sizeof(limite), &actPet);
    cargarCampo(&saltear, sizeof(saltear), &actPet);
    tamPeticion = actPet - peticion;

    // Si no puede enviar la peticion, sale.
    if (!enviarPeticion(sock, tamPeticion, peticion, respuesta))
        return ERR_CONEXION;

    codRes = iniciarSecuencia(sock, respuesta, cantObj);

    // Si no puede iniciar la recepcion de la secuencia, sale.
    if (codRes != INICIO_SECUENCIA)
        return codRes;

    codRes = siguienteSecuencia(sock, &rankComp, sizeof(rankComp));

    while (codRes == ENVIO_SECUENCIA) {
        procesarRanking(&rankComp, extraAcc);
        codRes = siguienteSecuencia(sock, &rankComp, sizeof(rankComp));
    }

    // Si termina con algun error, se indica.
    return codRes == FIN_SECUENCIA ? OK : codRes;
}
