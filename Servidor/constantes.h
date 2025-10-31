#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <winsock2.h>
#include "../libs/Comun/comun.h"
#include "../libs/Comun/conexion.h"
#include "../libs/Cola/Cola.h"
#include "../libs/Arbol/Arbol.h"
#include "../libs/Vector/Vector.h"
#include "../libs/Lista/Lista.h"

#define PUERTO 6502
#define BUFFER_TAM 1024

// Rutas de archivos
// #define RUTA_JUGADORES_DAT "../../persistencia/jugadores.dat"
// #define RUTA_JUGADORES_IDX_DAT "../../persistencia/jugadoresIdx.dat"
// #define RUTA_PARTIDAS_DAT "../../persistencia/partidas.dat"
// #define RUTA_RANKINGS_DAT "../../persistencia/rankings.dat"

#define RUTA_JUGADORES_DAT "./persistencia/jugadores.dat"
#define RUTA_JUGADORES_IDX_DAT "./persistencia/jugadoresIdx.dat"
#define RUTA_PARTIDAS_DAT "./persistencia/partidas.dat"
#define RUTA_RANKINGS_DAT "./persistencia/rankings.dat"

#endif // CONSTANTES_H_INCLUDED
