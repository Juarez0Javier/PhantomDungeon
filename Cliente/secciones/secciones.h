#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include "../global.h"
#include "../constants.h"
#include "../configs/configs.h"
#include "../inicios/inicios.h"
#include "../partida/partida.h"
#include "../mapa/mapa.h"
#include"../mapa/mapaRandom.h"
#include "../movimiento/movimiento.h"

#define MAX_LEN_SETTINGS_TEXT 10

#define INPUTCHARSETVAL(x) ((x>47 && x<58) || x==120 || x=='%' || x=='/')


#ifndef _WIN32
#include <windows.h>
#endif // _WIN32

//#include "otrasFunciones.h"

typedef struct {
    void (*init)(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
    void (*handler)(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);
    void (*render)(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
} Seccion;

// here should be functions of specific menus
// also the mesh updater
// and the init, handler and render for each menu

void initMenu(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerMenu(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handleJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);
void renderJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);

void initDerrota(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerDerrota(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initVictoria(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerVictoria(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

// otros
void handleButtonsClick(GHP_Button* buttons, int ammount, int x, int y, Partida* game, int* mode, SDL_Event* event);

#endif // MENUS_H_INCLUDED
