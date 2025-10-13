#include <stdio.h>
#include <stdlib.h>

#include "ListaSimple/Lista_Simple.h"
#include "ColaDina/Cola_Dinamica.h"

#define ROWS 10
#define COLS 10
#define MINLENGTH 20


typedef struct
{
    int x;
    int y;

} tVector2;

void mapExample();
void DirRandom (tVector2* dir);

int vector2ACard(tVector2 vec);
tVector2 cardaVector2(int card);

int main()
{
    srand(time(NULL));

    mapExample();




}


void mapExample()
{
    char mapa[ROWS][COLS];

    int i = 0;

    tVector2 pos, prevPos, dir, Entr;


    ///Inicializando Matriz en Paredes

    for(int i = 0; i<ROWS; i++)
    {
        for(int j = 0; j<COLS; j++)
                mapa[i][j] = '#';
    }



    ///Asignasr posicion inicial

    dir.y = 0;
    dir.x = 0;

    switch ((rand() % 4))
        {
            case 0:
                pos.x = 0;
                pos.y = 1 + (rand() % (COLS - 1));

                mapa[pos.y][pos.x] = 'E';

                dir.x = 1;

                break;

            case 1:
                pos.x = COLS - 1;
                pos.y = 1 + (rand() % (COLS - 1));

                mapa[pos.y][pos.x] = 'E';

                dir.x = -1;

                break;

            case 2:
                pos.x = 1 + (rand() % (ROWS - 1));
                pos.y = 0;

                mapa[pos.y][pos.x] = 'E';

                dir.y = 1;

                break;

            case 3:
                pos.x = 1 + (rand() % (ROWS - 1));
                pos.y = ROWS - 1;

                mapa[pos.y][pos.x] = 'E';

                dir.y = -1;

                break;
        }

    //printf("x = %d, y = %d\n", pos.x, pos.y);

    Entr.x = pos.x;
    Entr.y = pos.y;

    pos.x += dir.x;
    pos.y += dir.y;

    //printf("x = %d, y = %d\n", pos.x, pos.y);

    mapa[pos.y][pos.x] = '.';

    //printf("x = %d, y = %d\n", pos.x, pos.y);


    ///Hasta que encuentre un borde y el camino se amayor al minomo
    do{

        dir.x = dir.y = 0;

        DirRandom(&dir);

        //printf("Original Dir: X:%d, Y:%d\n",dir.x,dir.y);

        //printf("X Dir: %d + %d = %d\n", pos.x,dir.x,prevPos.x);


        ///Resolviendo direccion en X
        if(dir.x != 0)
        {

            if(pos.x + dir.x == prevPos.x)
            {
                //printf("\n\n Testing for X\n\n");
                DirRandom(&dir);
            }

            if((pos.x + dir.x >= COLS - 2) || (pos.x + dir.x < 1))
                DirRandom(&dir);

            //printf("X Clause: New Dir: X:%d, Y:%d\n",dir.x,dir.y);
        }

        //printf("Y Dir: %d + %d = %d\n", pos.y,dir.y,prevPos.y);

        ///Resolviendo direccion en Y
        if(dir.y != 0)
        {

            if(pos.y + dir.y == prevPos.y)
                DirRandom(&dir);

            if((pos.y + dir.y >= ROWS - 2) || (pos.y + dir.y < 1))
                DirRandom(&dir);

            //printf("Y Clause: New Dir: X:%d, Y:%d\n",dir.x,dir.y);
        }

        //printf("\n");

        prevPos.x = pos.x;
        prevPos.y = pos.y;

        pos.x += dir.x;
        pos.y += dir.y;

        mapa[pos.y][pos.x] = '.';

        //printf("x = %d, y = %d\n", pos.x, pos.y);

        i++;

    } while (!((pos.y == 1 || pos.y == ROWS - 1 || pos.x == 1 || pos.x == COLS - 1) && (i>MINLENGTH) && (Entr.x != (pos.x +1) &&  Entr.y != (pos.y +1))));

    //printf("Dir X: %d, Dir Y: %d\n", dir.x, dir.y);

    ///Resolviendo el posicionamiento de la Salida

    dir.x = dir.y = 0;

    if(pos.y == 1)
        dir.y--;
    if(pos.y == ROWS - 1)
        dir.y++;

    if(pos.x == 1)
        dir.x--;
    if(pos.x == COLS - 1)
        dir.x++;

    if(dir.x !=0 && dir.y != 0)
    {
        switch(rand() % 2)
        {
            case 0:
                dir.x = 0;
                break;
            case 1:
                dir.y = 0;
                break;
        }
    }

    mapa[pos.y + dir.y][pos.x + dir.x] = 'S';

    ///Llenando el Mapa con caminos randomizados

    for(int i = 1; i<ROWS - 1; i++)
    {
        for(int j = 1; j<COLS -1; j++)
        {
            if(mapa[i][j] == '#')
            {
                if(!(mapa[i+1][j] == '#' && mapa[i-1][j] == '#' && mapa[i][j+1] == '#' && mapa[i][j-1] == '#'))
                {
                    switch((rand() % 3))
                    {
                        case 0:
                        case 1:
                            mapa[i][j] = '.';
                            break;
                    }
                }

            }

        }

    }

    for(int i = 0; i<ROWS; i++)
    {
        for(int j = 0; j<COLS; j++)
        {
            printf("%c",mapa[i][j]);

        }
        printf("\n");
    }
}

void DirRandom (tVector2* dir, int cardInval[4])
{
    int sel, vecSel[4] = {0,0,0,0};
    int vecTam = 0;
    int card = vector2ACard(*dir);


    //printf("Card = %d\n",card);


    for(int i = 0; i < 4; i++)
    {
        if(cardValid[i]!=0)
        {
            vecSel[vecTam] = i + 1;
            vecTam++;
        }
    }

    for(int i = 0; i < 4; i++)
    {
        if(card != i + 1)
        {
            vecSel[vecTam] = i + 1;
            vecTam++;
        }
    }

    //printf("\n");

    //printf("XDir = %d, YDir = %d \n",dir->x,dir->y);

    sel = vecSel[rand() % vecTam];

    dir->x = cardaVector2(sel).x;
    dir->y = cardaVector2(sel).y;

    /*

    switch (sel)
        {
            case 0:
                printf("Sel Value %d\n", sel);
                printf("Proc 0\n");

                if(dir->x == 1)
                    sel = 1;
                else
                    dir->y = 1;
                break;
            case 1:
                printf("Sel Value %d\n", sel);
                printf("Proc 1\n");
                if(dir->y == -1)
                    sel = 2;
                else
                    dir->y = -1;
                break;
            case 2:
                printf("Sel Value %d\n", sel);
                printf("Proc 2\n");
                if(dir->x == 1)
                    sel = 3;
                else
                    dir->x = 1;
                break;
            case 3:
                printf("Sel Value %d\n", sel);
                printf("Proc 3\n");
                if(dir->x == -1)
                    sel = 0;
                else
                    dir->x = -1;
                break;
        }*/

    //printf("XDir = %d, YDir = %d \n",dir->x,dir->y);
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
