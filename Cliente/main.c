#include "./conexion/conexion.h"
#include "./conexion/api.h"
#include "../libs/conexion/modelos.h"
#include "../libs/comun/conexion.h"
#include "main.h"
#include <stdlib.h>
#include <time.h>
#include "./global.h"

int main (int argc, char *argv[]) {

    tContextoGlobal cGlobal;
    GHP_TexturesData texturas;
    struct GHP_WindowData ventana;
    char* nameWindow = "PhantomDungeon";

    srand(time(NULL));

    cGlobal.socket = abrirConexion();

    // Podria usarse otra flag para verificar.
    if (cGlobal.socket == INVALID_SOCKET) {
        printf("Error al conectarse al servidor\n");
        cGlobal.idJugador = 0; // Se le carga un ID invalido.
    }

    // Retorna true al final de la partida.
    if (GHP_SetWindow(&ventana, nameWindow, react, WIDTH, HEIGHT, &cGlobal, &texturas)) {
        GHP_DestroyTexturesData(&texturas);
        GHP_freeBG(&texturas);
        GHP_DestroyWindow(&ventana);
    }

    if (cGlobal.socket != INVALID_SOCKET)
        cerrarConexion(cGlobal.socket);

    return 0;
}

void react(SDL_Renderer* renderer, void* partidaData, GHP_TexturesData* TexData) {

    // Configuración, partida y eventos.
    tContextoGlobal* cGlobal = (tContextoGlobal*) partidaData;

    ConfigData* configs = &(cGlobal -> configData);
    Partida* partida = &(cGlobal -> partida);
    int* seccion = (&cGlobal -> seccion);
    SDL_Event event;

    int seccionPrev;

    // Los elementos estan en el orden que se encuentran las opciones de seccion en el archivo de constantes.
    Seccion secciones[] = {
        {initMenu, handlerMenu, NULL},
        {initJuegoCorriendo, handleJuegoCorriendo, renderJuegoCorriendo},
        {initDerrota, handlerDerrota, NULL},
        {initVictoria, handlerVictoria, NULL},
        {initVerConfigs, handlerVerConfigs, NULL}
    };

    // Relacionadas al control de frames.
    unsigned inicioFrame, duracionFrame, ticksUltFrame;

    // Inicio de seccion.
    *seccion = SECCION_MENU;
    seccionPrev = *seccion;

    // Aplica todas las configuraciones previas al inicio de partida.
    if (iniciarPartida(partida, configs, TexData, renderer)) {
        secciones[*seccion].init(cGlobal, renderer, TexData);
        SDL_RenderPresent(renderer);
    } else {
        printf("\nError en iniciar partida");
        *seccion = SECCION_SALIR_DIRECTO;
    }

    ticksUltFrame = SDL_GetTicks();

    while (*seccion != SECCION_SALIR_DIRECTO) {

        // Maneja los frames y calcula el deltaTime para que el juego se ejecute a la misma velocidad en cualquier equipo.
        inicioFrame = SDL_GetTicks();
        cGlobal -> deltaTime = inicioFrame - ticksUltFrame; // Calcula cuantos ticks pasaron desde el frame anterior.
        ticksUltFrame = inicioFrame;

        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
            *seccion = SECCION_SALIR_DIRECTO;

        else {
            secciones[*seccion].handler(cGlobal, renderer, TexData, &event);

            if (*seccion != SECCION_SALIR_DIRECTO && seccionPrev != *seccion) {
                SDL_Event discard;

                seccionPrev = *seccion;

                // Limpia la cola de eventos de SDL al cambiar de sección.
                while (SDL_PollEvent(&discard));

                // Al cambiar de sección, se ejecuta el init de la nueva sección.
                secciones[*seccion].init(cGlobal, renderer, TexData);
                SDL_RenderPresent(renderer); // algunos init renderizan cosas
            }
        }

        // Se comprueba esto ya que no todas las secciones renderizan algo.
        if (*seccion != SECCION_SALIR_DIRECTO && secciones[*seccion].render) {
            secciones[*seccion].render(cGlobal, renderer, TexData);
            SDL_RenderPresent(renderer);
        }

        // Controla y limita los frames máximos a los que trabaja el juego.
        // Sin esto, cada frame duraría solamente lo que le toma procesar cada instrucción al equipo.
        duracionFrame = SDL_GetTicks() - inicioFrame;

        if (duracionFrame < DELAY_DE_FRAMES)
            SDL_Delay(DELAY_DE_FRAMES - duracionFrame);
    }

    destruirPartida(partida);
}
