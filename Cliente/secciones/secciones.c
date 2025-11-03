#include "secciones.h"
#include "../conexion/api.h"
#include "../../libs/string/string.h"
#include <ctype.h>

void _limpiarTextosGenerales(int ini, int fin, SDL_Renderer* renderer, GHP_TexturesData* tex);
bool _procesarNombre(tContextoGlobal* cGlobal);
void _cargarTexsConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void _mostrarRankCompletoSDL(void* elem, void* extra);
void _obtenerYMostrarRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void _mostrarMovListaSDL(void* elemLista, void* infoExtra);
void _procesarFinPartida(Partida* partida, SOCKET socket, unsigned idJugador);
void _mostrarFinPartida(const char* msj, Partida* partida, SDL_Renderer* renderer, GHP_TexturesData* tex);

void initMenu (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    system("cls");

    printf("=======================\n");
    printf("||  Phantom Dungeon  ||\n");
    printf("=======================\n\n");

    printf("Enter: Jugar\n");
    printf("Escape: Salir\n");
    printf("'R': Ver rankings\n");
    printf("'C': Ver configuracion\n");

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    GHP_renderButton(renderer, &tex->buttons[BUT_JUGAR_GRANDE_A_NOMBRE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
    GHP_renderButton(renderer, &tex->buttons[BUT_SALIR_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.4);
    GHP_renderButton(renderer, &tex->buttons[BUT_VERCONFIG_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.6);
    GHP_renderButton(renderer, &tex->buttons[BUT_VERRANKING_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.8);
}

void handlerMenu (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {

    GHP_Button botonesActivos[] = {
        tex->buttons[BUT_JUGAR_GRANDE_A_NOMBRE],
        tex->buttons[BUT_SALIR_GRANDE],
        tex->buttons[BUT_VERCONFIG_GRANDE],
        tex->buttons[BUT_VERRANKING_GRANDE],
    };

    if (event->type == SDL_KEYDOWN) {
        switch (event -> key.keysym.sym) {
            case SDLK_RETURN:
                cGlobal -> seccion = SECCION_INGRESO_NOMBRE;
                break;
            case SDLK_ESCAPE:
                cGlobal -> seccion = SECCION_SALIR_DIRECTO;
                break;
            case 'c':
                cGlobal -> seccion = SECCION_CONFIGS;
                break;
            case 'r':
                cGlobal -> seccion = SECCION_RANKINGS;
                break;
        }
    }

    handleButtonsClick(botonesActivos, 4, &(cGlobal -> partida), &(cGlobal -> seccion), event);
}

void initIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    system("cls");

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    if (cGlobal -> socket == INVALID_SOCKET) {
        strcpy(tex->texts[TEXT_ENTRADANOMBREMENSAJE].text, "Inicio de sesion y guardado de partidas no disponible.");
        GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREMENSAJE, TAM_FUENTE_CHICO, RED_COLOR);
        return;
    }

    strcpy(tex->texts[TEXT_ENTRADANOMBREMENSAJE].text, "Nombre de usuario:");
    GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREMENSAJE, TAM_FUENTE_CHICO, BLACK_COLOR);

    // Limpia el texto del input y del error antes de empezar.
    tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[0] = '\0';
    GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, 0, BLACK_COLOR);
    tex->texts[TEXT_ERR_NOMBRE_JUGADOR].text[0] = '\0';
    GHP_updateTextTexture(renderer, tex, TEXT_ERR_NOMBRE_JUGADOR, 0, RED_COLOR);

    GHP_renderButton(renderer, &tex->buttons[BUT_JUGAR_GRANDE], (WIDTH - tex->buttons[BUT_JUGAR_GRANDE].tex->width)/2, HEIGHT*0.7);
}

void handlerIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button buttons[] = {tex->buttons[BUT_JUGAR_GRANDE]};
    unsigned tamNombre;

    // Si no hay conexion, se informa y se la opcion de jugar.
    if (cGlobal -> socket == INVALID_SOCKET) {

        handleButtonsClick(buttons, 1, &cGlobal->partida, &cGlobal->seccion, event);

        if (event->type == SDL_KEYDOWN) {

            switch(event->key.keysym.sym) {

                case SDLK_ESCAPE:
                    cGlobal -> seccion = SECCION_MENU;
                    break;

                case SDLK_RETURN:
                    cGlobal -> seccion = SECCION_PARTIDA;
                    break;
            }
        }

        return;
    }

    tamNombre = strlen(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text);

    if (validarStrNoVacio(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text, tamNombre))
        handleButtonsClick(buttons, 1, &cGlobal->partida, &cGlobal->seccion, event);

    if (event->type == SDL_TEXTINPUT) {

        // Si no se excede el limite de tamanio del texto, lo escribe en el input.
        if (tamNombre <= TAM_NOMBRE) {
            strcat(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text, event->text.text);
            GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, TAM_FUENTE_CHICO, BLACK_COLOR);
            tex->texts[TEXT_ERR_NOMBRE_JUGADOR].text[0] = '\0';
            GHP_updateTextTexture(renderer, tex, TEXT_ERR_NOMBRE_JUGADOR, TAM_FUENTE_CHICO, RED_COLOR);
        }

    } else if (event->type == SDL_KEYDOWN) {

        switch(event->key.keysym.sym) {
            case SDLK_BACKSPACE:

                // Va borrando caracter a caracter.
                if (tamNombre > 0) {
                    tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[tamNombre-1] = '\0';
                    GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, TAM_FUENTE_CHICO, BLACK_COLOR);
                }
                break;

            case SDLK_ESCAPE:
                cGlobal -> seccion = SECCION_MENU;
                tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[0] = '\0';
                GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, TAM_FUENTE_CHICO, BLACK_COLOR);
                break;

            case SDLK_RETURN:
                cGlobal -> seccion = SECCION_PARTIDA;
                break;
        }
    }

    if (cGlobal -> seccion == SECCION_PARTIDA) {

        // Si se ingresa un nombre vacio, se informa el error y se queda en la seccion de ingreso de nombre.
        if (!validarStrNoVacio(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text, tamNombre)) {
            strcpy(tex->texts[TEXT_ERR_NOMBRE_JUGADOR].text, "Se debe ingresar un nombre.");
            GHP_updateTextTexture(renderer, tex, TEXT_ERR_NOMBRE_JUGADOR, TAM_FUENTE_CHICO, RED_COLOR);
            cGlobal -> seccion = SECCION_INGRESO_NOMBRE;
            return;
        }

        strncpy(
            cGlobal -> nombreJugador,
            tex->texts[TEXT_ENTRADANOMBREJUGADOR].text,
            TAM_NOMBRE
        );

        cGlobal -> nombreJugador[TAM_NOMBRE] = '\0';
        trimStr(cGlobal -> nombreJugador);

        // Si falla en encontrar o crear el nombre, se queda en la seccion de ingreso de nombre e informa el error.
        if (!_procesarNombre(cGlobal)) {
            cGlobal -> socket = INVALID_SOCKET;
            strcpy(tex->texts[TEXT_ENTRADANOMBREMENSAJE].text, "Inicio de sesion y guardado de partidas no disponible.");
            GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREMENSAJE, TAM_FUENTE_CHICO, RED_COLOR);
            cGlobal -> seccion = SECCION_INGRESO_NOMBRE;
            return;
        }
    }

    // Limpia el texto del input y del error al terminar.
    if (cGlobal -> seccion != SECCION_INGRESO_NOMBRE) {
        tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[0] = '\0';
        GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, 0, BLACK_COLOR);
        tex->texts[TEXT_ERR_NOMBRE_JUGADOR].text[0] = '\0';
        GHP_updateTextTexture(renderer, tex, TEXT_ERR_NOMBRE_JUGADOR, 0, RED_COLOR);
    }
}

void renderIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    unsigned tamNombre = strlen(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text);

    if (cGlobal -> socket != INVALID_SOCKET) {
        GHP_renderTexture(renderer, tex->texts[TEXT_ENTRADANOMBREJUGADOR].tex, 52, HEIGHT * 0.4);
        dibujarRectanguloParaEntrada(renderer);
    }

    // Solo si se ingresa un nombre valido, o no hay conexion, muestra el boton para jugar.
    if (
        validarStrNoVacio(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text, tamNombre) ||
        cGlobal -> socket == INVALID_SOCKET
    ) {
        GHP_renderButton(
            renderer,
            &tex->buttons[BUT_JUGAR_GRANDE],
            (WIDTH-tex->buttons[BUT_JUGAR_GRANDE].tex->width)/2,
            HEIGHT*0.7
        );
    }

    GHP_renderTexture(renderer, tex->texts[TEXT_ENTRADANOMBREMENSAJE].tex, 52, HEIGHT * 0.3);
    GHP_renderTexture(renderer, tex->texts[TEXT_ERR_NOMBRE_JUGADOR].tex, 52, HEIGHT * 0.55);
}

void initJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {

    Partida* partida = &(cGlobal -> partida);

    printf("Init juego\n");

    partida -> pausado = false;
    partida -> vidasRestantes = CONFIG_VIDAS;
    partida -> premiosObt = 0;
    partida -> puntuacion = 0;

    if (generarMapaRandom(&(cGlobal -> configData), &cGlobal->partida, RUTA_LABERINTO_PRESET) != OK)
        printf("Error genrando el mapa random.\n");

    mostrarMapa(&partida -> mapa);
    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderMesh(renderer, &(tex->active_mesh), 0);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_CHICO], WIDTH*0.21, HEIGHT*0.01);
    GHP_renderButton(renderer, &tex->buttons[BUT_PAUSA_CHICO], WIDTH*0.41, HEIGHT*0.01);
    GHP_renderButton(renderer, &tex->buttons[BUT_SALIR_CHICO], WIDTH*0.61, HEIGHT*0.01);
}

void handleJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {

    Partida* partida = &(cGlobal -> partida);
    GHP_Button botonesActivos[] = {
        tex->buttons[BUT_MENU_CHICO],
        tex->buttons[BUT_PAUSA_CHICO],
    };

    if (event -> type == SDL_KEYDOWN) {
        char entrada = GHP_keyCodeToWASD(event -> key.keysym.sym);

        // Mientras este pausado, solo acepta los inputs para salir de pausa o volver al menu.
        if (partida -> pausado) {
            switch (entrada) {
                case SDLK_RETURN:
                    partida -> pausado = false;
                    break;

                case SDLK_ESCAPE:
                    cGlobal -> seccion = SECCION_MENU;
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
        partida -> jugador.ticksUltimoMov += cGlobal -> deltaTime;

        // Luego, calcula el movimiento de todos los fantasmas.
        calcularMovFantasmas(
            &partida -> fantasmas,
            &partida -> mapa,
            &partida -> movs,
            &partida -> jugador,
            cGlobal -> deltaTime
        );

        resolverMovimientos(
            partida,
            &partida -> mapa,
            &partida -> movs,
            &(cGlobal -> seccion)
        );
    }

    handleButtonsClick(botonesActivos, 2, &(cGlobal -> partida), &(cGlobal -> seccion), event);

    if (cGlobal -> seccion != SECCION_PARTIDA) {
        vaciarCola(&partida -> movs);
        vectorVaciar(&partida -> fantasmas);
    }
}

void renderJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {

    Partida* partida = &(cGlobal -> partida);

    // could be any cell of texs, its only for the dimensions
    SDL_Rect rectPausa = {
        tex->buttons[BUT_PAUSA_CHICO].curWindowX,
        tex->buttons[BUT_PAUSA_CHICO].curWindowY,
        tex->buttons[BUT_PAUSA_CHICO].tex->width,
        tex->buttons[BUT_PAUSA_CHICO].tex->height
    };

    SDL_Rect rectTablero = {
        tex->active_mesh.offsetX,
        tex->active_mesh.offsetY,
        tex->active_mesh.txtr->width*tex->active_mesh.cols,
        tex->active_mesh.txtr->height*tex->active_mesh.rows
    };

    if (!partida -> pausado) {
        system("cls");
        GHP_renderBG(renderer,tex,WIDTH,HEIGHT);

        printf("=======================\n");
        printf("Vidas: %d\n", partida -> vidasRestantes);
        printf("Premios obtenidos: %d\n", partida -> premiosObt);
        printf("Puntuacion: %d\n", partida -> puntuacion);
        printf("=======================\n");

        printf("\n");

        mostrarMapa(&partida -> mapa);
        actualizarMapaRender(renderer, &partida->mapa, tex, tex->active_mesh);

        GHP_renderButton(renderer, &tex->buttons[BUT_PAUSA_CHICO], WIDTH*0.41, HEIGHT*0.01);
        GHP_renderButton(renderer, &tex->buttons[BUT_MENU_CHICO], WIDTH*0.21, HEIGHT*0.01);

        sprintf(tex->texts[TEXT_VIDAS].text, "Vidas: %d", cGlobal->partida.vidasRestantes);
        sprintf(tex->texts[TEXT_PUNTUACION].text, "Puntuacion: %d", cGlobal->partida.puntuacion);
        GHP_renderText(renderer, tex, TEXT_VIDAS, TAM_FUENTE_CHICO, BLACK_COLOR, 5, 5);
        GHP_renderText(renderer, tex, TEXT_PUNTUACION, TAM_FUENTE_CHICO, BLACK_COLOR, 610, 5);

    } else {
        // agrisando el boton de pausa y el tablero
        SDL_SetRenderDrawColor(renderer, 214, 214, 214, 10);
        SDL_RenderFillRect(renderer, &rectPausa);
        SDL_SetRenderDrawColor(renderer, 137, 137, 137, 2);
        SDL_RenderFillRect(renderer, &rectTablero);
    }
}

void initDerrota (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {

    printf("\n");
    printf("=======================\n");
    printf("Perdiste!\n");
    printf("Enter: Jugar de nuevo.\n");
    printf("Escape: Salir al menu.\n");
    printf("=======================\n\n");

    _procesarFinPartida(&(cGlobal -> partida), cGlobal -> socket, cGlobal -> idJugador);
    _mostrarFinPartida("Fin del juego.", &(cGlobal -> partida), renderer, tex);
}

void handlerDerrota(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE], tex->buttons[BUT_VERMOVIMIENTOS_GRANDE]};

    switch (event -> key.keysym.sym) {
        case SDLK_ESCAPE:
            cGlobal -> seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            cGlobal -> seccion = SECCION_PARTIDA;
            break;
    }

    handleButtonsClick(botonesActivos, 2, &(cGlobal -> partida), &(cGlobal -> seccion), event);

    if (cGlobal->seccion != SECCION_DERROTA && cGlobal->seccion != SECCION_VERMOVS)
        vaciarLista(&cGlobal->partida.regMovs);
}

void initVictoria (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {

    printf("\n");
    printf("=======================\n");
    printf("Ganaste!\n");
    printf("Enter: Jugar de nuevo.\n");
    printf("Escape: Salir al menu.\n");
    printf("=======================\n\n");

    _procesarFinPartida(&(cGlobal -> partida), cGlobal -> socket, cGlobal -> idJugador);
    _mostrarFinPartida("Nivel superado!", &(cGlobal -> partida), renderer, tex);
}

void handlerVictoria(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE], tex->buttons[BUT_VERMOVIMIENTOS_GRANDE]};

    switch (event -> key.keysym.sym) {
        case SDLK_ESCAPE:
            cGlobal -> seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            cGlobal -> seccion = SECCION_PARTIDA;
            break;
    }

    handleButtonsClick(botonesActivos, 2, &(cGlobal -> partida), &(cGlobal -> seccion), event);

    if (cGlobal->seccion != SECCION_VICTORIA && cGlobal->seccion != SECCION_VERMOVS)
        vaciarLista(&cGlobal->partida.regMovs);
}

void initVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {

    cGlobal -> cantRankings = 0;
    cGlobal -> salteoRankings = 0;

    _obtenerYMostrarRankings(cGlobal, renderer, tex);
}

void handlerVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {

    if (event->type == SDL_KEYDOWN) {

        if (event -> key.keysym.sym == SDLK_RETURN) {

            // Si ya no hay rankings para saltear, termina el salteo.
            if (cGlobal -> salteoRankings > 0)
                _obtenerYMostrarRankings(cGlobal, renderer, tex);
            else
                cGlobal -> seccion = SECCION_MENU;
        }

        if (event -> key.keysym.sym == SDLK_ESCAPE)
            cGlobal -> seccion = SECCION_MENU;

        // Cuando cambia de seccion, limpia los textos.
        if (cGlobal -> seccion != SECCION_RANKINGS) {
            _limpiarTextosGenerales(INICIO_TEXTOS_RANKINGS, LIMITE_RANKINGS, renderer, tex);
            tex->texts[TEXT_PRESIONEENTER].text[0] = '\0';
            GHP_updateTextTexture(renderer, tex, TEXT_PRESIONEENTER, 0, BLACK_COLOR);
        }
    }
}

void renderVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    for (int i = INICIO_TEXTOS_RANKINGS; i < LIMITE_RANKINGS; i++)
        GHP_renderTexture(renderer, tex->texts[i].tex, 52, HEIGHT * i * 0.1 + 52);

    GHP_renderTexture(renderer, tex->texts[TEXT_PRESIONEENTER].tex, 52, HEIGHT - 75);
}

void initVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    printf("\nConfiguraciones:\n");
    mostrarConfigs(&(cGlobal -> configData));
    printf("\nPresione enter para volver al menu...");

    _cargarTexsConfigs(cGlobal, renderer, tex);

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.8);

    for (int i = 0; i < CANT_CONFIGS; i++)
        GHP_renderTexture(renderer, tex->texts[i].tex, 52, HEIGHT * (i - INICIO_TEXTOS_CONFIGS) * 0.1 + 20);
}

void handlerVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};

    if (event->type == SDL_KEYDOWN) {
        switch (event -> key.keysym.sym) {
            case SDLK_RETURN:
                cGlobal -> seccion = SECCION_MENU;
                break;
            case SDLK_ESCAPE:
                cGlobal -> seccion = SECCION_MENU;
                break;
        }
    }

    handleButtonsClick(botonesActivos, 1, &(cGlobal -> partida), &(cGlobal -> seccion), event);

    // Cuando sale de la seccion, limpia los textos.
    if (cGlobal -> seccion != SECCION_CONFIGS) {
        for (int i = 0; i < CANT_CONFIGS; i++) {
            tex->texts[i].text[0] = '\0';
            GHP_updateTextTexture(renderer, tex, i, TAM_FUENTE_GRANDE, BLACK_COLOR);
        }
    }
}

void initVerMovs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    ctxImpReg lineasImpresas = {0,0, tex, renderer};

    for (int i= 0; i < CANT_LINEAS_MOVS; i++)
        tex->texts[i-INICIO_TEXTOS_MOVS].text[0] = '\0';

    recorrerListaConInfoExtra(&cGlobal->partida.regMovs, _mostrarMovListaSDL, &lineasImpresas);

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    for (int i=0; i<CANT_LINEAS_MOVS; i++)
        GHP_renderTexture(renderer, tex->texts[i].tex, 52, HEIGHT * 0.05 * (i - INICIO_TEXTOS_CONFIGS) + 10);

    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.7);
}

void handlerVerMovs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};

    switch (event -> key.keysym.sym) {
        case SDLK_ESCAPE:
            cGlobal -> seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            cGlobal -> seccion = SECCION_MENU;
            break;
    }

    handleButtonsClick(botonesActivos, 1, &(cGlobal -> partida), &(cGlobal -> seccion), event);

    // Cuando sale de la seccion, limpia los textos y vacia la lista.
    if (cGlobal->seccion != SECCION_VERMOVS) {
        _limpiarTextosGenerales(INICIO_TEXTOS_MOVS, CANT_LINEAS_MOVS, renderer, tex);
        vaciarLista(&cGlobal->partida.regMovs);
    }

}

void handleButtonsClick(GHP_Button* botones, int cantidad, Partida* partida, int* seccion, SDL_Event* event) {
    int i;

    for (i = 0; i < cantidad; i++) {
        if (
            event->type == SDL_MOUSEBUTTONDOWN &&
            event->button.button == SDL_BUTTON_LEFT &&
            GHP_clickInButton(event->button.x, event->button.y, botones+i)
        ) {
            if ((botones+i)->on_click)
                (botones+i)->on_click(partida, seccion);
        }
    }
}

void dibujarRectanguloParaEntrada(SDL_Renderer* renderer) {
    SDL_Rect rect = {50, HEIGHT*0.4, WIDTH-50*2, 50};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}



void _limpiarTextosGenerales(int ini, int fin, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    int i;

    for (i = ini; i < fin; i++) {
        tex->texts[i].text[0] = '\0';
        GHP_updateTextTexture(renderer, tex, i, 0, BLACK_COLOR);
    }
}

