#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#define WIDTH 1366
#define HEIGHT 768

#define MODE_PLAY 0
#define MODE_LOST 1
#define MODE_WIN 2
//#define MODE_MENU
#define MODE_END -1

#define AMMOUNT_ASSETS 0

#define AMMOUNT_TEXTURES 0
#define AMM_TEXT_COL_ASSET 0

#define BUT_START 0
#define AMMOUNT_BUTTONS 1

#define AMMOUNT_TEXTS 0




#define MAX_CANT_FILAS 50
#define BUF_LINEA_ARCH 100 // be careful that this var must limit a possible MAX_CANT_COLS

#define NO_ESTABLECIDO -1
#define OK 0
#define MEM_ERR 1
#define DIM_ERR 2
#define FILE_ERR 3
#define GRAL_ERR 4
#define TEX_ERR 5

#define MOV_IMPOSIBLE 6

#define COOLDOWN_MOV_JUGADOR 300 // aprox ms

#include <stddef.h>
#include <stdbool.h>

#endif // CONSTANTS_H_INCLUDED
