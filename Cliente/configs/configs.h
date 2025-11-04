#ifndef CONFIGS_H_INCLUDED
#define CONFIGS_H_INCLUDED

#include "../constants.h"
#include "../partida/partida.h"

#define TAM_LINEA 50

typedef struct {
    float filas;
    float columnas;
    float vidas_inicio;
    float maximo_numero_fantasmas;
    float maximo_numero_premios;
    float maximo_vidas_extra;
    bool por_defecto;
} ConfigData;

typedef struct {
    char argumento[51];
    float valor;
} campoConfig;

typedef struct {
    char argumento[51];
    float* p;
} campoValido;

bool iniciarConfig(ConfigData* configs);
bool validarConfig(ConfigData* configs);
void nullConfig(ConfigData* configs);
bool resetConfig();
void mostrarConfigs(ConfigData* configs);

int cmpCampoTxt(const void* camp, const void* txt);
void impCampo(const void* campo);

#endif // CONFIGS_H_INCLUDED