void _procesarFinPartida(Partida* partida, SOCKET socket, unsigned idJugador) {

    unsigned idPartida, cMovs = 0;
    int cod;

    printf("Movimientos realizados:\n");

    mostrarLista(&partida -> regMovs, mostrarCoordenada);
    reducirLista(&partida -> regMovs, contarMovs, &cMovs);

    // Una vez finaliza la partida, si puede, la guarda en el servidor.
    if (socket != INVALID_SOCKET) {

        printf("ID jugador: %d\n", idJugador);

        cod = apiCrearPartida(socket, &idPartida, idJugador, partida -> puntuacion, cMovs);

        if (cod != OK)
            printf("\nError al crear la partida.\n");
        else
            printf("\nID de partida: %d.\n", idPartida);

    } else
        printf("\nNo se puede enviar al server.\n");
}

void _mostrarFinPartida(const char* msj, Partida* partida, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    strcpy(tex->texts[TEXT_CABECERA_O_TITULO].text, msj);
    GHP_updateTextTexture(renderer, tex, TEXT_CABECERA_O_TITULO, TAM_FUENTE_GRANDE, BLACK_COLOR);

    sprintf(tex->texts[TEXT_PUNTUACION].text, "Puntuacion: %d", partida -> puntuacion);
    sprintf(tex->texts[TEXT_PREMIOS].text, "Premios: %d", partida -> premiosObt);
    sprintf(tex->texts[TEXT_VIDAS].text, "Vidas: %d", partida -> vidasRestantes);

    GHP_renderTexture(renderer, tex->texts[TEXT_CABECERA_O_TITULO].tex, WIDTH*0.35, HEIGHT*0.2);

    GHP_renderText(renderer, tex, TEXT_PUNTUACION, TAM_FUENTE_CHICO, BLACK_COLOR, WIDTH*0.35, HEIGHT*0.3);
    GHP_renderText(renderer, tex, TEXT_PREMIOS, TAM_FUENTE_CHICO, BLACK_COLOR, WIDTH*0.35, HEIGHT*0.35);
    GHP_renderText(renderer, tex, TEXT_VIDAS, TAM_FUENTE_CHICO, BLACK_COLOR, WIDTH*0.35, HEIGHT*0.4);

    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.55);
    GHP_renderButton(renderer, &tex->buttons[BUT_VERMOVIMIENTOS_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.75);

    // Limpia los textos usados por la partida.
    _limpiarTextosGenerales(INICIO_TEXTOS_PARTIDAS, CANT_TEXTOS_PARTIDAS, renderer, tex);
}

bool _procesarNombre(tContextoGlobal* cGlobal) {

    int cod;
    Jugador jug;

    cod = apiBuscarJugador(cGlobal -> socket, &jug, OPCION_NOMBRE, 0, cGlobal -> nombreJugador);

    if (cod == NO_ENCONTRADO) {

        cod = apiCrearJugador(
            cGlobal -> socket,
            &(cGlobal -> idJugador),
            cGlobal -> nombreJugador
        );
    } else
        cGlobal -> idJugador = jug.id;

    return cod == OK;
}

void _cargarTexsConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    char configsNombres[][TAM_MAX_NOMBRE_CONFIG] = {
        "filas",
        "columnas",
        "vidas_inicio",
        "maximo_numero_fantasmas",
        "maximo_numero_premios",
        "maximo_vidas_extra"
    };

    float* configsValores[] = {
        &cGlobal->configData.filas,
        &cGlobal->configData.columnas,
        &cGlobal->configData.vidas_inicio,
        &cGlobal->configData.maximo_numero_fantasmas,
        &cGlobal->configData.maximo_numero_premios,
        &cGlobal->configData.maximo_vidas_extra
    };

    for (int i = INICIO_TEXTOS_CONFIGS; i < CANT_CONFIGS; i++) {
        sprintf(tex->texts[i].text, "%s: %.0f", configsNombres[i], *(configsValores[i]));
        GHP_updateTextTexture(renderer, tex, i, TAM_FUENTE_GRANDE, BLACK_COLOR);
    }
}

void _mostrarRankCompletoSDL(void* elem, void* extra) {
    RankingCompleto* ranking = (RankingCompleto*) elem;
    tCTex* ctx = (tCTex*) extra;

    int nroReg = ctx->cGlobal->cantRankings + INICIO_TEXTOS_RANKINGS;

    snprintf(
        ctx->tex->texts[nroReg].text,
        TAM_NOMBRE + 2*TAM_MAX_UINT_A_STR +4 +1, // El +4 es por los espacios.
        "%u  %s  %u",
        ranking->idJugador,
        ranking->nombre,
        ranking->puntTotal
    );

    ctx->cGlobal->cantRankings++;

    printf("%d\t%s\t%d\n", ranking->idJugador, ranking->nombre, ranking->puntTotal);
}

