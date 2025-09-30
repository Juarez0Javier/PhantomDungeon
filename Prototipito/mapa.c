#include "mapa.h"

int crearMapa(Mapa* mapa, int filas, int cols) {
    mapa->data = (char**)newDinMtx(filas, cols, sizeof(char));
    if (!mapa->data)
        return MEM_ERR;
    mapa->filas = filas;
    mapa->cols = cols;
    return OK;
}

void vaciarMapa(Mapa* mapa) {
    for (int i=0; i<mapa->filas; i++) {
        for (int j=0; j<mapa->cols; j++) {
            mapa->data[i][j] = '.';
        }
    }
}

void mostrarMapa(Mapa* mapa, Entidad* jugador, MapaEntidades* mapaEntidades) {
    printf("\nMAPA:\n");
    for (int i=0; i<mapa->filas; i++) {
        for (int j=0; j<mapa->cols; j++) {

            printf("|%c", (i==jugador->y && j==jugador->x) ? 'J' :
                   mapaEntidades->data[i][j].tipo != '\0' ? mapaEntidades->data[i][j].tipo : mapa->data[i][j]);

        }
        printf("|\n");
    }
}

int cargarMapaDeArchivo(Mapa* mapa, char* rutaArch, Entidad* jugador, Vector* entidades) {
    FILE* arch = fopen(rutaArch, "rt");
    if (!arch)
        return FILE_ERR;

    char bufferLinea[BUF_LINEA_ARCH];

    // reserving rows vec
    mapa->data = (char**)malloc(sizeof(char*) * MAX_CANT_FILAS);
    if (!mapa->data)
        return MEM_ERR;

    mapa->cols = NO_ESTABLECIDO;

    int i = 0;
    while (fgets(bufferLinea, BUF_LINEA_ARCH, arch)) {

        if (cambioSaltoLineaPorFinCadena(bufferLinea) != OK)
            return FILE_ERR;

        if (strlen(bufferLinea) != mapa->cols && mapa->cols != NO_ESTABLECIDO)
            return FILE_ERR;
        else if (mapa->cols == NO_ESTABLECIDO)
            mapa->cols = strlen(bufferLinea);


        // reserving col vec
        mapa->data[i] = (char*)malloc(sizeof(char) * mapa->cols);
        if (!mapa->data[i])
            return MEM_ERR;

        for (int j=0; j<mapa->cols; j++) {
            if (bufferLinea[j] == '#' || bufferLinea[j] == 'E' || bufferLinea[j] == 'S') {
                mapa->data[i][j] = bufferLinea[j];
                if (bufferLinea[j] == 'E') {
                    jugador->x = j;
                    jugador->y = i;
                }
            }
            else {
                Entidad nuevoFantasma;
                switch (bufferLinea[j]) {
                    case 'F':
                        //Entidad nuevoFantasma = {entidades->tam, 'F', j, i}; raises error idk
                        nuevoFantasma.num = entidades->tam;
                        nuevoFantasma.tipo = 'F';
                        nuevoFantasma.x = j;
                        nuevoFantasma.y = i;
                        vectorInsertarAlFinal(entidades, &nuevoFantasma);
                        break;
                }

                mapa->data[i][j] = '.';
            }
        }

        i++;
    }

    mapa->filas = i;
    mapa->data = realloc(mapa->data, sizeof(char*) * mapa->filas);

    return OK;
}

int cambioSaltoLineaPorFinCadena(char* cadena) {
    char* pSaltoLinea = strrchr(cadena, '\n');
    if (!pSaltoLinea)
        return GRAL_ERR;
    *pSaltoLinea = '\0';
    return OK;
}

int moverEntidad(Entidad* entidad, Mapa* mapa, char direccion) { // it is not case sensitive (no need?)
    int difX= direccion=='d' ? 1 : direccion=='a' ? -1 : 0;
    int difY= direccion=='s' ? 1 : direccion=='w' ? -1 : 0;
    if (
        entidad->y+difY == mapa->filas || entidad->y+difY == -1 ||
        entidad->x+difX == mapa->cols || entidad->x+difX == -1 ||
        mapa->data[entidad->y+difY][entidad->x+difX] != '.'
    )
        return MOV_IMPOSIBLE;
    entidad->x += difX;
    entidad->y += difY;
    return OK;
}

char colisionConMapa(Entidad* entidad, Mapa* mapa, char direccion) {
    int difX= direccion=='d' ? 1 : direccion=='a' ? -1 : 0;
    int difY= direccion=='s' ? 1 : direccion=='w' ? -1 : 0;

    if (
        entidad->y+difY == mapa->filas || entidad->y+difY == -1 ||
        entidad->x+difX == mapa->cols || entidad->x+difX == -1
    )
        return '\0';

    return mapa->data[entidad->y+difY][entidad->x+difX];
}

Entidad* colisionConEntidad(Entidad* entidad, MapaEntidades* mapaEntidades, char direccion) {
    int difX= direccion=='d' ? 1 : direccion=='a' ? -1 : 0;
    int difY= direccion=='s' ? 1 : direccion=='w' ? -1 : 0;
    if (
        entidad->y+difY == mapaEntidades->filas || entidad->y+difY == -1 || // TODO: Replace this shit with a func or macro "fueraDeMapa"
        entidad->x+difX == mapaEntidades->cols || entidad->x+difX == -1 ||
        mapaEntidades->data[entidad->y+difY][entidad->x+difX].tipo == '\0'
    )
        return NULL;
    return &mapaEntidades->data[entidad->y+difY][entidad->x+difX];
}

int crearMapaEntidadesVacio(MapaEntidades* mapaEntidades, int filas, int cols) {
    mapaEntidades->data = (Entidad**)newDinMtx(filas, cols, sizeof(Entidad));
    if (!mapaEntidades->data)
        return MEM_ERR;
    mapaEntidades->filas = filas;
    mapaEntidades->cols = cols;
    for (int i=0; i<mapaEntidades->filas; i++) {
        for (int j=0; j<mapaEntidades->cols; j++) {
            Entidad entNull = entidadNula();
            memcpy(&mapaEntidades->data[i][j], &entNull, sizeof(Entidad));
        }
    }
    return OK;
}

void llenarMapaConEntidades(MapaEntidades* mapaEntidades, Vector* entidades) { // TODO Optimize this bullshit
    for (int i=0; i<entidades->tam; i++) {
        Entidad ent;
        copiarEntidad(&ent, (Entidad*)entidades->vec + i);
        copiarEntidad(&mapaEntidades->data[ent.y][ent.x], &ent);
    }
}

void mostrarMapaEntidades(MapaEntidades* mapaEntidades) {
    printf("\nMAPA ENTIDADES:\n");
    for (int i=0; i<mapaEntidades->filas; i++) {
        for (int j=0; j<mapaEntidades->cols; j++) {
            printf("|");
            imprimirEntidadMini(mapaEntidades->data[i][j]);
        }
        printf("|\n");
    }
}
