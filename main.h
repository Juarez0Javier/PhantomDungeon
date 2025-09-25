#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "dinMtx.h"
#include "graphics.h"
#include "gamestate.h"
#include "configs.h"
#include "menus.h"
#include "init.h"

void react(SDL_Renderer* renderer, void* gameData, GHP_TexturesData* TexData);

#endif // MAIN_H_INCLUDED
