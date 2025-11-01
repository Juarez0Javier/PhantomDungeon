#include "mapaRandom.h"

int generarMapaRandom(ConfigData* configData, char nombreArch[])
{
    ///Temporal. Hasta que se pueda cargar configuracion.
    int filas = 10, columnas = 10;
    int fantasmas = 2, premios = 3, vidas = 2;

    ///Habilitar cuando se pueda cargar la configuracion
    //int filas = configData->filas, columnas = configData->columnas;
    //int fantasmas = configData->maximo_numero_fantasmas, premios = configData->maximo_numero_premios, vidas = configData->maximo_vidas_extra;

    char mapa[filas][columnas], mapaEleccion[filas][columnas], item =' ';
    int cardBloq[4], rangoSpawn = 2, rangoSpawnE = 3;
    tVector2 pos, dir, posS, posE;
    tPilaD stack;

    crearPilaD(&stack);

    FILE* archTxt = fopen(nombreArch, "wt");

    if (!archTxt)
        return 0;

    ///Inicializando Matriz en Paredes

    for(int i = 0; i<filas; i++)
    {
        for(int j = 0; j<columnas; j++)
            mapa[i][j] = '#';
    }


    ///Asignado Entrada y posicion inicial

    dir.y = 0;
    dir.x = 0;

    pos.x = 1 + (rand() % (filas - 2));
    pos.y = 1 + (rand() % (columnas - 2));

    switch ((rand() % 4))
    {
    case 0:
        pos.x = 0;
        dir.x = 1;

        break;

    case 1:
        pos.x = columnas - 1;
        dir.x = -1;

        break;

    case 2:
        pos.y = 0;
        dir.y = 1;

        break;

    case 3:
        pos.y = filas - 1;
        dir.y = -1;

        break;
    }

    posE.x = pos.x;
    posE.y = pos.y;

    mapa[pos.y][pos.x] = 'E';

    pos.x += dir.x;
    pos.y += dir.y;

    mapa[pos.y][pos.x] = '.';

    ///Hasta que recorra cada celda del mapa
    do
    {

        apilarD(&stack,&pos,sizeof(tVector2));

        dir.x = dir.y = 0;

        do
        {

            cardBloq[0] = 0;
            cardBloq[1] = 0;
            cardBloq[2] = 0;
            cardBloq[3] = 0;

            if(mapa[pos.y - 1][pos.x] == '.' || mapa[pos.y - 2][pos.x] == '.' || mapa[pos.y - 1][pos.x + 1] == '.' || mapa[pos.y - 1][pos.x - 1] == '.' || pos.y - 1 == 0)
                cardBloq[0] = 1;

            if(mapa[pos.y][pos.x + 1] == '.' || mapa[pos.y][pos.x + 2] == '.' || mapa[pos.y - 1][pos.x + 1] == '.' || mapa[pos.y + 1][pos.x + 1] == '.' || pos.x + 1 == filas - 1)
                cardBloq[1] = 1;

            if(mapa[pos.y + 1][pos.x] == '.' || mapa[pos.y + 2][pos.x] == '.' || mapa[pos.y + 1][pos.x + 1] == '.' || mapa[pos.y + 1][pos.x - 1] == '.' || pos.y + 1 == columnas - 1)
                cardBloq[2] = 1;

            if(mapa[pos.y][pos.x - 1] == '.'  || mapa[pos.y][pos.x - 2] == '.'  || mapa[pos.y - 1][pos.x - 1] == '.'  || mapa[pos.y + 1][pos.x - 1] == '.'  || pos.x - 1 == 0)
                cardBloq[3] = 1;

            if(cardBloq[0] == 1 && cardBloq[1] == 1 && cardBloq[2] == 1 && cardBloq[3] == 1)
                desapilarD(&stack,&pos,sizeof(tVector2));

        }
        while((cardBloq[0] == 1 && cardBloq[1] == 1 && cardBloq[2] == 1 && cardBloq[3] == 1) && !pilaDVacia(&stack));

        ///Si todas las direcciones no estan bloqueadas, definimos una direccion random para que se mueva
        if(!(cardBloq[0] == 1 && cardBloq[1] == 1 && cardBloq[2] == 1 && cardBloq[3] == 1))
            dir = dirRandom(cardBloq);

        pos.x += dir.x;
        pos.y += dir.y;

        mapa[pos.y][pos.x] = '.';

        ///Resolviendo el posicionamiento de la Salida candidata

        if(pos.y == 1 && posE.y == columnas - 1)
        {
            posS.y = pos.y - 1;
            posS.x = pos.x;
        }
        if(pos.y == filas - 2 && posE.y == 0)
        {
            posS.y = pos.y + 1;
            posS.x = pos.x;
        }
        if(pos.x == 1 && posE.x == filas - 1)
        {
            posS.y = pos.y;
            posS.x = pos.x - 1;
        }

        if(pos.x == columnas - 2 && posE.x == 0)
        {
            posS.y = pos.y;
            posS.x = pos.x + 1;
        }
    }
    while (!pilaDVacia(&stack));

    mapa[posS.y][posS.x] = 'S';

    vaciarPilaD(&stack);



///Añadiendo "bulk" a los caminos (que sean un poquito mas grandes).


    for(int i = 1; i<filas - 1; i++)
    {
        for(int j = 1; j<columnas - 1; j++)
        {
            if(mapa[i][j] == '#')
            {
                if(!(mapa[i+1][j] == '#' && mapa[i-1][j] == '#' && mapa[i][j+1] == '#' && mapa[i][j-1] == '#'))
                {
                    switch((rand() % 3))
                    {
                    case 0:
                        mapa[i][j] = '.';
                        break;
                    }
                }
            }
        }
    }

///Añadiendo Fantasmas, Premios y Vidas


    memcpy(&mapaEleccion,&mapa,sizeof(mapa));


    for(int i = posE.y - rangoSpawnE; i<posE.y + rangoSpawnE + 1; i++)
    {
        for(int j = posE.x - rangoSpawnE; j<posE.x + rangoSpawnE + 1; j++)
        {
            if(i > 0 && i < columnas && j > 0 && j < filas)
                mapaEleccion[i][j] = '#';

        }
    }

    /*
    item = 'P';
    for(int i=0; i<premios; i++)


    item = 'V';
    for(int i=0; i<vidas; i++)
        apilarD(&stack,&item,sizeof(char));

    item = 'F';
    for(int i=0; i<fantasmas; i++)
        apilarD(&stack,&item,sizeof(char));*/


    while (fantasmas + premios + vidas > 0)
    {

        pos = posCaminoRandom(columnas,filas,mapaEleccion);

        if (pos.x == -1)
            break;

        item = itemRandom(&fantasmas,&premios,&vidas);

        mapa[pos.y][pos.x] = item;

        for(int i = pos.y - rangoSpawn; i<pos.y + rangoSpawn + 1; i++)
        {
            for(int j = pos.x - rangoSpawn; j<pos.x + rangoSpawn + 1; j++)
            {
                if(i > 0 && i < columnas && j > 0 && j < filas)
                    mapaEleccion[i][j] = '#';

            }
        }


        //printf("X = %d, Y = %d\n",pos.x,pos.y);
        //printf("Mapa\n");
        //printMapa(columnas,filas,mapa);
        //printf("Mapa Eleccion\n");
        //printMapa(columnas,filas,mapaEleccion);

    }

    vaciarPilaD(&stack);

    ///Imprimoendo el Mapa en el archivo de texto

    printMapaOnTxt(columnas,filas,mapa,&archTxt);

    fclose(archTxt);

    printMapa(columnas,filas,mapa);

    //exit(0);

    return 1;
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

tVector2 posCaminoRandom (int columnas, int filas, char mapa[filas][columnas])
{
    tVector2 posDisp [(columnas * filas)], ret;
    int cantCam = 0, sel;

    for(int i = 0; i<filas; i++)
    {
        for(int j = 0; j<columnas; j++)
        {
            if (mapa[i][j] == '.')
            {
                posDisp[cantCam].x = j;
                posDisp[cantCam].y = i;
                cantCam++;
            }
        }
    }

    /*printf("------------------------\n");

    for(int i = 0; i<cantCam; i++)
    {
        printf("X: %d, Y: %d\n",posDisp[i].x,posDisp[i].y);
    }*/

    if (cantCam == 0)
    {
        ret.x = -1;
    }
    else
    {
        sel = rand() % cantCam;

        ret.x = posDisp[sel].x;
        ret.y = posDisp[sel].y;
    }

    //printf("------------------------\n");

    //printf("X: %d, Y: %d\n",ret.x,ret.y);

    return ret;
}

char itemRandom (int* fantasmas, int* premios, int* vidas)
{
    char itemVec[(*fantasmas) + (*premios) + (*vidas)];
    // Comentado momentaneamente por ser una variable no utilizada.
    // char vecSel[3] = {'F','P','V'}
    char ret;
    int cantItem = 0;

    for(int i=0; i<(*fantasmas); i++)
    {
        itemVec[cantItem] = 'F';
        cantItem++;
    }

    for(int i=0; i<(*vidas); i++)
    {
        itemVec[cantItem] = 'V';
        cantItem++;
    }

    for(int i=0; i<(*premios); i++)
    {
        itemVec[cantItem] = 'P';
        cantItem++;
    }

    /*printf("------------------------\n");

    for(int i = 0; i<cantItem; i++)
    {
        printf("%c",itemVec[i]);
    }

    printf("\n");*/

    ret = itemVec[rand() % cantItem];

    //printf("Item: %c\n",ret);

    switch (ret)
    {
    case 'F':
        (*fantasmas)--;
        break;
    case 'V':
        (*vidas)--;
        break;
    case 'P':
        (*premios)--;
        break;
    }

    return ret;
}


void printMapa(int columnas, int filas, char mapa[filas][columnas])
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

void printMapaOnTxt(int columnas, int filas, char mapa[filas][columnas], FILE** file)
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

