#include "mapaRandom.h"

int generarMapaRandom(ConfigData* configData, Partida* partida, char nombreArch[])   // Generacion por busqueda en profundidad y retorno
{
    FILE* archTxt;
    int filas = configData->filas, columnas = configData->columnas;
    int fantasmas = configData->maximo_numero_fantasmas,
        premios = configData->maximo_numero_premios,
        vidas = configData->maximo_vidas_extra;

    char **mapa = partida->mapa.data;
    size_t semilla;
    tPos pos, dirMov, posE, posS;

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

    // Vacia vector y matriz de fantasmas
    vectorVaciar(&partida->fantasmas);
    for (int i=0; i<filas; i++)
        for (int j=0; j<columnas; j++)
            partida->mapa.entidades[i][j] = NULL;

    generarEntradaYPrimerEspacio(&pos, &dirMov, &posE, filas, columnas, mapa);

    generarCaminosSimples(pos, dirMov, posE, &posS, filas, columnas, mapa);

    generarGaleria(3,3,posE,filas,columnas,mapa);

    cortarParedesParaMasCaminos(filas, columnas, mapa);

    //bifurcacionesCaminoPrincial(filas,columnas,posE,posS,mapa);

    insertarEntidadesEnMapa(posE, fantasmas, premios, vidas, filas, columnas, mapa, partida);

    printMapaEnTxt(columnas,filas,mapa,&archTxt);

    //printMapa(columnas,filas,mapa);

    //exit(0);

    insertarFantasmasEnMapaEntidades(filas, columnas, mapa, &partida->fantasmas, partida->mapa.entidades);

    crearEntidad(&partida->jugador, posE.x, posE.y, JUGADOR, COOLDOWN_MOV_JUGADOR);
    partida->mapa.entidades[posE.y][posE.x] = &partida->jugador;

    mostrarMapa(&partida->mapa);

    fclose(archTxt);
    return OK;
}

void generarEntradaYPrimerEspacio(tPos* pos, tPos* dirMov, tPos* posE, int filas, int cols, char** mapa)
{
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

void generarCaminosSimples(tPos pos, tPos dirMov, tPos posE, tPos* posS, int filas, int cols, char** mapa)
{
    tPilaD stack;
    tPos posSCan;
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

        apilarD(&stack,&pos,sizeof(tPos));

        dirMov.x = dirMov.y = 0;

        do
        {
            chequearPosiblesMovimientos(cardBloq, pos, filas, cols, mapa);
            if(ningunMovimientoPosible(cardBloq))
                desapilarD(&stack,&pos,sizeof(tPos));
        }
        while(ningunMovimientoPosible(cardBloq) && !pilaDVacia(&stack));

        if(!ningunMovimientoPosible(cardBloq))
            dirMov = dirRandom(cardBloq);

        // aplica el movimiento y pone el camino
        pos.x += dirMov.x;
        pos.y += dirMov.y;
        mapa[pos.y][pos.x] = '.';

        posicionarSalidaCandidata(&posSCan, pos, posE, filas, cols);

        // cada vez que se acerca a un borde opuesto al de la entrada se fija si
        // podria ser la salida, caso positivo reescribe la ultima salida candidata
    }
    while (!pilaDVacia(&stack));

    posS->x = posSCan.x;
    posS->y = posSCan.y;

    mapa[posSCan.y][posSCan.x] = 'S';

}

