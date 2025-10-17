#include "mapaRandom.h"

int generarMapaRandom(ConfigData* configData, char nombreArch[])
{
    ///Temporal. Hasta que se pueda cargar configuracion.
    int filas = 10, columnas = 10;
    int fantasmas = 2, premios = 1, vidas = 1;

    ///Habilitar cuando se pueda cargar la configuracion
    //int filas = configData->filas, columnas = configData->columnas;
    //int fantasmas = configData->maximo_numero_fantasmas, premios = configData->maximo_numero_premios, vidas = configData->maximo_vidas_extra;

    char mapa[filas][columnas], item =' ';
    int cardBloq[4], vecSel[4];
    int selTam;
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
            dirRandom(&dir,cardBloq);

        pos.x += dir.x;
        pos.y += dir.y;

        mapa[pos.y][pos.x] = '.';

        ///Resolviendo el posicionamiento de la Salida candidata

        if(pos.y == 1 && abs(posE.y - pos.y) > columnas / 2)
        {
            posS.y = pos.y - 1;
            posS.x = pos.x;
        }
        if(pos.y == filas - 2 && abs(posE.y - pos.y) > columnas / 2)
        {
            posS.y = pos.y + 1;
            posS.x = pos.x;
        }
        if(pos.x == 1 && abs(posE.x - pos.x) > filas / 2)
        {
            posS.y = pos.y;
            posS.x = pos.x - 1;
        }

        if(pos.x == columnas - 2 && abs(posE.x - pos.x) > filas / 2)
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

    item = 'F';
    for(int i=0; i<fantasmas; i++)
        apilarD(&stack,&item,sizeof(char));

    item = 'P';
    for(int i=0; i<premios; i++)
        apilarD(&stack,&item,sizeof(char));

    item = 'V';
    for(int i=0; i<vidas; i++)
        apilarD(&stack,&item,sizeof(char));

    while(!pilaDVacia(&stack))
    {
        pos.x = 1 + (rand() % (filas - 2));
        pos.y = 1 + (rand() % (columnas - 2));

        if(mapa[pos.y][pos.x] == '.' && abs(posE.x - pos.x) > filas / 5 && abs(posE.y - pos.y) > columnas / 5)
        {
            desapilarD(&stack,&item,sizeof(char));
            mapa[pos.y][pos.x] = item;
        }
    }

    vaciarPilaD(&stack);

    //printMapa(columnas,filas,mapa);

    ///Imprimoendo el Mapa en el archivo de texto

    printMapaOnTxt(columnas,filas,mapa,&archTxt);

    fclose(archTxt);

    return 1;
}

void dirRandom (tVector2* dir, int cardBloq[4])
{
    int sel, vecSel[4] = {0,0,0,0};
    int vecTam = 0;

    for(int i = 0; i < 4; i++)
    {
        if(cardBloq[i]!= 1)
        {
            vecSel[vecTam] = i + 1;
            vecTam++;
        }
    }

    sel = vecSel[rand() % vecTam];

    dir->x = cardaVector2(sel).x;
    dir->y = cardaVector2(sel).y;
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

