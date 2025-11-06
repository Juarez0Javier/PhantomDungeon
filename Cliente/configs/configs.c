#include "configs.h"

bool iniciarConfig(ConfigData* configs) {

    Vector campos;
    char buffer[TAM_LINEA + 1];
    campoConfig campoBuffer;
    int camposLeidos = 0;
    FILE* archConfigs = fopen(RUTA_CONFIGS, "rt");

    if (!archConfigs)
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
        campoConfig* campConf = (campoConfig*) vectorObtenerElementoSegunPos(&campos, pos);
        *(camposValidos[i].p) = campConf->valor;
        camposLeidos++;
    }

    vectorDestruir(&campos);
    fclose(archConfigs);

    if (camposLeidos < sizeof(camposValidos)/sizeof(campoValido))
        return false;

    if (!validarConfig(configs))
        return false;

    mostrarConfigs(configs);
    return true;
}

bool validarConfig(ConfigData* configs) {
    return !(
        configs->filas < MIN_FILAS || configs->filas > MAX_FILAS
        || configs->columnas < MIN_COLUMNAS || configs->columnas > MAX_COLUMNAS
        || configs->maximo_numero_fantasmas < MIN_NUM_FANTASMAS || configs->maximo_numero_fantasmas > MAX_NUM_FANTASMAS
        || configs->maximo_numero_premios < MIN_NUM_PREMIOS || configs->maximo_numero_premios > MAX_NUM_PREMIOS
        || configs->maximo_vidas_extra < MIN_VIDAS_EXTRA || configs->maximo_vidas_extra > MAX_VIDAS_EXTRA
        || configs->vidas_inicio < MIN_VIDAS_INICIO || configs->vidas_inicio > MAX_VIDAS_INICIO
    );
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
    FILE* archConfig = fopen(RUTA_CONFIGS, "wt");
    if (!archConfig)
        return false;
    fprintf(archConfig, "filas: %d\ncolumnas: %d\nvidas_inicio: %d\nmaximo_numero_fantasmas: %d\nmaximo_numero_premios: %d\nmaximo_vidas_extra: %d\n",
        CONFIG_FILAS, CONFIG_COLS, CONFIG_VIDAS, CONFIG_MAX_NUM_FANT, CONFIG_MAX_NUM_PREM, CONFIG_MAX_VIDAS_EXTRA
    );
    fclose(archConfig);
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

