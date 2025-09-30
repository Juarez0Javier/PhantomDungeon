#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "./constantes.h"
#include "../Comun/comun.h"
#include "./secciones/secciones.h"
#include "./partida/partida.h"
#include "./config/config.h"

int main (int argc, char* argv[]) {

    Partida partida;
    int seccionAct = SECCION_MENU;
    int seccionAnt;
    // unsigned ultimoFrame;
    bool corriendo = true;

    Seccion secciones[] = {
        {initMenu, handleMenu, renderMenu},
        {initJuegoCorriendo, handleJuegoCorriendo, renderJuegoCorriendo},
        {initDerrota, handleDerrota, renderDerrota},
        {initVictoria, handleVictoria, renderVictoria},
    };

    // Carga de la ventana y configs de SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != false) {
        printf("Error SDL: %s\n", SDL_GetError());
        return ERR_VIDEO;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "Ejemplo SDL Estado",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    if (!ventana) {
        printf("Error ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return ERR_VIDEO;
    }

    // Mientras que el mapa se mantenga fijo, podemos tener esto acá.
    if (!crearConfigPartida(&partida)) {
        printf("Error creando la configuracion de partida");
        return ERR_CONFIG;
    }

    secciones[seccionAct].init(&partida);
    seccionAnt = seccionAct;
    // ultimoFrame = SDL_GetTicks();

    while (corriendo) {
        SDL_Event e;
        // float deltaTime;
        unsigned inicioFrame = SDL_GetTicks();
        unsigned duracionFrame;

        // Calculo de deltaTime (en ms)
        // deltaTime = inicioFrame - ultimoFrame;

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                corriendo = false;

            secciones[seccionAct].handle(&e, &partida, &seccionAct);

            // Maneja los cambios entre secciones
            if (seccionAct != seccionAnt) {
                seccionAnt = seccionAct;
                secciones[seccionAct].init(&partida);
            }
        }

        secciones[seccionAct].render(&partida);
        duracionFrame = SDL_GetTicks() - inicioFrame;

        if (duracionFrame < DELAY_DE_FRAMES)
            SDL_Delay(DELAY_DE_FRAMES - duracionFrame);

    }

    destruirConfigPartida(&partida);

    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}
