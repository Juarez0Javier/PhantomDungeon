#include "mapaRandom.h"

int generarMapaRandom(ConfigData* configData, Partida* partida, char nombreArch[]) // Generacion por busqueda en profundidad y retorno
{
    FILE* archTxt;
    int filas = configData->filas, columnas = configData->columnas;
    int fantasmas = configData->maximo_numero_fantasmas,
        premios = configData->maximo_numero_premios,
        vidas = configData->maximo_vidas_extra;

    char **mapa = partida->mapa.data;
    size_t semilla;
    tVector2 pos, dirMov, posE;

    archTxt = fopen(nombreArch, "wt");
    if (!archTxt)
        return ERR_ARCHIVO;

    semilla = time(NULL);
    srand(semilla);
    partida->mapa.semilla = semilla;

    //Inicializando Matriz en Paredes
    for(int i = 0; i<filas; i++)
        for(int j = 0; j<columnas; j++)
            mapa[i][j] = '#';


    generarEntradaYPrimerEspacio(&pos, &dirMov, &posE, filas, columnas, mapa);

    generarCaminosSimples(pos, dirMov, posE, filas, columnas, mapa);

    cortarParedesParaMasCaminos(filas, columnas, mapa);

    insertarEntidades(posE, fantasmas, premios, vidas, filas, columnas, mapa, partida);

    printMapaOnTxt(columnas,filas,mapa,&archTxt);

    partida->jugador.x = partida->jugador.xInicial = posE.x;
    partida->jugador.y = partida->jugador.yInicial = posE.y;
    partida->mapa.entidades[posE.x][posE.y] = &partida->jugador;

    fclose(archTxt);
    return OK;
}

tVector2 dirRandom (int cardBloq[4])
{
    int sel, vecSel[4] = {0,0,0,0};
    int vecTam = 0;

    //printf("Proc\n");

    tVector2 ret;

    for(int i = 0; i < 4; i++)
    {
        if(cardBloq[i]!= 1)
        {
            vecSel[vecTam] = i + 1;
            vecTam++;
        }
    }

    sel = vecSel[rand() % vecTam];

    ret.x = cardaVector2(sel).x;
    ret.y = cardaVector2(sel).y;

    //printf("X: %d, Y: %d\n",ret.x,ret.y);

    return ret;
}

tVector2 posCaminoRandom (int columnas, int filas, char** mapa)
{
    // es enrevezada, pero garantiza llegar siempre a alguna posicion (O²)
    // (agarrando posiciones random y si no son camino volviendo a randomizar
    // no garantiza un tiempo minimo)
    tVector2 *posDisp, ret = {-1,-1};
    int cantCam = 0, sel;

    posDisp = (tVector2*)malloc(sizeof(tVector2) * filas * columnas);

    if (!posDisp)
        return ret;

    for(int i = 0; i<filas; i++) {
        for(int j = 0; j<columnas; j++) {
            if (mapa[i][j] == '.') {
                posDisp[cantCam].x = j;
                posDisp[cantCam].y = i;
                cantCam++;
            }
        }
    }

    if (cantCam == 0)
        return ret;

    sel = rand() % cantCam;
    ret.x = posDisp[sel].x;
    ret.y = posDisp[sel].y;

    free(posDisp);
    return ret;
}

char itemRandom (int* fantasmas, int* premios, int* vidas)
{
    size_t iter=0;

    if (*fantasmas + *premios + *vidas <= 0)
        return '\0';

    while (iter<MAX_ITER_RAND) {
        int tipoItem = rand() % 3;
        switch (tipoItem) {
            case 1:
                if (*fantasmas) {
                    (*fantasmas)--;
                    return 'F';
                }
                break;
            case 2:
                if (*premios) {
                    (*premios)--;
                    return 'P';
                }
            case 3:
                if (*vidas) {
                    (*vidas)--;
                    return 'V';
                }
        }
        iter++;
    }

    return '\0';
}


void printMapa(int columnas, int filas, char** mapa)
{
    printf("------------------------\n");

    for(int i = 0; i<filas; i++)
    {
        for(int j = 0; j<columnas; j++)
        {
            printf("%c",mapa[i][j]);
        }
        printf("\n");
    }
}

void printMapaOnTxt(int columnas, int filas, char** mapa, FILE** file)
{

    for(int i = 0; i<filas; i++)
    {
        for(int j = 0; j<columnas; j++)
        {
            fprintf((*file),"%c",mapa[i][j]);

        }
        fprintf((*file),"\n");
    }
}

