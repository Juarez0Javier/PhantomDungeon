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

void initInicioSesion(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerInicioSesion(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initMenu(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerMenu(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);
void renderIngresoNombre(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);

void initConfirmarReg(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerConfirmarReg(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);
void renderConfirmarReg(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);

void initJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handleJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);
void renderJuegoCorriendo (tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);

void initDerrota(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerDerrota(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initVictoria(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerVictoria(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerVerRankings(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

void initVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex);
void handlerVerConfigs(tContextoGlobal* cGlobal, SDL_Renderer* renderer, GHP_TexturesData* tex, SDL_Event* event);

// otros
void handleButtonsClick(GHP_Button* botones, int cantidad, Partida* partida, int* seccion, SDL_Event* event);
void dibujarRectanguloParaEntrada(SDL_Renderer* renderer);

#endif // MENUS_H_INCLUDED
