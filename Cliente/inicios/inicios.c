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

    tex_data->buttons = malloc(sizeof(GHP_Button)*AMMOUNT_BUTTONS);
    tex_data->buttonsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_BUTTONS);
    tex_data->buttons_loaded = 0;

    if (iniciarBotones(renderer, tex_data) != OK)
        return TEX_ERR;

    return OK;
}

int iniciarBotones(SDL_Renderer* renderer, GHP_TexturesData* texData) {

    GHP_newButtonAbs(renderer, "./src/img/botones.png", texData, &texData->buttons[BUT_MENU_CHICO], 132, 19, 248, 53, setSeccionMenu);
    GHP_newButtonAbs(renderer, "./src/img/botones.png", texData, &texData->buttons[BUT_SALIR_CHICO], 312, 21, 428, 55, setSeccionSalir);
    GHP_newButtonAbs(renderer, "./src/img/botones.png", texData, &texData->buttons[BUT_PAUSA_CHICO], 455, 21, 571, 55, setPausa);
    GHP_newButtonAbs(renderer, "./src/img/botones.png", texData, &texData->buttons[BUT_JUGAR_GRANDE], 28, 85, 231, 165, setSeccionJugar);
    GHP_newButtonAbs(renderer, "./src/img/botones.png", texData, &texData->buttons[BUT_SALIR_GRANDE], 283, 87, 486, 167, setSeccionSalir);
    GHP_newButtonAbs(renderer, "./src/img/botones.png", texData, &texData->buttons[BUT_VERCONFIG_GRANDE], 546, 86, 749, 166, setSeccionConfigs);
    GHP_newButtonAbs(renderer, "./src/img/botones.png", texData, &texData->buttons[BUT_MENU_GRANDE], 28, 195, 231, 275, setSeccionMenu);

    for(int i=0; i<AMMOUNT_BUTTONS; i++) {
        if (! (texData->buttons + i)->tex ) {
            printf("\nError loading the buttons. Button %d.", i);
            return TEX_ERR; // could be file
        }
    }

    return OK;
}




// reacciones de botones
void setSeccionMenu(void* dataJuego, int* seccion) {*seccion = SECCION_MENU;}
void setSeccionSalir(void* dataJuego, int* seccion) {*seccion = SECCION_SALIR_DIRECTO;}
void setPausa(void* dataJuego, int* seccion) {((Partida*)dataJuego)->pausado = !((Partida*)dataJuego)->pausado;}
void setSeccionJugar(void* dataJuego, int* seccion) {*seccion = SECCION_PARTIDA;}
void setSeccionConfigs(void* dataJuego, int* seccion) {*seccion = SECCION_CONFIGS;}

/*

    // mallocs for buttons and texts

//
//    tex_data->texts = malloc(sizeof(GHP_Text)*AMMOUNT_TEXTS);
//    tex_data->textsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_TEXTS);
//    tex_data->texts_loaded = 0;
//

//    if (initTexts(renderer, tex_data) != OK) return TEX_ERR;

int iniciarTextos(SDL_Renderer* renderer, GHP_TexturesData* texData) {

    return OK;
}
*/
