#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <stdlib.h>

#include "constants.h"
#include "graphics.h"
#include "configs.h"
#include "init.h"
#include "gamestate.h"

#include "mapa.h"

#define MAX_LEN_SETTINGS_TEXT 10

#define INPUTCHARSETVAL(x) ((x>47 && x<58) || x==120 || x=='%' || x=='/')


#ifndef _WIN32
#include <windows.h>
#endif // _WIN32

#include "otrasFunciones.h"

typedef struct Section {
    void (*init)(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
    void (*handler)(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, SDL_Event* event, int* mode);
    void (*render)(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, int* mode);
} Section;

// here should be functions of specific menus
// also the mesh updater
// and the init, handler and render for each menu

void initMenu(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handlerMenu(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, SDL_Event* event, int* mode);

void initLost(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handlerLost(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, SDL_Event* event, int* mode);

void initWin(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, ConfigData* configData, int* mode);
void handlerWin(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* tex, SDL_Event* event, int* mode);

#endif // MENUS_H_INCLUDED
