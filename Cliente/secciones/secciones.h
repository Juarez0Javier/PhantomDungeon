#include <SDL.h>
#include "../partida/partida.h"

typedef struct {
    void (*init) (Partida* partida);
    void (*handle) (SDL_Event* e, Partida* partida, int* seccion);
    void (*render) (Partida* partida);
} Seccion;

void initMenu (Partida* partida);
void handleMenu (SDL_Event* e, Partida* partida, int* seccion);
void renderMenu (Partida* partida);

void initJuegoCorriendo (Partida* partida);
void handleJuegoCorriendo (SDL_Event* e, Partida* partida, int* seccion);
void renderJuegoCorriendo (Partida* partida);

void initDerrota (Partida* partida);
void handleDerrota (SDL_Event* e, Partida* partida, int* seccion);
void renderDerrota (Partida* partida);

void initVictoria (Partida* partida);
void handleVictoria (SDL_Event* e, Partida* partida, int* seccion);
void renderVictoria (Partida* partida);
