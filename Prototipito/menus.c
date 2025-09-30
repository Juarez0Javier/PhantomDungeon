#include "menus.h"


void initMenu(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, ConfigData* configData, int* mode) {
    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_START], (WIDTH-289)/2, 30);
    //GHP_renderButton(renderer, &tex->buttons[BUT_SEARCHDIR], (WIDTH-(961-597))/2 , HEIGHT*0.7);
    //GHP_renderButton(renderer, &tex->buttons[BUT_STATS], (WIDTH-(959-595))/2, HEIGHT*0.5);
    //GHP_renderButton(renderer, &tex->buttons[BUT_SETTINGS], (WIDTH-(961-597))/2 , HEIGHT*0.3);

    game->jugador.num = 1;
    game->jugador.tipo = JUGADOR;

    vectorCrear(&game->entidades, sizeof(Entidad));

    if (cargarMapaDeArchivo(&game->mapa, "./mapas/laberinto.txt", &game->jugador, &game->entidades) != OK)
        printf("\nERROR CARGANDO ARCHIVO DE MAPA");
    else {
        crearMapaEntidadesVacio(&game->mapaEntidades,game->mapa.cols, game->mapa.filas);
        llenarMapaConEntidades(&game->mapaEntidades, &game->entidades);
        clearConsole();
        mostrarMapa(&game->mapa, &game->jugador, &game->mapaEntidades);
        //mostrarMapaEntidades(&game->mapaEntidades);
    }


}

void handlerMenu(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, SDL_Event* event, int* mode) {

    if (event->type == SDL_KEYDOWN) {

        if (game->ticks - game->jugador.ultMovTicks > COOLDOWN_MOV_JUGADOR) {

            clearConsole();

            printf("\nPressed %c\n", event->key.keysym.sym);

            Entidad* colisionado = colisionConEntidad(&game->jugador, &game->mapaEntidades, (char)GHP_keyCodeToWASD(event->key.keysym.sym));
            if (colisionado) {
                printf("\nColisionado con: ");
                imprimirEntidad(*colisionado);
                if (colisionado->tipo == 'F') {
                    *mode = MODE_LOST;
                }
            }

            if (moverEntidad(&game->jugador, &game->mapa, (char)GHP_keyCodeToWASD(event->key.keysym.sym)) == MOV_IMPOSIBLE)
                if (colisionConMapa(&game->jugador, &game->mapa, (char)GHP_keyCodeToWASD(event->key.keysym.sym)) == 'S')
                    *mode = MODE_WIN;

            mostrarMapa(&game->mapa, &game->jugador, &game->mapaEntidades);
            game->jugador.ultMovTicks = game->ticks;
        }
        //mostrarMapaEntidades(&game->mapaEntidades);
    }

    //GHP_Button buttons[] = {tex->buttons[BUT_START], tex->buttons[BUT_SEARCHDIR], tex->buttons[BUT_STATS], tex->buttons[BUT_SETTINGS]};
    //handleButtonsClick(buttons, 4, event->button.x, event->button.y, game, mode, event);
}

void initLost(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, ConfigData* configData, int* mode) {

}

void handlerLost(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, SDL_Event* event, int* mode) {
    printf("Perdiste :(. Reiniciando partida...");
    for (int i=0; i<10; i++) {
        SDL_Delay(250);
        printf(".");
    }
    *mode = MODE_PLAY;
}

void initWin(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, ConfigData* configData, int* mode) {
}

void handlerWin(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, SDL_Event* event, int* mode) {
    printf("Ganaste :). Reiniciando partida...");
    for (int i=0; i<10; i++) {
        SDL_Delay(250);
        printf(".");
    }
    *mode = MODE_PLAY;
}




void handleButtonsClick(GHP_Button* buttons, int ammount, int x, int y, GameState* game, int* mode, SDL_Event* event) {
    for (int i=0; i<ammount; i++) {
        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && GHP_clickInButton(x, y, buttons+i)) {
            if ((buttons+i)->on_click)
                (buttons+i)->on_click(game, mode);
        }
    }
}
