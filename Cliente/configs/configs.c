#include "configs.h"

bool iniciarConfig(ConfigData* configs) {

    Vector campos;
    char buffer[TAM_LINEA + 1];
    campoConfig campoBuffer;
    FILE* archConfigs = fopen(RUTA_CONFIGS, "rt");

    if (archConfigs)
        return false;

    vectorCrear(&campos, sizeof(campoConfig));

    while (fgets(buffer, TAM_LINEA + 1, archConfigs)) {
        sscanf(buffer, "%[^:]: %f", campoBuffer.argumento, &campoBuffer.valor);
        vectorInsertarAlFinal(&campos, &campoBuffer);
    }

    campoValido camposValidos[] = {
        {"filas", &configs->filas},
        {"columnas", &configs->columnas},
        {"vidas_inicio",&configs->vidas_inicio},
        {"maximo_numero_fantasmas",&configs->maximo_numero_fantasmas},
        {"maximo_numero_premios",&configs->maximo_numero_premios},
        {"maximo_vidas_extra",&configs->maximo_vidas_extra}
    };

    for (int i = 0; i < sizeof(camposValidos) / sizeof(campoValido); i++) {
        int pos = vectorDesordBuscar(&campos, camposValidos[i].argumento, cmpCampoTxt);
        campoConfig* campConf = (campoConfig*)campos.vec + pos;
        *(camposValidos[i].p) = campConf->valor;
    }

    mostrarConfigs(configs);

    fclose(archConfigs);
    return true;
}

bool validarConfig(ConfigData* configs) {
    return !(
        configs->filas < MIN_FILAS || configs->filas > MAX_FILAS
        || configs->columnas < MIN_COLUMNAS || configs->columnas > MAX_COLUMNAS
        || configs->maximo_numero_fantasmas < MIN_NUM_FANTASMAS || configs->maximo_numero_fantasmas > MAX_NUM_FANTASMAS // kinda weird, ask. TODO
        || configs->maximo_numero_premios < MIN_NUM_PREMIOS || configs->maximo_numero_premios > MAX_NUM_PREMIOS
        || configs->maximo_vidas_extra < MIN_VIDAS_EXTRA || configs->maximo_vidas_extra > MAX_VIDAS_EXTRA
        || configs->vidas_inicio < MIN_VIDAS_INICIO || configs->vidas_inicio > MAX_VIDAS_INICIO
    );
}

void aplicarConfig(ConfigData* configs, GameState* game) { // to put in game the configs

}

void nullConfig(ConfigData* configs) {
    configs->columnas = -1;
    configs->filas = -1;
    configs->maximo_numero_fantasmas = -1;
    configs->maximo_numero_premios = -1;
    configs->maximo_vidas_extra = -1;
    configs->vidas_inicio = -1;
}

bool resetConfig() {
    FILE* archConfig = fopen("config.txt", "wt");
    if (!archConfig)
        return false;
    printf("filas: 10\ncolumnas: 10\nvidas_inicio: 3\nmaximo_numero_fantasmas: 2\nmaximo_numero_premios: 1\nmaximo_vidas_extra: 1\n");
    return true;
}

int cmpCampoTxt(const void* camp, const void* txt) {
    campoConfig* campo = (campoConfig*)camp;
    char* texto = (char*)txt;
    return strcmp(campo->argumento, texto);
}

void impCampo(const void* campo) {
    campoConfig* campoConf = (campoConfig*)campo;
    printf("\n<Campo Argumento:%s Valor:%f>", campoConf->argumento, campoConf->valor);
}

void mostrarConfigs(ConfigData* configs) {
    printf("\nConfigs:\nfilas:%.0f\ncolumnas:%.0f\nvidas_inicio:%.0f\nmaximo_numero_fantasmas:%.0f\nmaximo_numero_premios:%.0f\nmaximo_vidas_extra:%.0f",
        configs->filas, configs->columnas, configs->vidas_inicio, configs->maximo_numero_fantasmas, configs->maximo_numero_premios, configs->maximo_vidas_extra
    );
}

