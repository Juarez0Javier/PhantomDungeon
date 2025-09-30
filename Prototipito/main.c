#include "main.h"

int main (int argc, char *argv[]) {

    ConfigData configs;
    GHP_TexturesData tex_data;

    char* nameWindow = "Fantasmas";

    struct GHP_WindowData myWindow;
    
    if (GHP_SetWindow(&myWindow, nameWindow, react, WIDTH, HEIGHT, &configs, &tex_data)) { // it returns true at the end of the game
        GHP_DestroyTexturesData(&tex_data);
        GHP_freeBG(&tex_data);
        GHP_DestroyWindow(&myWindow);
    }

    //getchar(); // avoid closing the cmd

    return 0;
}

void react(SDL_Renderer* renderer, void* gameData, GHP_TexturesData* TexData) {

    ConfigData* configs = (ConfigData*) gameData;

    int mode = MODE_PLAY, modePrev; // init mode


    GameState game;
    nullGame(&game);


    if (!initGame(&game, configs, TexData, renderer)) {
        printf("\nError in game initalizing");
        mode=MODE_END;
    }

    Section SectionPool[] = { // BE CAREFUL!!! IT HAS SAME ORDER AS CONSTANTS MODE_*
        {initMenu, handlerMenu, NULL},
        {initLost, handlerLost, NULL},
        {initWin, handlerWin, NULL}
    };


    if (mode != MODE_END) {
        SectionPool[mode].init(renderer, &game, TexData, configs, &mode);
        SDL_RenderPresent(renderer);
    }

    SDL_Event event;
    while (mode != MODE_END) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                mode = MODE_END;
            }

            else {

                modePrev = mode;
                SectionPool[mode].handler(renderer, &game, TexData, &event, &mode);

                if (modePrev != mode) { // init function only when the mode changes

                    SDL_Event discard;
                    while (SDL_PollEvent(&discard)) {} // clear the queue of events

                    SectionPool[mode].init(renderer, &game, TexData, configs, &mode);
                }

                if (SectionPool[mode].render) { // because some modes could have no render
                    SectionPool[mode].render(renderer, &game, TexData, &mode);
                }

                // updateGameTime(renderer, &game, TexData, mode); if needed should be here to update everytime

                SDL_RenderPresent(renderer);
            }

        }

        game.ticks = SDL_GetTicks();

        // updateGameTime(renderer, &game, TexData, mode); // It has to be twice to update each frame (with or without event)

    }

}
