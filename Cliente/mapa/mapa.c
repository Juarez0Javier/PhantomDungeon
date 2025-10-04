#include "mapa.h"

bool crearMapa(Mapa* mapa, int filas, int cols) {
    mapa->data = (char**)crearMatriz(filas, cols, sizeof(char));
    if (!mapa->data)
        return false;
    mapa -> entidades = (Entidad***) crearMatriz(filas, cols, sizeof(Entidad*));
    if (!mapa -> entidades)
        return false;

    mapa->filas = filas;
    mapa->cols = cols;
    return true;
}

void vaciarMapa(Mapa* mapa) {
    for (int i=0; i<mapa->filas; i++) {
        for (int j=0; j<mapa->cols; j++) {
            mapa->data[i][j] = '.';
        }
    }
}

void mostrarMapa (Mapa* mapa) {
    int i, j;

    for (i = 0; i < mapa -> filas; i++) {
        for (j = 0; j < mapa -> cols; j++) {

            if (mapa -> entidades[i][j])
                printf("%c ", (mapa -> entidades[i][j]) -> tipo);
            else
                printf("%c ", mapa -> data[i][j]);
        }
        printf("\n");
    }
}

// Esta funcion se adapta a cualquier mapa cargado, reservando ella misma el espacio para mapa segun lo necesario
int cargarMapaDeArchivoNoSeguro(Mapa* mapa, char* rutaArch, Entidad* jugador, Vector* entidades) {
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

// Esta funcion carga el mapa siempre y cuando cumpla con las dimensiones precargadas
bool cargarMapaDeArchivo (Mapa* mapa, Entidad* jugador, Vector* fantasmas, const char* nomArch) {

    FILE* archTxt = fopen(nomArch, "rt");
    Entidad fantasma, *pf;
    VectorIterador it;
    int i = 0, j;
    char casilla;

    if (!archTxt)
        return false;

    while ((casilla = fgetc(archTxt)) != EOF) {

        j = 0;

        while (casilla != '\n') {

            mapa -> entidades[i][j] = NULL;

            switch (casilla) {
                case ENTRADA:
                    jugador -> x = j;
                    jugador -> y = i;
                    jugador -> xInicial = j;
                    jugador -> yInicial = i;
                    jugador -> tipo = JUGADOR;
                    jugador -> eliminado = false;

                    mapa -> entidades[i][j] = jugador; // cuidado: no cuida que sea indexable
                    break;

                case FANTASMA:
                    fantasma.x = j;
                    fantasma.y = i;
                    fantasma.xInicial = j;
                    fantasma.yInicial = i;
                    fantasma.tipo = FANTASMA;
                    fantasma.eliminado = false;

                    if (vectorInsertarAlFinal(fantasmas, &fantasma) != OK)
                        printf("Error al insertar fantasma, memoria insuficiente");

                    casilla = CAMINO;
                    break;
            }

            mapa -> data[i][j] = casilla;
            casilla = fgetc(archTxt);
            j++;
        }

        i++;
    }

    fclose(archTxt);

    // Carga al mapa de entidades los fantasmas.
    vectorIteradorCrear(&it, fantasmas);

    pf = (Entidad*) vectorIteradorPrimero(&it);

    while (pf) {
        mapa -> entidades[pf -> y][pf -> x] = pf;
        pf = (Entidad*) vectorIteradorSiguiente(&it);
    }

    return true;
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

void reestablecerPosInicial (Entidad* e, Mapa* mapa) {
    mapa -> entidades[e -> y][e -> x] = NULL;
    mapa -> entidades[e -> yInicial][e -> xInicial] = e;
    e -> x = e -> xInicial;
    e -> y = e -> yInicial;
}



// es fea, lo se, prometo optimizarla despues
void actualizarMapaRender(SDL_Renderer* renderer, Mapa* mapa, GHP_TexturesData* tex, GHP_Mesh mesh) {
    GHP_Texture pared = tex->textures[0];
    GHP_Texture espacio = tex->textures[1];
    GHP_Texture jugador = tex->textures[2];
    GHP_Texture fantasma = tex->textures[3];
    GHP_Texture premio = tex->textures[4];
    GHP_Texture entrada = tex->textures[5];
    GHP_Texture salida = tex->textures[6];
    GHP_Texture vida = tex->textures[7];

    for (int i=0; i<mapa->filas; i++) {
        for (int j=0; j<mapa->cols; j++) {
            if (mapa->entidades[i][j]) {
                switch (mapa->entidades[i][j]->tipo) {
                    case 'F':
                        GHP_renderTexture(renderer, &fantasma, j*fantasma.width+mesh.offsetX, i*fantasma.height+mesh.offsetY);
                        break;
                    case 'J':
                        GHP_renderTexture(renderer, &jugador, j*jugador.width+mesh.offsetX, i*jugador.height+mesh.offsetY);
                        break;
                }
            }
            else {
                switch (mapa->data[i][j]) {
                    case '.':
                        GHP_renderTexture(renderer, &espacio, j*espacio.width+mesh.offsetX, i*espacio.height+mesh.offsetY);
                        break;
                    case '#':
                        GHP_renderTexture(renderer, &pared, j*pared.width+mesh.offsetX, i*pared.height+mesh.offsetY);
                        break;
                    case 'E':
                        GHP_renderTexture(renderer, &entrada, j*entrada.width+mesh.offsetX, i*entrada.height+mesh.offsetY);
                        break;
                    case 'S':
                        GHP_renderTexture(renderer, &salida, j*salida.width+mesh.offsetX, i*salida.height+mesh.offsetY);
                        break;
                    case 'P':
                        GHP_renderTexture(renderer, &premio, j*premio.width+mesh.offsetX, i*premio.height+mesh.offsetY);
                        break;
                    case 'V':
                        GHP_renderTexture(renderer, &vida, j*vida.width+mesh.offsetX, i*vida.height+mesh.offsetY);
                        break;
                }
            }
        }
    }
}

