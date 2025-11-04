#include "inicios.h"

bool iniciarPartida(Partida* partida, ConfigData* configs, GHP_TexturesData* TexData, SDL_Renderer* renderer) {

    configs -> por_defecto = false;

    if (!iniciarConfig(configs)) {

        // Sobreescribe el archivo de configuraciones con parametros validos.
        if (!resetConfig())
            return false;
        
        iniciarConfig(configs);
        configs -> por_defecto = true;
    }

    if (!crearPartida(partida, configs->filas, configs->columnas)) {
        printf("Error creando la configuracion de partida\n");
        return false;
    }

    if (iniciarTexData(TexData, renderer, partida) != OK) {
        printf("Error creando la configuracion de video\n");
        return false;
    }

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


    int stdDimGrid[] = {11, 15, 20};
    int stdDimPix[]  = {40, 30, 20};
    int dimGridTex = 0, i = 0;
    int ammount_assets = sizeof(stdDimGrid)/sizeof(int);

    while (!dimGridTex && i<ammount_assets) {
        if (partida->mapa.filas > stdDimGrid[i] || partida->mapa.cols > stdDimGrid[i]) i++;
        else dimGridTex = stdDimGrid[i];
    }
    if (i >= ammount_assets) {
        printf("\nDimension error. There is no asset matchable with the dimension chosen.");
        return DIM_ERR;
    }

    char path[25];
    sprintf(path, "./src/img/asset%dx%d.png", stdDimGrid[i], stdDimGrid[i]);
    while (GHP_loadRectAsset(renderer, path, &(tex_data->textures), AMMOUNT_TEXTURES, stdDimPix[i], stdDimPix[i], AMM_TEXT_COL_ASSET) != OK && i<= ammount_assets ){
        // at least try to render other asset
        printf("\nFile of field %s not found, trying to use another asset.", path);
        i++;
        sprintf(path, "img/celds%dx%d.png", stdDimGrid[i], stdDimGrid[i]);
    }

    if (!tex_data->textures) {
        printf("\nError loading textures.");
        return MEM_ERR;
    }
    tex_data->textures_loaded = AMMOUNT_TEXTURES;

    // set mesh
    int offset_to_centerX = (WIDTH - partida->mapa.cols * stdDimPix[i]) / 2;
    //int offset_to_centerY = (HEIGHT - partida->mapa.filas * stdDimPix[i]) / 2;
    tex_data->active_mesh = (GHP_Mesh){offset_to_centerX, TAM_HEADER_PARTIDA, &(tex_data->textures[0]), partida->mapa.filas, partida->mapa.cols};

    tex_data->buttons = malloc(sizeof(GHP_Button)*AMMOUNT_BUTTONS);
    tex_data->buttonsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_BUTTONS);
    tex_data->buttons_loaded = 0;

    if (iniciarBotones(renderer, tex_data) != OK)
        return TEX_ERR;

    tex_data->texts = malloc(sizeof(GHP_Text)*AMMOUNT_TEXTS);
    tex_data->textsTexs = malloc(sizeof(GHP_Texture)*AMMOUNT_TEXTS);
    tex_data->texts_loaded = 0;

    if (iniciarTextos(renderer, tex_data) != OK)
        return TEX_ERR;

    return OK;
}

int iniciarBotones(SDL_Renderer* renderer, GHP_TexturesData* texData) {

    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_MENU_CHICO], 132, 19, 248, 53, setSeccionMenu);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_SALIR_CHICO], 312, 21, 428, 55, setSeccionSalir);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_PAUSA_CHICO], 455, 21, 571, 55, setPausa);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_JUGAR_GRANDE], 28, 85, 231, 165, setSeccionJugar);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_SALIR_GRANDE], 283, 87, 486, 167, setSeccionSalir);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_VERCONFIG_GRANDE], 546, 86, 749, 166, setSeccionConfigs);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_MENU_GRANDE], 28, 195, 231, 275, setSeccionMenu);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_JUGAR_GRANDE_A_NOMBRE], 28, 85, 231, 165, setSeccionIngresoNombre);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_VERRANKING_GRANDE], 547, 207, 750, 287, setSeccionVerRankings);
    GHP_newButtonAbs(renderer, RUTA_BOTONES, texData, &texData->buttons[BUT_VERMOVIMIENTOS_GRANDE], 326, 207, 529, 287, setSeccionVerMovs);

    for(int i=0; i<AMMOUNT_BUTTONS; i++) {
        if (! (texData->buttons + i)->tex ) {
            printf("\nError loading the buttons. Button %d.", i);
            return TEX_ERR; // could be file
        }
    }

    return OK;
}

int iniciarTextos(SDL_Renderer* renderer, GHP_TexturesData* texData) {
    SDL_Color whiteColor = {255, 255, 255, 255};
    char path[] = "fnt/Consolas-Regular.ttf";
    for (int i=0; i<AMMOUNT_TEXTS; i++) {
        GHP_newText(renderer, path, texData, &(texData->texts[i]), -1, -1, -1, whiteColor);
        texData->texts[i].text[0] = '\0';
    }

    for(int i=0; i<AMMOUNT_TEXTS; i++) {
        if (! (texData->texts + i)->tex ) {
            printf("\nError loading the textures. Texture %d.", i);
            return TEX_ERR;
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
void setSeccionIngresoNombre(void* dataJuego, int* seccion) {*seccion = SECCION_INGRESO_NOMBRE;}
void setSeccionVerRankings(void* dataJuego, int* seccion) {*seccion = SECCION_RANKINGS;}
void setSeccionVerMovs(void* dataJuego, int* seccion) {*seccion = SECCION_VERMOVS;}
