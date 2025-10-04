#include "inicios.h"

bool iniciarPartida(Partida* partida, ConfigData* configs, GHP_TexturesData* TexData, SDL_Renderer* renderer) {

    if (!iniciarConfig(configs)) {
        if (!resetConfig()) {
            return false;
        }
        iniciarConfig(configs);
    }

    aplicarConfig(configs, partida);

    if (!crearPartida(partida)) {
        printf("Error creando la configuracion de partida");
    }

    if (iniciarTexData(TexData, renderer, partida) != OK)
        return false;

    return true;
}

int iniciarTexData(GHP_TexturesData* tex_data, SDL_Renderer* renderer, Partida* partida) {

    // set BG
    if (!GHP_setBG(renderer, tex_data, RUTA_FONDO)) {
        printf("\nNo se pudo cargar el fondo.");
        //return ERR_ARCHIVO; // u can play without bg
    }

    // here setted in 0 in case there is an error before initializing them
    tex_data->buttons_loaded = 0;
    tex_data->texts_loaded = 0;

    GHP_loadRectAsset(renderer, PATH_ASSET, &(tex_data->textures), AMMOUNT_TEXTURES, 40, 40, AMM_TEXT_COL_ASSET);

    if (!tex_data->textures) {
        printf("\nError loading textures.");
        return MEM_ERR;
    }
    tex_data->textures_loaded = AMMOUNT_TEXTURES;

    // set mesh
    int offset_to_centerX = (WIDTH - partida->mapa.cols * 40) / 2;
    int offset_to_centerY = (HEIGHT - partida->mapa.filas * 40) / 2;
    tex_data->active_mesh = (GHP_Mesh){offset_to_centerX, offset_to_centerY, &(tex_data->textures[0]), partida->mapa.filas, partida->mapa.cols};

    return OK;
}



/*




    // mallocs for buttons and texts
//    tex_data->buttons = malloc(sizeof(GHP_Button)*AMMOUNT_BUTTONS);
//    tex_data->buttonsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_BUTTONS);
//    tex_data->buttons_loaded = 0;
//
//    tex_data->texts = malloc(sizeof(GHP_Text)*AMMOUNT_TEXTS);
//    tex_data->textsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_TEXTS);
//    tex_data->texts_loaded = 0;
//
//    if (initButtons(renderer, tex_data) != OK) return TEX_ERR;
//    if (initTexts(renderer, tex_data) != OK) return TEX_ERR;



int iniciarBotones(SDL_Renderer* renderer, GHP_TexturesData* texData) {

//    GHP_newButtonAbs(renderer, "img/buttons.png", texData, &texData->buttons[BUT_START], 0, 0, 289, 153, NULL);
//
//    for(int i=0; i<AMMOUNT_BUTTONS; i++) {
//        if (! (texData->buttons + i)->tex ) {
//            printf("\nError loading the buttons. Button %d.", i);
//            return TEX_ERR; // could be file
//        }
//    }

    return OK;
}

int iniciarTextos(SDL_Renderer* renderer, GHP_TexturesData* texData) {

    return OK;
}
*/
