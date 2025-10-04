#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include "../constants.h"
#include "../configs/configs.h"
#include "../inicios/inicios.h"
#include "../partida/partida.h"
#include "../mapa/mapa.h"
#include "../movimiento/movimiento.h"

#define MAX_LEN_SETTINGS_TEXT 10

#define INPUTCHARSETVAL(x) ((x>47 && x<58) || x==120 || x=='%' || x=='/')


#ifndef _WIN32
#include <windows.h>
#endif // _WIN32

//#include "otrasFunciones.h"

typedef struct {
    void (*init)(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
    void (*handler)(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, SDL_Event* event, int* mode);
    void (*render)(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, int* mode);
} Seccion;

// here should be functions of specific menus
// also the mesh updater
// and the init, handler and render for each menu

void initMenu(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handlerMenu(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, SDL_Event* event, int* seccion);

void initJuegoCorriendo (SDL_Renderer* renderer, Partida* partida, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handleJuegoCorriendo (SDL_Renderer* renderer, Partida* partida, GHP_TexturesData* tex, SDL_Event* event, int* seccion);
void renderJuegoCorriendo (SDL_Renderer* renderer, Partida* partida, GHP_TexturesData* tex, int* mode);

void initDerrota(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handlerDerrota(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, SDL_Event* event, int* mode);

void initVictoria(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handlerVictoria(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, SDL_Event* event, int* mode);

void initVerConfigs(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handlerVerConfigs(SDL_Renderer* renderer, Partida* game, GHP_TexturesData* tex, SDL_Event* event, int* seccion);




// otros
void handleButtonsClick(GHP_Button* buttons, int ammount, int x, int y, Partida* game, int* mode, SDL_Event* event);

#endif // MENUS_H_INCLUDED
