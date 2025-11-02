#include "secciones.h"
#include "../conexion/api.h"
#include "../../libs/string/string.h"
#include <ctype.h>

typedef struct {
    tContextoGlobal* cGlobal;
    GHP_TexturesData* tex;
} tCTex;

bool buscarYCrearJugador (SOCKET sock, unsigned* id);
bool _procesarNombre(tContextoGlobal* cGlobal);

void impRankCompleto(void* elem, void* extra) {
    RankingCompleto* ranking = (RankingCompleto*) elem;
    tCTex* ctx = (tCTex*) extra;

    int nroReg = ctx->cGlobal->cantRankings + INICIO_TEXTOS_RANKINGS;

    // Armamos la línea completa a mostrar
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "%u  %s  %u",
             ranking->idJugador,
             ranking->nombre,
             ranking->puntTotal);

    // Copiamos al text correspondiente
    strcpy(ctx->tex->texts[nroReg].text, buffer);

    ctx->cGlobal->cantRankings++;

    printf("%d\n", ctx->cGlobal->salteoRankings);
    printf("%d\t%s\t%d\n", ranking->idJugador, ranking->nombre, ranking->puntTotal);
}

void prueba(void* a, int* b) {
    printf("Hola");
}

void _procesarFinPartida(Partida* partida, SOCKET socket, unsigned idJugador);

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
}

void handlerMenu (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {

    GHP_Button botonesActivos[] = {
        tex->buttons[BUT_JUGAR_GRANDE_A_NOMBRE],
        tex->buttons[BUT_SALIR_GRANDE],
        tex->buttons[BUT_VERCONFIG_GRANDE]
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

    handleButtonsClick(botonesActivos, 3, &(cGlobal -> partida), &(cGlobal -> seccion), event);
}

void initIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    system("cls");

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    if (cGlobal -> socket == INVALID_SOCKET) {
        strcpy(tex->texts[TEXT_ENTRADANOMBREMENSAJE].text, "Inicio de sesion no disponible.");
        GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREMENSAJE, 30, BLACK_COLOR);
        return;
    }

    GHP_renderButton(renderer, &tex->buttons[BUT_JUGAR_GRANDE], (WIDTH - tex->buttons[BUT_JUGAR_GRANDE].tex->width)/2, HEIGHT*0.7);
    tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[0] = '\0';

    strcpy(tex->texts[TEXT_ENTRADANOMBREMENSAJE].text, "Nombre de usuario:");

    // if (buscarYCrearJugador(cGlobal -> socket, &(cGlobal -> idJugador)))
    //     printf("ID: %d\n", cGlobal -> idJugador);
    // else
    //     printf("Error buscando / creando.\n");
    GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREMENSAJE, 30, BLACK_COLOR);

    tex->buttons[BUT_JUGAR_GRANDE].on_click = prueba;
    // cGlobal -> seccion = SECCION_MENU;

}

void handlerIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button buttons[] = {tex->buttons[BUT_JUGAR_GRANDE]};

    handleButtonsClick(buttons, 1, &cGlobal->partida, &cGlobal->seccion, event);

    // Si se esta conectado al servidor, permite escribir en el input.
    if (event->type == SDL_TEXTINPUT && cGlobal -> socket != INVALID_SOCKET) {
        if (strlen(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text)+1 < TAM_NOMBRE) {
            strcat(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text, event->text.text);
            GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, 30, BLACK_COLOR);
            tex->texts[TEXT_ERR_NOMBRE_JUGADOR].text[0] = '\0';
            GHP_updateTextTexture(renderer, tex, TEXT_ERR_NOMBRE_JUGADOR, 20, RED_COLOR);
        }
    } else if (event->type == SDL_KEYDOWN) {

        unsigned tamNombre = strlen(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text);

        switch(event->key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (tamNombre > 0) {
                    tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[tamNombre-1] = '\0';
                    GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, 30, BLACK_COLOR);
                }
                break;

            case SDLK_ESCAPE:
                cGlobal -> seccion = SECCION_MENU;
                tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[0] = '\0';
                GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, 30, BLACK_COLOR);
                break;

            case SDLK_RETURN:
                if (cGlobal -> socket != INVALID_SOCKET) {

                    if (strlen(tex->texts[TEXT_ENTRADANOMBREJUGADOR].text) == 0) {
                        strcpy(tex->texts[TEXT_ERR_NOMBRE_JUGADOR].text, "Se debe ingresar un nombre.");
                        GHP_updateTextTexture(renderer, tex, TEXT_ERR_NOMBRE_JUGADOR, 20, RED_COLOR);
                        return;
                    }

                    strncpy(
                        cGlobal -> nombreJugador,
                        tex->texts[TEXT_ENTRADANOMBREJUGADOR].text,
                        TAM_NOMBRE
                    );

                    cGlobal -> nombreJugador[TAM_NOMBRE] = '\0';
                    trimStr(cGlobal -> nombreJugador);

                    tex->texts[TEXT_ENTRADANOMBREJUGADOR].text[0] = '\0';
                    GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREJUGADOR, 30, BLACK_COLOR);

                    printf("Nombre jugador: %s", cGlobal -> nombreJugador);

                    if (_procesarNombre(cGlobal)) {
                        cGlobal -> seccion = SECCION_PARTIDA;
                    } else {
                        cGlobal -> socket = INVALID_SOCKET;
                        strcpy(tex->texts[TEXT_ENTRADANOMBREMENSAJE].text, "Inicio de sesion no disponible.");
                        GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREMENSAJE, 30, BLACK_COLOR);
                    }

                } else
                    cGlobal -> seccion = SECCION_PARTIDA;

                break;
        }
    }
}

void renderIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    if (cGlobal -> socket != INVALID_SOCKET) {
        GHP_renderTexture(renderer, tex->texts[TEXT_ENTRADANOMBREJUGADOR].tex, 52, HEIGHT * 0.4);
        dibujarRectanguloParaEntrada(renderer);
    }

    GHP_renderTexture(renderer, tex->texts[TEXT_ENTRADANOMBREMENSAJE].tex, 52, HEIGHT * 0.3);
    GHP_renderTexture(renderer, tex->texts[TEXT_ERR_NOMBRE_JUGADOR].tex, 52, HEIGHT * 0.55);
    GHP_renderButton(renderer, &tex->buttons[BUT_JUGAR_GRANDE], (WIDTH-tex->buttons[BUT_JUGAR_GRANDE].tex->width)/2, HEIGHT*0.7);
}

void initConfirmarReg(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    system("cls");

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    strcpy(tex->texts[TEXT_ENTRADANOMBREMENSAJE].text, "Usuario inexistente. ¿Registrar?");
    GHP_updateTextTexture(renderer, tex, TEXT_ENTRADANOMBREMENSAJE, 40, BLACK_COLOR);
}

void handlerConfirmarReg(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {

}

void renderConfirmarReg(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    GHP_renderTexture(renderer, tex->texts[TEXT_ENTRADANOMBREMENSAJE].tex, 52, 200);
    GHP_renderButton(renderer, &tex->buttons[BUT_JUGAR_GRANDE], (WIDTH-tex->buttons[BUT_JUGAR_GRANDE].tex->width)/2, HEIGHT*0.7);
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

    /*
    // Le podriamos pasar el nombre del archivo de laberinto por argumentos a main.
    // Una vez tengamos el algoritmo generador, reemplazarlo ac�.
    //if (!cargarMapaDeArchivo(&partida -> mapa, &partida -> jugador, &partida -> fantasmas, RUTA_LABERINTO_PRESET))
    if (cargarMapaDeArchivoNoSeguro(&partida -> mapa, &partida -> jugador, &partida -> fantasmas, RUTA_LABERINTO_PRESET) != OK) {
        printf("Error cargando el mapa del archivo.\n");
        cGlobal->seccion = SECCION_MENU;
    } else {
        mostrarMapa(&partida -> mapa);
        GHP_renderMesh(renderer, &(tex->active_mesh), 0);
    }
    */

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
        tex->buttons[BUT_SALIR_CHICO]
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

    handleButtonsClick(botonesActivos, 3, &(cGlobal -> partida), &(cGlobal -> seccion), event);

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

        printf("=======================\n");
        printf("Vidas: %d\n", partida -> vidasRestantes);
        printf("Premios obtenidos: %d\n", partida -> premiosObt);
        printf("Puntuacion: %d\n", partida -> puntuacion);
        printf("=======================\n");

        printf("\n");

        mostrarMapa(&partida -> mapa);
        actualizarMapaRender(renderer, &partida->mapa, tex, tex->active_mesh);

        // De vuelta porque sino queda como si estuviera pausado.
        GHP_renderButton(renderer, &tex->buttons[BUT_PAUSA_CHICO], WIDTH*0.41, HEIGHT*0.01);

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

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
    // Mostrar registro de movmientos.
}

void handlerDerrota(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};

    switch (event -> key.keysym.sym) {
        case SDLK_ESCAPE:
            cGlobal -> seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            cGlobal -> seccion = SECCION_PARTIDA;
            break;
    }

    handleButtonsClick(botonesActivos, 1, &(cGlobal -> partida), &(cGlobal -> seccion), event);
}