void chequearPosiblesMovimientos(int cardBloq[4], tPos pos, int filas, int columnas, char** mapa)
{
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

bool ningunMovimientoPosible(int cardBloq[4])
{
    return cardBloq[0] == 1 && cardBloq[1] == 1 && cardBloq[2] == 1 && cardBloq[3] == 1;
}

tPos dirRandom (int cardBloq[4])
{
    int sel, vecSel[4] = {0,0,0,0};
    int vecTam = 0;

    //printf("Proc\n");

    tPos ret;

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

void posicionarSalidaCandidata(tPos* posS, tPos pos, tPos posE, int filas, int columnas)
{
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

void taparRadioRespectoAPosEnMascara(tPos pos, int rango, int filas, int cols, char** mapaMascara)
{
    for(int i = pos.y - rango; i<pos.y + rango + 1; i++)
        for(int j = pos.x - rango; j<pos.x + rango + 1; j++)
            if(i > 0 && i < cols && j > 0 && j < filas)
                mapaMascara[i][j] = '#';
}

void generarGaleria (int galLong, int galAlt, tPos posE, int filas, int cols, char** mapa)
{
    int limSupY, limInfY, limSupX, limInfX, ajusteY = 0, ajusteX = 0;

    limInfX = posE.x - (galLong - 1)/2;
    limSupX = limInfX + (galLong - 1);

    limInfY = posE.y - (galAlt - 1)/2;
    limSupY = limInfY + (galAlt - 1);

    if (limInfX < 1)
        ajusteX = abs(1 - limInfX);
    if (limSupX > cols - 2)
        ajusteX = cols - 2 - limSupX;

    if (limInfY < 1)
        ajusteY = abs(1 - limInfY);
    if (limSupY > filas - 2)
        ajusteY = filas - 2 - limSupY;

    //printf("X Sup: %d - X Inf: %d - Ajuste X: %d\n",limSupX,limInfX,ajusteX);
    //printf("Y Sup: %d - Y Inf: %d - Ajuste Y: %d\n",limSupY,limInfY,ajusteY);

    limInfX += ajusteX;
    limSupX += ajusteX;

    limInfY += ajusteY;
    limSupY += ajusteY;

    for(int i = limInfY; i<= limSupY; i++)
    {
        for(int j = limInfX; j<= limSupX; j++)
        {
            mapa[i][j] = '.';
        }
    }

    return;
}

void bifurcacionesCaminoPrincial (int filas, int cols, tPos posE, tPos posS, char** mapa)
{
    //Algoritmo BFS
    // Reinicia la matriz de visita (todos como no visitados)

    tCola colaBFS;

    int ** mat_visitado = (int**)crearMatriz(filas, cols, sizeof(int));
    int ** mat_padreX  = (int**)crearMatriz(filas, cols, sizeof(int));
    int ** mat_padreY  = (int**)crearMatriz(filas, cols, sizeof(int));

    int y, x, d;

    int dx[8] = { 0, 0, 1, -1};
    int dy[8] = {-1, 1, 0, 0};

    int metaX, metaY;
    bool encontrado;

    tPos m_actual;
    tPos m_nueva;
    int nx, ny;

    int r_actualX, r_actualY;
    int anteriorX, anteriorY;
    bool retrocediendo;

    crearCola(&colaBFS);

    y = 0;
    while (y < filas)
    {
        x = 0;
        while (x < cols)
        {
            ((int*)mat_visitado[y])[x] = false;
            x++;
        }
        y++;
    }

    //Obteniendo el primer casillero de E
    if (posE.x == 0)
        posE.x++;
    if (posE.x == cols - 1)
        posE.x--;
    if (posE.y == 0)
        posE.y++;
    if (posE.y == filas -1)
        posE.y--;

    //Obteniendo el primer casillero de S

    if (posS.x == 0)
        posS.x++;
    if (posS.x == cols - 1)
        posS.x--;
    if (posS.y == 0)
        posS.y++;
    if (posS.y == filas - 1)
        posS.y--;

    printf("E en X: %d - E en Y: %d \n",posE.x,posE.y);
    printf("S en X: %d - S en Y: %d \n",posS.x,posS.y);

    m_actual.x = posE.x;
    m_actual.y = posE.x;

    ponerEnCola(&colaBFS, &m_actual, sizeof(tPos));

    ((int*)mat_visitado[posE.y])[posE.x] = true;
    ((int*)mat_padreX[posE.y])[posE.x] = posE.x;
    ((int*)mat_padreY[posE.y])[posE.x] = posE.y;

    metaX = posS.x;
    metaY = posS.y;
    encontrado = false;


    //BFS: contin�a mientras haya nodos en la cola y no se haya encontrado la meta.
    while (!colaVacia(&colaBFS) && !encontrado)
    {

        if (sacarDeCola(&colaBFS, &m_actual, sizeof(tPos)))
        {
            d = 0;
            while (d < 4)   // 4 direcciones
            {

                nx = m_actual.x + dx[d];
                ny = m_actual.y + dy[d];
                if (nx >= 0 && nx < cols && ny >= 0 && ny < filas)
                {

                    if ((((int*)mat_visitado[ny])[nx] == false) && (mapa[ny][nx] !='#') && !encontrado)
                    {

                        // Marca como visitado, guarda padre y encola
                        ((int*)mat_visitado[ny])[nx] = true;
                        ((int*)mat_padreX[ny])[nx] = m_actual.x;
                        ((int*)mat_padreY[ny])[nx] = m_actual.y;

                        m_nueva.x = nx;
                        m_nueva.y = ny;
                        ponerEnCola(&colaBFS, &m_nueva, sizeof(tPos));

                        // Si llega al jugador
                        if (nx == metaX && ny == metaY)
                        {
                            encontrado = true;
                        }
                    }
                }
                d++;
            }
        }
    }

    //Reconstrucci�n del camino
    if (encontrado)
    {

        r_actualX = metaX;
        r_actualY = metaY;

        retrocediendo = true;
        anteriorX = posE.x;
        anteriorY = posE.y;

        // El bucle busca el nodo 'anteriorX, anteriorY'
        while (retrocediendo)
        {/*
            bool posibleCorteHorizontal, posibleCorteVertical;

            posibleCorteHorizontal = (
                                         mapa[r_actualY-1][ r_actualX ] == '#' &&
                                         mapa[r_actualY][r_actualX-1] == '.' && mapa[ r_actualY ][ r_actualX ] == '#' && mapa[ r_actualY ][r_actualX+1] == '.' &&
                                         mapa[r_actualY+1][ r_actualX ] == '#'
                                     );

            posibleCorteVertical = (
                                       mapa[r_actualY-1][ r_actualX ] == '.' &&
                                       mapa[r_actualY][r_actualX-1] == '#' && mapa[ r_actualY ][ r_actualX ] == '#' && mapa[ r_actualY ][r_actualX+1] == '#' &&
                                       mapa[r_actualY+1][r_actualX ] == '.'
                                   );

            if ( (posibleCorteHorizontal || posibleCorteVertical)
                    && rand()%100<=PORCENTAJE_CORTE_PAREDES // porcentaje de cortes personalizable
               )
                mapa[r_actualY][r_actualX] = '.';*/

            printf("X Vistando: %d - Y Visitado: %d \n",r_actualX,r_actualY);

            if (!(r_actualX == posE.x && r_actualY == posE.y))
            {

                anteriorX = r_actualX;
                anteriorY = r_actualY;
                r_actualX = ((int*)mat_padreX[anteriorY])[anteriorX];
                r_actualY = ((int*)mat_padreY[anteriorY])[anteriorX];

                // si el padre al que saltamos es el fantasma
                if (r_actualX == posE.x && r_actualY == posE.y)
                {
                    retrocediendo = false;
                }

            }
            else
            {
                retrocediendo = false;
            }
        }
    }
}

void cortarParedesParaMasCaminos(int filas, int cols, char** mapa)
{
    for (int i=1; i<filas-2; i++)
        for (int j=1; j<cols-2; j++)
        {
            bool posibleCorteHorizontal, posibleCorteVertical;

            posibleCorteHorizontal = (
                                         mapa[i-1][ j ] == '#' &&
                                         mapa[ i ][j-1] == '.' &&
                                         mapa[ i ][ j ] == '#' &&
                                         mapa[ i ][j+1] == '.' &&
                                         mapa[i+1][ j ] == '#'
                                     );

            posibleCorteVertical = (
                                       mapa[i-1][ j ] == '.' &&
                                       mapa[ i ][j-1] == '#' && mapa[ i ][ j ] == '#' && mapa[ i ][j+1] == '#' &&
                                       mapa[i+1][ j ] == '.'
                                   );

            if ( (posibleCorteHorizontal || posibleCorteVertical)
                    && rand()%100<=PORCENTAJE_CORTE_PAREDES // porcentaje de cortes personalizable
               )
                mapa[i][j] = '.';
        }
}

int insertarEntidadesEnMapa(tPos posE, int fantasmas, int premios, int vidas, int filas, int cols, char** mapa, Partida* partida)
{
    int rangoSpawn = 1, rangoSpawnE = 3;

    char **mapaMascara = (char**)crearMatriz(filas, cols, sizeof(char)); // Mascara para rangos de spawn
    if (!mapaMascara)
        return SIN_MEM;

    copiarMatriz((void**)mapaMascara, (void**)mapa, filas, cols, sizeof(char));
    // vamos a usar esta mascara para evitar que las entidades se generen muy cerca
    taparRadioRespectoAPosEnMascara(posE, rangoSpawnE, filas, cols, mapaMascara);

    while (fantasmas + premios + vidas)
    {
        tPos pos = posCaminoRandom(cols,filas,mapaMascara);
        if (pos.x == -1)
        {
            destruirMatriz(filas, (void**)mapaMascara);
            return ERR_MAPA;
        }

        char item = itemRandom(&fantasmas,&premios,&vidas);
        if (!item)
        {
            destruirMatriz(filas, (void**)mapaMascara);
            return ERR_MAPA;
        }

        mapa[pos.y][pos.x] = item;

        taparRadioRespectoAPosEnMascara(pos, rangoSpawn, filas, cols, mapaMascara);
    }
    destruirMatriz(filas, (void**)mapaMascara);
    return OK;
}

tPos posCaminoRandom (int columnas, int filas, char** mapa)
{
    // es enrevezada, pero garantiza llegar siempre a alguna posicion (O^2)
    // (agarrando posiciones random y si no son camino volviendo a randomizar
    // no garantiza un tiempo minimo)
    tPos *posDisp, ret = {-1,-1};
    int cantCam = 0, sel;

    posDisp = (tPos*)malloc(sizeof(tPos) * (filas-2) * (columnas-2));

    if (!posDisp)
        return ret;

    for(int i = 1; i<filas-2; i++)
    {
        for(int j = 1; j<columnas-2; j++)
        {
            if (mapa[i][j] == '.')
            {
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
        return '?';

    while (iter<MAX_ITER_RAND)
    {
        int tipoItem = rand() % 3;
        switch (tipoItem)
        {
        case 1:
            if (*fantasmas)
            {
                (*fantasmas)--;
                return 'F';
            }
            break;
        case 2:
            if (*premios)
            {
                (*premios)--;
                return 'P';
            }
        case 3:
            if (*vidas)
            {
                (*vidas)--;
                return 'V';
            }
        }
        iter++;
    }

    return '?';
}

void insertarFantasmasEnMapaEntidades(int filas, int cols, char** mapa, Vector* fantasmas, Entidad*** mapaEntidades)
{
    for (int i=0; i<filas; i++)
    {
        for (int j=0; j<cols; j++)
        {
            if (mapa[i][j] == 'F')
            {
                Entidad nuevoFantasma;
                crearEntidad(&nuevoFantasma, j, i, 'F', TICKS_ENTRE_MOVS_FANTASMA_NORMAL);
                vectorInsertarAlFinal(fantasmas, &nuevoFantasma);
                mapa[i][j] = '.';
            }
        }
    }

    VectorIterador it;
    vectorIteradorCrear(&it, fantasmas);
    Entidad* pf = (Entidad*) vectorIteradorPrimero(&it);
    while (pf)
    {
        mapaEntidades[pf -> y][pf -> x] = pf;
        pf = (Entidad*) vectorIteradorSiguiente(&it);
    }
}

void printMapa(int columnas, int filas, char** mapa)
{
    printf("------------------------\n");
    for(int i = 0; i<filas; i++)
    {
        for(int j = 0; j<columnas; j++)
            printf("%c",mapa[i][j]);
        printf("\n");
    }
}

void printMapaEnTxt(int columnas, int filas, char** mapa, FILE** file)
{
    for(int i = 0; i<filas; i++)
    {
        for(int j = 0; j<columnas; j++)
            fprintf((*file),"%c",mapa[i][j]);
        fprintf((*file),"\n");
    }
}

tPos cardaVector2(int card)
{
    tPos res = {0,0};

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
