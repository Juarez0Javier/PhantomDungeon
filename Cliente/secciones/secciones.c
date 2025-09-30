#include <stdio.h>
#include <SDL.h>
#include "./secciones.h"
#include "../mapa/mapa.h"
#include "../partida/partida.h"
#include "../constantes.h"
#include "../auxiliares/matriz/matriz.h"
#include "../../Comun/comun.h"

void initMenu (Partida* partida) {
    system("cls");

    printf("=======================\n");
    printf("||  Phantom Dungeon  ||\n");
    printf("=======================\n\n");

    printf("Enter: Jugar\n");
    printf("Escape: Salir\n");
}

void handleMenu(SDL_Event* e, Partida* partida, int* seccion) {
    SDL_Event keyEvent;

    if (e -> type == SDL_KEYDOWN) {
        switch (e -> key.keysym.sym) {
            case SDLK_RETURN:
                *seccion = SECCION_PARTIDA;
                break;
            case SDLK_ESCAPE:
                keyEvent.type = SDL_QUIT;
                SDL_PushEvent(&keyEvent);
                break;
        }
    }
}

void renderMenu (Partida* partida) {

}

void initJuegoCorriendo (Partida* partida) {

    printf("Init juego\n");

    partida -> pausado = false;
    partida -> vidasRestantes = CONFIG_VIDAS;
    partida -> premiosObt = 0;
    partida -> puntuacion = 0;

    // Le podriamos pasar el nombre del archivo de laberinto por argumentos a main.
    // Una vez tengamos el algoritmo generador, reemplazarlo acá.
    if (!cargarMapaDeArchivo(&partida -> mapa, &partida -> jugador, &partida -> fantasmas, "mapa/laberinto.txt"))
        printf("Error cargando el mapa del archivo.\n");
    
    mostrarMapa(&partida -> mapa);
}

void handleJuegoCorriendo (SDL_Event* e, Partida* partida, int* seccion) {

    if (e -> type == SDL_KEYDOWN) {
        char entrada = teclaAWASD(e -> key.keysym.sym);

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

            // Primero se fija si el jugador hizo un movimiento.
            switch (entrada) {
                case ARRIBA:
                case ABAJO:
                case IZQUIERDA:
                case DERECHA:
                    calcularMovJugador(
                        &partida -> jugador,
                        &partida -> mapa,
                        entrada,
                        &partida -> movs
                    );
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

            // Luego, calcula el movimiento de todos los fantasmas.
            calcularMovFantasmas(
                &partida -> fantasmas, 
                &partida -> mapa, 
                &partida -> movs
            );
        }
    }

    // Probablemente haya una mejor forma de manejar esto, quizás no hace falta el condicional.
    if (!partida -> pausado) {
        resolverMovimientos(
            partida,
            &partida -> mapa,
            &partida -> movs,
            seccion
        );
    }

    if (*seccion != SECCION_PARTIDA) {
        renderJuegoCorriendo(partida);
        vaciarCola(&partida -> movs);
        vectorVaciar(&partida -> fantasmas);
        return;
    }
}

void renderJuegoCorriendo (Partida* partida) {

    if (!partida -> pausado) {
        system("cls");

        printf("=======================\n");
        printf("Vidas: %d\n", partida -> vidasRestantes);
        printf("Premios obtenidos: %d\n", partida -> premiosObt);
        printf("Puntuacion: %d\n", partida -> puntuacion);
        printf("=======================\n");

        printf("\n");

        mostrarMapa(&partida -> mapa);
    }
}

void initDerrota (Partida* partida) {
    printf("\n");
    printf("=======================\n");
    printf("Perdiste!\n");
    printf("Enter: Jugar de nuevo.\n");
    printf("Escape: Salir al menu.\n");
    printf("=======================\n");

    // Mostrar registro de movmientos.
}

void handleDerrota (SDL_Event* e, Partida* partida, int* seccion) {

    switch (e -> key.keysym.sym) {
        case SDLK_ESCAPE:
            *seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            *seccion = SECCION_PARTIDA;
            break;
    }
}

void renderDerrota (Partida* partida) {
    
}

void initVictoria (Partida* partida) {
    printf("\n");
    printf("=======================\n");
    printf("Ganaste!\n");
    printf("Enter: Jugar de nuevo.\n");
    printf("Escape: Salir al menu.\n");
    printf("=======================\n");

    // Mostrar registro de movmientos.
}

void handleVictoria (SDL_Event* e, Partida* partida, int* seccion) {
    
    switch (e -> key.keysym.sym) {
        case SDLK_ESCAPE:
            *seccion = SECCION_MENU;
            break;
        case SDLK_RETURN:
            *seccion = SECCION_PARTIDA;
            break;
    }
}

void renderVictoria (Partida* partida) {
}