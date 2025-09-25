#include "init.h"

bool initGame(GameState* game, ConfigData* configs, GHP_TexturesData* TexData, SDL_Renderer* renderer) {

    if (!initConfig(configs)) {
        if (!resetConfig()) {
            return false;
        }
        initConfig(configs);
    }

    applyConfig(configs, game);

    if (initTexData(TexData, renderer, game) != OK)
        return false;

    return true;
}

int initTexData(GHP_TexturesData* tex_data, SDL_Renderer* renderer, GameState* game) {

    // here setted in 0 in case there is an error before initializing them
    tex_data->buttons_loaded = 0;
    tex_data->texts_loaded = 0;

/*
    if (!GHP_setBG(renderer, tex_data, PATH_BG)) {
        return FILE_ERR;
    }
*/

    int stdDimGrid[] = { 8, 10, 15, 18, 24, 32, 40};
    int stdDimPix[]  = {76, 58, 48, 38, 29, 22, 16}; // TODO: CHECK
    //int dimGridTex = 0;
    int i = 0;
    /*
    while (!dimGridTex && i<AMMOUNT_ASSETS) {
        if (game->columns > stdDimGrid[i] || game->rows > stdDimGrid[i]) i++;
        else dimGridTex = stdDimGrid[i];
    }
    */
    if (i >= AMMOUNT_ASSETS) {
        printf("\nDimension error. There is no asset matchable with the dimension chosen.");
        // return DIM_ERR; TODO CHANGE
    }


    char path[19];
    sprintf(path, "img/celds%dx%d.png", stdDimGrid[i], stdDimGrid[i]); // CHANGE NAME OF ASSET
    while (GHP_loadRectAsset(renderer, path, &(tex_data->textures), AMMOUNT_TEXTURES, stdDimPix[i], stdDimPix[i], AMM_TEXT_COL_ASSET) != OK && i<= AMMOUNT_ASSETS ){
        // at least try to render other asset
        printf("\nFile of field %s not found, trying to use another asset.", path);
        i++;
        sprintf(path, "img/celds%dx%d.png", stdDimGrid[i], stdDimGrid[i]);
    }
    if (i > AMMOUNT_ASSETS) {
        printf("\nDimension error");
        return FILE_ERR;
    }
    if (!tex_data->textures) {
        printf("\nError loading textures.");
        return MEM_ERR;
    }
    tex_data->textures_loaded = AMMOUNT_TEXTURES;


    // mallocs for buttons and texts
    tex_data->buttons = malloc(sizeof(GHP_Button)*AMMOUNT_BUTTONS);
    tex_data->buttonsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_BUTTONS);
    tex_data->buttons_loaded = 0;

    tex_data->texts = malloc(sizeof(GHP_Text)*AMMOUNT_TEXTS);
    tex_data->textsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_TEXTS);
    tex_data->texts_loaded = 0;

    if (initButtons(renderer, tex_data) != OK) return TEX_ERR;
    if (initTexts(renderer, tex_data) != OK) return TEX_ERR;


    return OK;
}

int initButtons(SDL_Renderer* renderer, GHP_TexturesData* texData) {
    GHP_newButtonAbs(renderer, "img/buttons.png", texData, &texData->buttons[BUT_START], 0, 0, 289, 153, NULL);

    for(int i=0; i<AMMOUNT_BUTTONS; i++) {
        if (! (texData->buttons + i)->tex ) {
            printf("\nError loading the buttons. Button %d.", i);
            return TEX_ERR; // could be file
        }
    }
    return OK;
}

int initTexts(SDL_Renderer* renderer, GHP_TexturesData* texData) {

    return OK;
}



