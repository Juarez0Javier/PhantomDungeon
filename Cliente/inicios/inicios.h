#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include "../constants.h"
#include "../configs/configs.h"
#include "../partida/partida.h"

bool iniciarPartida(Partida* game, ConfigData* configs, GHP_TexturesData* TexData, SDL_Renderer* renderer);
int iniciarBotones(SDL_Renderer* renderer, GHP_TexturesData* texData);
int iniciarTextos(SDL_Renderer* renderer, GHP_TexturesData* texData);
int iniciarTexData(GHP_TexturesData* tex_data, SDL_Renderer* renderer, Partida* game);

// here also should be buttons reactions

#endif // INIT_H_INCLUDED
