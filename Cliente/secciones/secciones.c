#include "secciones.h"

void initMenu(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    ConfigData* configData,
    int* seccion
) {
    partida->jugador.num = 1;
    partida->jugador.tipo = JUGADOR;

    system("cls");

    printf("=======================\n");
    printf("||  Phantom Dungeon  ||\n");
    printf("=======================\n\n");

    printf("Enter: Jugar\n");
    printf("Escape: Salir\n");
    printf("'C': Ver configuracion\n");

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    GHP_renderButton(renderer, &tex->buttons[BUT_JUGAR_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
    GHP_renderButton(renderer, &tex->buttons[BUT_SALIR_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.4);
    GHP_renderButton(renderer, &tex->buttons[BUT_VERCONFIG_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.6);

}

void handlerMenu(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    SDL_Event* event,
    int* seccion,
    unsigned deltaTime
) {
    if (event->type == SDL_KEYDOWN) {
        switch (event -> key.keysym.sym) {
            case SDLK_RETURN:
                *seccion = SECCION_PARTIDA;
                break;
            case SDLK_ESCAPE:
                *seccion = SECCION_SALIR_DIRECTO;
                break;
            case 'c':
                *seccion = SECCION_CONFIGS;
                break;
        }
    }
    GHP_Button botonesActivos[] = {tex->buttons[BUT_JUGAR_GRANDE], tex->buttons[BUT_SALIR_GRANDE], tex->buttons[BUT_VERCONFIG_GRANDE]};
    handleButtonsClick(botonesActivos, 3, event->button.x, event->button.y, partida, seccion, event);
}

void initJuegoCorriendo (
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    ConfigData* configData,
    int* seccion
) {

    printf("Init juego\n");

    partida -> pausado = false;
    partida -> vidasRestantes = CONFIG_VIDAS;
    partida -> premiosObt = 0;
    partida -> puntuacion = 0;

    vaciarLista(&partida -> regMovs);


    if (!generarMapaRandom(configData,RUTA_LABERINTO_PRESET))
        printf("Error genrando el mapa random.\n");

    // Le podriamos pasar el nombre del archivo de laberinto por argumentos a main.
    // Una vez tengamos el algoritmo generador, reemplazarlo ac�.
    if (!cargarMapaDeArchivo(&partida -> mapa, &partida -> jugador, &partida -> fantasmas, RUTA_LABERINTO_PRESET))
        printf("Error cargando el mapa del archivo.\n");

    mostrarMapa(&partida -> mapa);
    GHP_renderMesh(renderer, &(tex->active_mesh), 0);

    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_CHICO], WIDTH*0.21, HEIGHT*0.01);
    GHP_renderButton(renderer, &tex->buttons[BUT_PAUSA_CHICO], WIDTH*0.41, HEIGHT*0.01);
    GHP_renderButton(renderer, &tex->buttons[BUT_SALIR_CHICO], WIDTH*0.61, HEIGHT*0.01);
}

void handleJuegoCorriendo (
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    SDL_Event* event,
    int* seccion,
    unsigned deltaTime
) {

    if (event -> type == SDL_KEYDOWN) {
        char entrada = GHP_keyCodeToWASD(event -> key.keysym.sym);

        // Mientras este pausado, solo acepta los inputs para salir de pausa o volver al menu.
        if (partida -> pausado) {
            switch (entrada) {
                case SDLK_RETURN:
                    partida -> pausado = false;
                    break;

                case SDLK_ESCAPE:
                    *seccion = SECCION_MENU;
                    break;
            }

        } else {

            // Se fija si el jugador hizo un movimiento.
            switch (entrada) {
                case ARRIBA:
                case ABAJO:
                case IZQUIERDA:
                case DERECHA:

                    // Revisa si puede moverse
                    if (
                        partida -> jugador.ticksUltimoMov >=
                        partida -> jugador.ticksEntreMovs
                    ) {
                        calcularMovJugador(
                            &partida -> jugador,
                            &partida -> mapa,
                            entrada,
                            &partida -> movs
                        );
                    }

                    break;

                case SDLK_ESCAPE:
                    partida -> pausado = true;
                    printf("\n");
                    printf("=======================\n");
                    printf("Juego pausado\n");
                    printf("Enter: Volver al juego.\n");
                    printf("Escape: Salir al menu.\n");
                    printf("=======================\n");
                    break;
            }

        }
    }

    if (!partida -> pausado) {

        // Si no puede moverse por cooldown, va aumentando los ticks hasta que eventualmente pueda moverse.
        partida -> jugador.ticksUltimoMov += deltaTime;

        // Luego, calcula el movimiento de todos los fantasmas.
        calcularMovFantasmas(
            &partida -> fantasmas,
            &partida -> mapa,
            &partida -> movs,
            &partida -> jugador,
            deltaTime
        );

        resolverMovimientos(
            partida,
            &partida -> mapa,
            &partida -> movs,
            seccion
        );
    }

    if (*seccion != SECCION_PARTIDA) {
        vaciarCola(&partida -> movs);
        vectorVaciar(&partida -> fantasmas);
    }

    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_CHICO], tex->buttons[BUT_PAUSA_CHICO], tex->buttons[BUT_SALIR_CHICO]};
    handleButtonsClick(botonesActivos, 3, event->button.x, event->button.y, partida, seccion, event);
}