void initVictoria (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {

    printf("\n");
    printf("=======================\n");
    printf("Ganaste!\n");
    printf("Enter: Jugar de nuevo.\n");
    printf("Escape: Salir al menu.\n");
    printf("=======================\n\n");

    _procesarFinPartida(&(cGlobal -> partida), cGlobal -> socket, cGlobal -> idJugador);

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
    // Mostrar registro de movmientos.
}

void handlerVictoria(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};

    switch (event -> key.keysym.sym) {
        case SDLK_ESCAPE:
            cGlobal -> seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            cGlobal -> seccion = SECCION_PARTIDA;
            break;
    }

    handleButtonsClick(botonesActivos, 1, &(cGlobal -> partida), &(cGlobal -> seccion), event);
}

void initVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    int cod;
    unsigned cantObt;
    tCTex ctx = { cGlobal, tex };

    cGlobal -> cantRankings = 0;
    cGlobal -> salteoRankings = 0;

    // Intenta obtener los rankings.
    if (cGlobal -> socket != INVALID_SOCKET) {

        system("cls");
        printf("Rankings:\n");

        cod = apiObtenerRankings(
            cGlobal -> socket,
            &cantObt,
            LIMITE_RANKINGS,
            cGlobal -> salteoRankings,
            impRankCompleto,
            &ctx
        );

        if (cod == OK) {
            if (cantObt == 0) {
                cGlobal -> salteoRankings = 0;
                printf("\nFin. Enter: Volver al menu");
            } else {
                cGlobal -> salteoRankings += cantObt;
                printf("\nEnter: Proxima pagina");
            }

            cGlobal->cantRankings = 0;

        } else {
            cGlobal -> salteoRankings = 0;
            printf("\nError obteniendo los rankings.\n");
        }

    } else
        printf("\nNo se puede consultar al server.\n");

    for (int i = INICIO_TEXTOS_RANKINGS; i < LIMITE_RANKINGS + INICIO_TEXTOS_RANKINGS; i++) {
        GHP_updateTextTexture(renderer, tex, i, 30, BLACK_COLOR); // actualizar cada texture correctamente
    }
}

void handlerVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {

    int cod;
    unsigned cantObt = 0;
    tCTex ctx = { cGlobal, tex };

    if (event->type == SDL_KEYDOWN) {

        if (event -> key.keysym.sym == SDLK_RETURN) {

            if (cGlobal -> salteoRankings == 0) {
                cGlobal -> seccion = SECCION_MENU;
                return;
            }

            for (int i = INICIO_TEXTOS_RANKINGS; i < LIMITE_RANKINGS + INICIO_TEXTOS_RANKINGS; i++) {
                tex->texts[i].text[0] = '\0';
            }

            if (cGlobal -> socket != INVALID_SOCKET) {

                system("cls");
                printf("Rankings:\n");

                cod = apiObtenerRankings(
                    cGlobal -> socket,
                    &cantObt,
                    LIMITE_RANKINGS,
                    cGlobal -> salteoRankings,
                    impRankCompleto,
                    &ctx
                );

                if (cod == OK) {
                    if (cantObt == 0) {
                        cGlobal -> salteoRankings = 0;

                        int idx = INICIO_TEXTOS_RANKINGS + cGlobal->cantRankings;
                        strcpy(tex->texts[idx].text, "Fin del ranking");
                        GHP_updateTextTexture(renderer, tex, idx, 30, BLACK_COLOR);

                        printf("\nFin. Enter: Volver al menu");
                    } else {
                        cGlobal -> salteoRankings += cantObt;
                        printf("\nEnter: Proxima pagina");
                    }

                    cGlobal->cantRankings = 0;

                    for (int i = INICIO_TEXTOS_RANKINGS; i < LIMITE_RANKINGS + INICIO_TEXTOS_RANKINGS; i++) {
                        GHP_updateTextTexture(renderer, tex, i, 30, BLACK_COLOR); // actualizar cada texture correctamente
                    }

                } else {
                    cGlobal -> salteoRankings = 0;
                    printf("\nError obteniendo los rankings.\n");
                }

            } else
                printf("\nNo se puede consultar al server.\n");
        }

        if (event -> key.keysym.sym == SDLK_ESCAPE)
            cGlobal -> seccion = SECCION_MENU;
    }
}

void renderVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);

    for (int i = INICIO_TEXTOS_RANKINGS; i < LIMITE_RANKINGS + INICIO_TEXTOS_RANKINGS; i++) {
        GHP_renderTexture(renderer, tex->texts[i].tex, 52, HEIGHT * (i - INICIO_TEXTOS_RANKINGS) * 0.1);
    }
}

void initVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex) {
    printf("\nConfiguraciones:\n");
    mostrarConfigs(&(cGlobal -> configData));
    printf("\nPresione enter para volver al menu...");

    GHP_renderBG(renderer, tex, WIDTH, HEIGHT);
    GHP_renderButton(renderer, &tex->buttons[BUT_MENU_GRANDE], (WIDTH-(231-28))/2 , HEIGHT*0.2);
}

void handlerVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event) {
    GHP_Button botonesActivos[] = {tex->buttons[BUT_MENU_GRANDE]};

    if (event->type == SDL_KEYDOWN) {
        switch (event -> key.keysym.sym) {
            case SDLK_RETURN:
                cGlobal -> seccion = SECCION_MENU;
                break;
            case SDLK_ESCAPE:
                cGlobal -> seccion = SECCION_SALIR_DIRECTO;
                break;
        }
    }

    handleButtonsClick(botonesActivos, 1, &(cGlobal -> partida), &(cGlobal -> seccion), event);
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

void _procesarFinPartida(Partida* partida, SOCKET socket, unsigned idJugador) {

    unsigned idPartida, cMovs = 0;
    int cod;

    printf("Movimientos realizados:\n");

    mostrarLista(&partida -> regMovs, mostrarCoordenada);
    reducirLista(&partida -> regMovs, contarMovs, &cMovs);
    vaciarLista(&partida -> regMovs);

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

void dibujarRectanguloParaEntrada(SDL_Renderer* renderer) {
    SDL_Rect rect = {50, HEIGHT*0.4, WIDTH-50*2, 50};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

bool _procesarNombre(tContextoGlobal* cGlobal) {

    int cod;
    Jugador jug;

    cod = apiBuscarJugador(cGlobal -> socket, &jug, OPCION_NOMBRE, 0, cGlobal -> nombreJugador);

    if (cod == NO_ENCONTRADO) {

        cod = apiCrearJugador(
            cGlobal -> socket,
            &(cGlobal -> idJugador),
            cGlobal -> nombreJugador,
            "email@gmail.com",
            "prueba"
        );
    } else
        cGlobal -> idJugador = jug.id;

    return cod == OK;
}

bool buscarYCrearJugador (SOCKET sock, unsigned* id) {

    Jugador jugador;
    int cod;
    bool err = false;

    char nombre[TAM_NOMBRE +1];
    char email[TAM_EMAIL +1] = "fab@gmail.com";
    char contrasenia[TAM_CONTRASENIA +1] = "deltarune";

    char op;

    // Inicializa con un valor invalido para iterar hasta tener uno valido.
    *id = 0;

    while (!err && *id == 0) {
        printf("Ingresa tu nombre: ");
        ingrStrNoVacio(nombre, TAM_NOMBRE, "Nombre vacio, reingresar: ");

        cod = apiBuscarJugador(sock, &jugador, OPCION_NOMBRE, 0, nombre);

        switch(cod) {
            case OK:
                imprimirJugador(&jugador);
                *id = jugador.id;
                break;
            case NO_ENCONTRADO:
                // Hacer esto por la UI.
                printf("No existe un jugador con este nombre, deseas registrarlo? (S/N): ");
                scanf("%c", &op);
                fflush(stdin);

                op = toupper(op);

                if (op == 'S') {
                    // Solicitar email
                    // Solicitar contrasenia

                    cod = apiCrearJugador(sock, id, nombre, email, contrasenia);

                    // cod == OK
                    // cod == ERR_FORMATO
                    // cod == ERR_ARCHIVO
                    // cod == ERR_CONEXION
                    if (cod != OK) {
                        printf("Error creando al jugador\n");
                        err = true;
                    }
                } else
                    printf("Jugador no creado\n");

                break;
            default:
                printf("Error de formato, conexion o archivo buscando al jugador\n");
                err = true;
        }
    }

    return !err;
}