int vector2ACard(tVector2 vec)
{
    if(vec.y == -1)
        return 1;
    if(vec.x == 1)
        return 2;
    if(vec.y == 1)
        return 3;
    if(vec.x == -1)
        return 4;
    return 0;
}

tVector2 cardaVector2(int card)
{
    tVector2 res;

    res.x = 0;
    res.y = 0;

    if(card == 1)
        res.y = -1;
    if(card == 2)
        res.x = 1;
    if(card == 3)
        res.y = 1;
    if(card == 4)
        res.x = -1;

    return res;
}

void generarEntradaYPrimerEspacio(tVector2* pos, tVector2* dirMov, tVector2* posE, int filas, int cols, char** mapa) {
    dirMov->y = 0;
    dirMov->x = 0;

    pos->x = 1 + (rand() % (filas - 2));
    pos->y = 1 + (rand() % (cols - 2));

    switch ((rand() % 4))
    {
        case 0:
            pos->x = 0;
            dirMov->x = 1;
            break;
        case 1:
            pos->x = cols - 1;
            dirMov->x = -1;
            break;
        case 2:
            pos->y = 0;
            dirMov->y = 1;
            break;
        case 3:
            pos->y = filas - 1;
            dirMov->y = -1;
            break;
    }

    posE->x = pos->x;
    posE->y = pos->y;

    mapa[pos->y][pos->x] = 'E';

    pos->x += dirMov->x;
    pos->y += dirMov->y;

    mapa[pos->y][pos->x] = '.';
}

void chequearPosiblesMovimientos(int cardBloq[4], tVector2 pos, int filas, int columnas, char** mapa) {
    cardBloq[0] = 0;
    cardBloq[1] = 0;
    cardBloq[2] = 0;
    cardBloq[3] = 0;

    if(
       pos.y - 1 == 0 ||
       mapa[pos.y - 1][pos.x] == '.' ||
       mapa[pos.y - 2][pos.x] == '.' ||
       mapa[pos.y - 1][pos.x + 1] == '.' ||
       mapa[pos.y - 1][pos.x - 1] == '.'
       )
        cardBloq[0] = 1;

    if(
       pos.x + 1 == filas - 1 ||
       mapa[pos.y][pos.x + 1] == '.' ||
       mapa[pos.y][pos.x + 2] == '.' ||
       mapa[pos.y - 1][pos.x + 1] == '.' ||
       mapa[pos.y + 1][pos.x + 1] == '.'
       )
        cardBloq[1] = 1;

    if(
       pos.y + 1 == columnas - 1 ||
       mapa[pos.y + 1][pos.x] == '.' ||
       mapa[pos.y + 2][pos.x] == '.' ||
       mapa[pos.y + 1][pos.x + 1] == '.' ||
       mapa[pos.y + 1][pos.x - 1] == '.'
       )
        cardBloq[2] = 1;

    if(
       pos.x - 1 == 0 ||
       mapa[pos.y][pos.x - 1] == '.'  ||
       mapa[pos.y][pos.x - 2] == '.'  ||
       mapa[pos.y - 1][pos.x - 1] == '.'  ||
       mapa[pos.y + 1][pos.x - 1] == '.'
       )
        cardBloq[3] = 1;
}

bool ningunMovimientoPosible(int cardBloq[4]) {
    return cardBloq[0] == 1 && cardBloq[1] == 1 && cardBloq[2] == 1 && cardBloq[3] == 1;
}

void posicionarSalidaCandidata(tVector2* posS, tVector2 pos, tVector2 posE, int filas, int columnas) {
    if(pos.y == 1 && posE.y == columnas - 1)
    {
        posS->y = pos.y - 1;
        posS->x = pos.x;
    }
    if(pos.y == filas - 2 && posE.y == 0)
    {
        posS->y = pos.y + 1;
        posS->x = pos.x;
    }
    if(pos.x == 1 && posE.x == filas - 1)
    {
        posS->y = pos.y;
        posS->x = pos.x - 1;
    }

    if(pos.x == columnas - 2 && posE.x == 0)
    {
        posS->y = pos.y;
        posS->x = pos.x + 1;
    }
}

