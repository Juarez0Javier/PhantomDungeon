#include "main.h"

int main (int argc, char *argv[]) {

    ConfigData configs;
    GHP_TexturesData tex_data;

    char* nameWindow = "Fantasmas";

    struct GHP_WindowData myWindow;

    if (GHP_SetWindow(&myWindow, nameWindow, react, WIDTH, HEIGHT, &configs, &tex_data)) { // it returns true at the end of the partida
        GHP_DestroyTexturesData(&tex_data);
        GHP_freeBG(&tex_data);
        GHP_DestroyWindow(&myWindow);
    }

    return 0;
}

void react(SDL_Renderer* renderer, void* partidaData, GHP_TexturesData* TexData) {

    ConfigData* configs = (ConfigData*) partidaData;

    int seccion = SECCION_MENU, seccionPrev; // init seccion

    Partida partida;
    iniciarPartida(&partida, configs, TexData, renderer);
    //nullpartida(&partida);

    if (!iniciarPartida(&partida, configs, TexData, renderer)) {
        printf("\nError in partida initalizing");
        seccion=SECCION_SALIR_DIRECTO;
    }

    Seccion secciones[] = { // BE CAREFUL!!! IT HAS SAME ORDER AS CONSTANTS MODE_*
        {initMenu, handlerMenu, NULL},
        {initJuegoCorriendo, handleJuegoCorriendo, renderJuegoCorriendo},
        {initDerrota, handlerDerrota, NULL},
        {initVictoria, handlerVictoria, NULL},
        {initVerConfigs, handlerVerConfigs, NULL}
    };

    if (seccion != SECCION_SALIR_DIRECTO) {
        secciones[seccion].init(renderer, &partida, TexData, configs, &seccion);
        SDL_RenderPresent(renderer);
    }

    SDL_Event event;
    while (seccion != SECCION_SALIR_DIRECTO) {

        unsigned inicioFrame = SDL_GetTicks();

        while (SDL_PollEvent(&event) && seccion != SECCION_SALIR_DIRECTO) {

            if (event.type == SDL_QUIT) {
                seccion = SECCION_SALIR_DIRECTO;
            }

            else {

                seccionPrev = seccion;
                secciones[seccion].handler(renderer, &partida, TexData, &event, &seccion);

                if (seccionPrev != seccion && seccion != SECCION_SALIR_DIRECTO) { // init function only when the seccion changes

                    SDL_Event discard;
                    while (SDL_PollEvent(&discard)) {} // clear the queue of events

                    secciones[seccion].init(renderer, &partida, TexData, configs, &seccion);
                }

                // updatepartidaTime(renderer, &partida, TexData, seccion); if needed should be here to update everytime

            }

        }

        if (secciones[seccion].render && seccion != SECCION_SALIR_DIRECTO) { // because some seccions could have no render
            secciones[seccion].render(renderer, &partida, TexData, &seccion);
            SDL_RenderPresent(renderer);
        }

        unsigned duracionFrame = SDL_GetTicks() - inicioFrame;

        if (duracionFrame < DELAY_DE_FRAMES)
            SDL_Delay(DELAY_DE_FRAMES - duracionFrame);

        // updatepartidaTime(renderer, &partida, TexData, seccion); // It has to be twice to update each frame (with or without event)

    }

    destruirPartida(&partida);

}
