#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "constants.h"
#include "configs.h"
#include "graphics.h"
#include "gamestate.h"

bool initGame(GameState* game, ConfigData* configs, GHP_TexturesData* TexData, SDL_Renderer* renderer);
int initButtons(SDL_Renderer* renderer, GHP_TexturesData* texData);
int initTexts(SDL_Renderer* renderer, GHP_TexturesData* texData);
int initTexData(GHP_TexturesData* tex_data, SDL_Renderer* renderer, GameState* game);
bool initMtx(GameState* game);
void adjustDim(SDL_Renderer* renderer, GameState* game, GHP_TexturesData* TexData, int dim[2], int* mode);

// here also should be buttons reactions

#endif // INIT_H_INCLUDED
