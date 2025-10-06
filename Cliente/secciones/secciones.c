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

    // Le podriamos pasar el nombre del archivo de laberinto por argumentos a main.
    // Una vez tengamos el algoritmo generador, reemplazarlo ac�.
    if (!cargarMapaDeArchivo(&partida -> mapa, &partida -> jugador, &partida -> fantasmas, RUTA_LABERINTO_PRESET))
        printf("Error cargando el mapa del archivo.\n");

    mostrarMapa(&partida -> mapa);
    GHP_renderMesh(renderer, &(tex->active_mesh), 0);
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