void _obtenerYMostrarRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {

    int cod;
    unsigned cantObt;
    tCTex ctx = { cGlobal, tex };

    // Si no pudo conectarse o se corto la conexion, informa el error.
    if (cGlobal -> socket == INVALID_SOCKET) {
        strcpy(tex->texts[INICIO_TEXTOS_RANKINGS].text, "Rankings no disponibles.");
        GHP_updateTextTexture(renderer, tex, INICIO_TEXTOS_RANKINGS, TAM_FUENTE_GRANDE, RED_COLOR);
        strcpy(tex->texts[TEXT_PRESIONEENTER].text, "[Presione enter para volver al menu]");
        GHP_updateTextTexture(renderer, tex, TEXT_PRESIONEENTER, TAM_FUENTE_GRANDE, BLACK_COLOR);
        printf("\nRankings no disponibles.\n");
        return;
    }

    system("cls");
    printf("Rankings:\n");

    // Antes de obtener y mostrar resultados, limpia los textos.
    _limpiarTextosGenerales(INICIO_TEXTOS_RANKINGS, LIMITE_RANKINGS, renderer, tex);

    cod = apiObtenerRankings(
        cGlobal -> socket,
        &cantObt,
        LIMITE_RANKINGS,
        cGlobal -> salteoRankings,
        _mostrarRankCompletoSDL,
        &ctx
    );

    if (cod == OK) {
        if (cantObt == 0) {
            cGlobal -> salteoRankings = 0;
            strcpy(tex->texts[TEXT_PRESIONEENTER].text, "[Presione enter para volver al menu]");
            GHP_updateTextTexture(renderer, tex, TEXT_PRESIONEENTER, TAM_FUENTE_GRANDE, BLACK_COLOR);
            printf("\nFin. Enter: Volver al menu");
        } else {
            cGlobal -> salteoRankings += cantObt;
            strcpy(tex->texts[TEXT_PRESIONEENTER].text, "[Presione enter para ver siguiente pagina]");
            GHP_updateTextTexture(renderer, tex, TEXT_PRESIONEENTER, TAM_FUENTE_GRANDE, BLACK_COLOR);
            printf("\nEnter: Proxima pagina");
        }

        // Actualiza los rankings obtenidos en cada textura.
        for (int i = INICIO_TEXTOS_RANKINGS; i < LIMITE_RANKINGS; i++)
            GHP_updateTextTexture(renderer, tex, i, TAM_FUENTE_GRANDE, BLACK_COLOR);

        cGlobal->cantRankings = 0;

    } else {
        cGlobal -> salteoRankings = 0;
        strcpy(tex->texts[INICIO_TEXTOS_RANKINGS].text, "Error obteniendo los rankings.");
        GHP_updateTextTexture(renderer, tex, INICIO_TEXTOS_RANKINGS, TAM_FUENTE_GRANDE, RED_COLOR);
        strcpy(tex->texts[TEXT_PRESIONEENTER].text, "[Presione enter para volver al menu]");
        GHP_updateTextTexture(renderer, tex, TEXT_PRESIONEENTER, TAM_FUENTE_GRANDE, BLACK_COLOR);
        printf("\nError obteniendo los rankings.\n");
    }
}

void _mostrarMovListaSDL(void* elemLista, void* infoExtra) {
    ctxImpReg* info = (ctxImpReg*)infoExtra;
    Coordenada* coord = (Coordenada*)elemLista;
    char* texto;
    char textoAAgregar[10] = "";

    if (info->regs_en_ult_linea >= 10) {
        if (info->ult_linea > 10)
            return;

        info->ult_linea++;
        info->regs_en_ult_linea=0;
    }

    texto = info->tex->texts[info->ult_linea].text;
    sprintf(textoAAgregar, "(%d,%d) ", coord->x, coord->y);
    strcat(texto, textoAAgregar);
    GHP_updateTextTexture(info->renderer, info->tex, info->ult_linea - INICIO_TEXTOS_MOVS, TAM_FUENTE_CHICO, BLACK_COLOR);
    info->regs_en_ult_linea++;
}