void renderJuegoCorriendo (
    SDL_Renderer*
    renderer,
    Partida* partida,
    GHP_TexturesData*
    tex,
    int* seccion
) {

    if (!partida -> pausado) {
        system("cls");

        printf("=======================\n");
        printf("Vidas: %d\n", partida -> vidasRestantes);
        printf("Premios obtenidos: %d\n", partida -> premiosObt);
        printf("Puntuacion: %d\n", partida -> puntuacion);
        printf("=======================\n");

        printf("\n");

        mostrarMapa(&partida -> mapa);

        actualizarMapaRender(renderer, &partida->mapa, tex, tex->active_mesh);
        GHP_renderButton(renderer, &tex->buttons[BUT_PAUSA_CHICO], WIDTH*0.41, HEIGHT*0.01); // de vuelta porque sino queda como si estuviera pausado
    } else {

        // agrisando el boton de pausa y el tablero
        SDL_Rect rectPausa = {tex->buttons[BUT_PAUSA_CHICO].curWindowX,tex->buttons[BUT_PAUSA_CHICO].curWindowY,tex->buttons[BUT_PAUSA_CHICO].tex->width,tex->buttons[BUT_PAUSA_CHICO].tex->height}; // could be any cell of texs, its only for the dimensions
        SDL_SetRenderDrawColor(renderer, 214, 214, 214, 10);
        SDL_RenderFillRect(renderer, &rectPausa);
        SDL_Rect rectTablero = {tex->active_mesh.offsetX,tex->active_mesh.offsetY, tex->active_mesh.txtr->width*tex->active_mesh.cols, tex->active_mesh.txtr->height*tex->active_mesh.rows};
        SDL_SetRenderDrawColor(renderer, 137, 137, 137, 2);
        SDL_RenderFillRect(renderer, &rectTablero);
    }
}

void initDerrota(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    ConfigData* configData,
    int* seccion
) {
    printf("\n");
    printf("=======================\n");
    printf("Perdiste!\n");
    printf("Enter: Jugar de nuevo.\n");
    printf("Escape: Salir al menu.\n");
    printf("=======================\n\n");

    printf("Movimientos realizados:\n");
    mostrarLista(&partida -> regMovs, mostrarCoordenada);

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
    // Mostrar registro de movmientos.
}

void handlerDerrota(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    SDL_Event* event,
    int* seccion,
    unsigned deltaTime
) {
    switch (event -> key.keysym.sym) {
        case SDLK_ESCAPE:
            *seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            *seccion = SECCION_PARTIDA;
            break;
    }
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};
    handleButtonsClick(botonesActivos, 1, event->button.x, event->button.y, partida, seccion, event);
}

void initVictoria(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    ConfigData* configData,
    int* seccion
) {
    printf("\n");
    printf("=======================\n");
    printf("Ganaste!\n");
    printf("Enter: Jugar de nuevo.\n");
    printf("Escape: Salir al menu.\n");
    printf("=======================\n\n");

    printf("Movimientos realizados:\n");
    mostrarLista(&partida -> regMovs, mostrarCoordenada);

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
    // Mostrar registro de movmientos.
}

void handlerVictoria(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    SDL_Event* event,
    int* seccion,
    unsigned deltaTime
) {
    switch (event -> key.keysym.sym) {
        case SDLK_ESCAPE:
            *seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            *seccion = SECCION_PARTIDA;
            break;
    }
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};
    handleButtonsClick(botonesActivos, 1, event->button.x, event->button.y, partida, seccion, event);
}

void initVerConfigs(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    ConfigData* configData,
    int* seccion
) {
    printf("\nConfiguraciones:\n");
    mostrarConfigs(configData);
    printf("\nPresione enter para volver al menu...");

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
}

void handlerVerConfigs(
    SDL_Renderer* renderer,
    Partida* partida,
    GHP_TexturesData* tex,
    SDL_Event* event,
    int* seccion,
    unsigned deltaTime
) {
    if (event->type == SDL_KEYDOWN) {
        switch (event -> key.keysym.sym) {
            case SDLK_RETURN:
                *seccion = SECCION_MENU;
                break;
            case SDLK_ESCAPE:
                *seccion = SECCION_SALIR_DIRECTO;
                break;
        }
    }
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};
    handleButtonsClick(botonesActivos, 1, event->button.x, event->button.y, partida, seccion, event);
}

void handleButtonsClick(
    GHP_Button* buttons,
    int ammount,
    int x,
    int y,
    Partida* partida,
    int* seccion,
    SDL_Event* event
) {
    for (int i=0; i<ammount; i++) {
        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && GHP_clickInButton(x, y, buttons+i)) {
            if ((buttons+i)->on_click)
                (buttons+i)->on_click(partida, seccion);
        }
    }
}
