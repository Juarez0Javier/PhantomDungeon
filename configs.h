#ifndef CONFIGS_H_INCLUDED
#define CONFIGS_H_INCLUDED

#include "constants.h"
#include "gamestate.h"

typedef struct {
    bool sth;
} ConfigData;

bool initConfig(ConfigData* game);
bool validConfig(ConfigData* game);
void applyConfig(ConfigData* configs, GameState* game);
void nullConfig(ConfigData* configs);
bool resetConfig();

#endif // CONFIGS_H_INCLUDED
