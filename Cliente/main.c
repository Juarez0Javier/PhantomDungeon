#include "main.h"
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[]) {

    ConfigData configs;
    GHP_TexturesData tex_data;
    struct GHP_WindowData myWindow;

    char* nameWindow = "Fantasmas";
    
    srand(time(NULL));

    // Retorna true al final de la partida.
    if (GHP_SetWindow(&myWindow, nameWindow, react, WIDTH, HEIGHT, &configs, &tex_data)) {
        GHP_DestroyTexturesData(&tex_data);
        GHP_freeBG(&tex_data);
        GHP_DestroyWindow(&myWindow);
    }

    return 0;
}

void react(SDL_Renderer* renderer, void* partidaData, GHP_TexturesData* TexData) {

    // Configuración, partida y eventos.
    ConfigData* configs = (ConfigData*) partidaData;
    Partida partida;
    SDL_Event event;

    // Secciones.
    int seccion = SECCION_MENU; // init seccion
    int seccionPrev = seccion;

    Seccion secciones[] = { // BE CAREFUL!!! IT HAS SAME ORDER AS CONSTANTS MODE_*
        {initMenu, handlerMenu, NULL},
        {initJuegoCorriendo, handleJuegoCorriendo, renderJuegoCorriendo},
        {initDerrota, handlerDerrota, NULL},
        {initVictoria, handlerVictoria, NULL},
        {initVerConfigs, handlerVerConfigs, NULL}
    };
    
    // Relacionadas al control de frames.
    unsigned inicioFrame, duracionFrame, ticksUltFrame, deltaTime;

    // Aplica todas las configuraciones previas al inicio de partida.
    if (iniciarPartida(&partida, configs, TexData, renderer)) {
        secciones[seccion].init(renderer, &partida, TexData, configs, &seccion);
        SDL_RenderPresent(renderer);
    } else {
        printf("\nError en iniciar partida");
        seccion = SECCION_SALIR_DIRECTO;
    }

    ticksUltFrame = SDL_GetTicks();

    while (seccion != SECCION_SALIR_DIRECTO) {

        // Maneja los frames y calcula el deltaTime para que el juego se ejecute a la misma velocidad en cualquier equipo.
        inicioFrame = SDL_GetTicks();
        deltaTime = inicioFrame - ticksUltFrame; // Calcula cuantos ticks pasaron desde el frame anterior.
        ticksUltFrame = inicioFrame;

        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
            seccion = SECCION_SALIR_DIRECTO;

        else {
            secciones[seccion].handler(renderer, &partida, TexData, &event, &seccion, deltaTime);

            if (seccion != SECCION_SALIR_DIRECTO && seccionPrev != seccion) {
                SDL_Event discard;
    
                seccionPrev = seccion;

                // Limpia la cola de eventos de SDL al cambiar de sección.
                while (SDL_PollEvent(&discard));

                // Al cambiar de sección, se ejecuta el init de la nueva sección.
                secciones[seccion].init(renderer, &partida, TexData, configs, &seccion);
            }
        }

        // Se comprueba esto ya que no todas las secciones renderizan algo.
        if (seccion != SECCION_SALIR_DIRECTO && secciones[seccion].render) {
            secciones[seccion].render(renderer, &partida, TexData, &seccion);
            SDL_RenderPresent(renderer);
        }

        // Controla y limita los frames máximos a los que trabaja el juego.
        // Sin esto, cada frame duraría solamente lo que le toma procesar cada instrucción al equipo.
        duracionFrame = SDL_GetTicks() - inicioFrame;

        if (duracionFrame < DELAY_DE_FRAMES)
            SDL_Delay(DELAY_DE_FRAMES - duracionFrame);
    }

    destruirPartida(&partida);
}