void taparRadioRespectoAPosEnMascara(tVector2 pos, int rango, int filas, int cols, char** mapaMascara) {
    for(int i = pos.y - rango; i<pos.y + rango + 1; i++)
        for(int j = pos.x - rango; j<pos.x + rango + 1; j++)
            if(i > 0 && i < cols && j > 0 && j < filas)
                mapaMascara[i][j] = '#';
}

void generarCaminosSimples(tVector2 pos, tVector2 dirMov, tVector2 posE, int filas, int cols, char** mapa) {
    tPilaD stack;
    tVector2 posS;
    int cardBloq[4];

    crearPilaD(&stack);

    ///Hasta que recorra cada celda del mapa
    do
    {
        /*
        Para ver como se crea paso a paso
        clearConsole();
        mostrarMapa(&partida->mapa);
        Sleep(100);
        */

        apilarD(&stack,&pos,sizeof(tVector2));

        dirMov.x = dirMov.y = 0;

        do
        {
            chequearPosiblesMovimientos(cardBloq, pos, filas, cols, mapa);
            if(ningunMovimientoPosible(cardBloq))
                desapilarD(&stack,&pos,sizeof(tVector2));
        }
        while(ningunMovimientoPosible(cardBloq) && !pilaDVacia(&stack));

        if(!ningunMovimientoPosible(cardBloq))
            dirMov = dirRandom(cardBloq);

        // aplica el movimiento y pone el camino
        pos.x += dirMov.x;
        pos.y += dirMov.y;
        mapa[pos.y][pos.x] = '.';

        posicionarSalidaCandidata(&posS, pos, posE, filas, cols);
        // cada vez que se acerca a un borde opuesto al de la entrada se fija si
        // podria ser la salida, caso positivo reescribe la ultima salida candidata
    }
    while (!pilaDVacia(&stack));

    mapa[posS.y][posS.x] = 'S';

}

int insertarEntidades(tVector2 posE, int fantasmas, int premios, int vidas, int filas, int cols, char** mapa, Partida* partida) {
    int rangoSpawn = 2, rangoSpawnE = 3;

    char **mapaEleccion = (char**)crearMatriz(filas, cols, sizeof(char)); // Mascara para rangos de spawn
    if (!mapaEleccion)
        return SIN_MEM;

    copiarMatriz((void**)mapaEleccion, (void**)mapa, filas, cols, sizeof(char));
        // vamos a usar esta mascara para evitar que las entidades se generen muy cerca
    taparRadioRespectoAPosEnMascara(posE, rangoSpawnE, filas, cols, mapaEleccion);

    while (fantasmas + premios + vidas)
    {
        tVector2 pos = posCaminoRandom(cols,filas,mapaEleccion);
        if (pos.x == -1) {
            destruirMatriz(filas, (void**)mapaEleccion);
            return ERR_MAPA;
        }

        char item = itemRandom(&fantasmas,&premios,&vidas);
        if (!item) {
            destruirMatriz(filas, (void**)mapaEleccion);
            return ERR_MAPA;
        }

        mapa[pos.y][pos.x] = item;

        if (item == 'F') { // TODO revisar
            Entidad nuevoFantasma = {'F', partida->fantasmas.tam + 1, pos.x, pos.y, pos.x, pos.y, TICKS_ENTRE_MOVS_FANTASMA_NORMAL, SDL_GetTicks(), false};
            vectorInsertarAlFinal(&partida->fantasmas, &nuevoFantasma);
            //partida->mapa.entidades[pos.x][pos.y] = vectorPunteroAlUltimoElem(&partida->fantasmas);
        }

        taparRadioRespectoAPosEnMascara(pos, rangoSpawn, filas, cols, mapaEleccion);
    }
    destruirMatriz(filas, (void**)mapaEleccion);
    return OK;
}

void cortarParedesParaMasCaminos(int filas, int cols, char** mapa) {
    for (int i=1; i<filas-2; i++)
        for (int j=1; j<cols-2; j++)
            if (
                ((mapa[i-1][j] == '.' && mapa[i+1][j] == '.') || (mapa[i][j-1] == '.' && mapa[i][j+1] == '.')) // que libere camino
                && !(mapa[i-1][j-1]=='.' && mapa[i+1][j-1] == '.' && mapa[i+1][j-1]=='.' && mapa[i+1][j+1]=='.') // que no deje zona 3x3 libre
                && rand()%100<=PORCENTAJE_CORTE_PAREDES // no se cortan todas las paredes
                )
                mapa[i][j] = '.';
}

