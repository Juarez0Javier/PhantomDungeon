#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

// Libs
#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h> // thx mate
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <SDL.h>
#include "../libs/Comun/comun.h"
#include "../libs/Vector/Vector.h"
#include "../libs/Cola/Cola.h"
#include "../libs/graphics/graphics.h"
#include "../libs/matrizDinamica/dinMtx.h"

// Configs (Algunos pares podrian consolidarse)
#define WIDTH 800
#define HEIGHT 500
#define MIN_FILAS 5
#define MAX_FILAS 20
#define MIN_COLUMNAS 5
#define MAX_COLUMNAS 20
#define MIN_VIDAS_INICIO 1
#define MAX_VIDAS_INICIO 5
#define MIN_NUM_FANTASMAS 1
#define MAX_NUM_FANTASMAS 5
#define MIN_NUM_PREMIOS 0
#define MAX_NUM_PREMIOS 5
#define MIN_VIDAS_EXTRA 0
#define MAX_VIDAS_EXTRA 5
// Constantes globales (check if could be replaced with properly configs)
#define MAX_CANT_FILAS 50
#define BUF_LINEA_ARCH 100 // be careful that this var must limit a possible MAX_CANT_COLS
// Configuracion por defecto
#define CONFIG_FILAS 10
#define CONFIG_COLS 10
#define CONFIG_VIDAS 1
// Configuracion de video
#define FPS 3
#define DELAY_DE_FRAMES 1000/FPS
#define COOLDOWN_MOV_JUGADOR 300 // aprox ms


// Rutas
#define RUTA_LABERINTO_PRESET "./laberintos/laberinto.txt"
#define RUTA_CONFIGS "./config.txt"
#define RUTA_FONDO "src/img/background.png"
#define PATH_ASSET "./src/img/asset.png"

// Secciones de juego
#define SECCION_MENU 0
#define SECCION_PARTIDA 1
#define SECCION_DERROTA 2
#define SECCION_VICTORIA 3
#define SECCION_CONFIGS 4
#define SECCION_SALIR_DIRECTO -1

// Texturas - General
#define AMMOUNT_ASSETS 1
#define AMMOUNT_TEXTURES 8
#define AMM_TEXT_COL_ASSET 8
#define TEX_PARED 0
#define TEX_ESPACIO 1
#define TEX_JUGADOR 2
#define TEX_FANTASMA 3
#define TEX_PREMIO 4
#define TEX_ENTRADA 5
#define TEX_SALIDA 6
#define TEX_VIDA 7
// Texturas - Botones
#define BUT_START 0
#define AMMOUNT_BUTTONS 1
// Texturas - Textos
#define AMMOUNT_TEXTS 0

// Valores de retorno
#define NO_ESTABLECIDO -1
#define OK 0
#define MEM_ERR 1
#define DIM_ERR 2
#define FILE_ERR 3
#define GRAL_ERR 4
#define TEX_ERR 5
#define MOV_IMPOSIBLE 6

// Representacion de objetos
#define JUGADOR 'J'
#define FANTASMA 'F'
#define VIDA 'V'
#define PREMIO 'P'
#define ENTRADA 'E'
#define SALIDA 'S'
#define CAMINO '.'
#define PARED '#'

// Direcciones de movimiento
#define ARRIBA 'w'
#define ABAJO 's'
#define IZQUIERDA 'a'
#define DERECHA 'd'


#endif // CONSTANTS_H_INCLUDED
