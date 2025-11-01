#include "movimiento.h"

void _resolverMovFantasma(Partida* partida, Mapa* mapa, Movimiento* mov, int* seccion);
void _colisionJugadorFantasma (Partida* partida, Mapa* mapa, Entidad* fantasmaCol, int* seccion);
void reestablecerPosInicial (Entidad* e, Mapa* mapa);
void _aplicarMovimiento (Movimiento* mov, Mapa* mapa);

// Calcula si el movimiento que quiere hacer el jugador es valido.
void calcularMovJugador (Entidad* jugador, Mapa* mapa, char direccion, tCola* movs) {
    int difX = direccion == DERECHA ? 1 : direccion == IZQUIERDA ? -1 : 0;
    int difY = direccion == ABAJO ? 1 : direccion == ARRIBA ? -1 : 0;
    int nuevaX = jugador -> x+difX;
    int nuevaY = jugador -> y+difY;

    bool dentroDelTerreno =
    nuevaY < mapa -> filas &&
    nuevaY > -1 &&
    nuevaX < mapa -> cols &&
    nuevaX > -1;

    // Preferible anidar a tener falta de legibilidad.
    if (dentroDelTerreno) {

        bool transitable =
        mapa -> data[nuevaY][nuevaX] == CAMINO ||
        mapa -> data[nuevaY][nuevaX] == PREMIO ||
        mapa -> data[nuevaY][nuevaX] == VIDA ||
        mapa -> data[nuevaY][nuevaX] == SALIDA;

        if (transitable) {
            Movimiento mov = { nuevaX, nuevaY, jugador };

            // Validar que pueda ponerse en cola.
            if (!ponerEnCola(movs, &mov, sizeof(Movimiento)))
                printf("Error, memoria insuficiente para encolar movimiento");
        }
    }
}

// Calcula el próximo movimiento de cada fantasma hacia el jugador
void calcularMovFantasmas(Vector* fantasmas, Mapa* mapa, tCola* movs, Entidad* jugador, unsigned deltaTime)
{
    tCola colaBFS;
    Movimiento mov_final;
    // Estas matrices guardan el estado de exploracion y la ruta
    int ** mat_visitado = (int**)crearMatriz(mapa->filas, mapa->cols, sizeof(int));
    int ** mat_padreX  = (int**)crearMatriz(mapa->filas, mapa->cols, sizeof(int));
    int ** mat_padreY  = (int**)crearMatriz(mapa->filas, mapa->cols, sizeof(int));
    //verificar si son null

    // Movimiento ORTOGONAL
    int dx[8] = { 0, 0, 1, -1};
    int dy[8] = {-1, 1, 0, 0};
    Entidad* f;
    size_t i=0;
    int y, x, d;
    int metaX, metaY;
    bool encontrado;
    // Variables del BFS
    Movimiento m_actual;
    Movimiento m_nueva;
    int nx, ny;

    // Variables de reconstruccion
    int r_actualX, r_actualY;
    int anteriorX, anteriorY;
    bool retrocediendo;
    char val;
    bool transitable;



    crearCola(&colaBFS);

    while (i < fantasmas->tam) {
        bool cooldownPermiteMoverse = true;

        //f = (Entidad*)((char*)fantasmas->vec + i * fantasmas->tamElem);
        f= (Entidad*)(vectorObtenerElementoSegunPos(fantasmas, i));

        if (f -> ticksUltimoMov < f -> ticksEntreMovs) {
            f -> ticksUltimoMov += deltaTime;
            cooldownPermiteMoverse = false;
        }

        if (f->tipo == 'F' && f->eliminado == false && cooldownPermiteMoverse) {

            // Reinicia la matriz de visita (todos como no visitados)
            y = 0;
            while (y < mapa->filas) {
                x = 0;
                while (x < mapa->cols) {
                    ((int*)mat_visitado[y])[x] = false;
                    x++;
                }
                y++;
            }

            vaciarCola(&colaBFS);

            m_actual.x = f->x;
            m_actual.y = f->y;
            m_actual.ent = f;

            ponerEnCola(&colaBFS, &m_actual, sizeof(Movimiento));

            ((int*)mat_visitado[f->y])[f->x] = true;
            ((int*)mat_padreX[f->y])[f->x] = f->x;
            ((int*)mat_padreY[f->y])[f->x] = f->y;

            metaX = jugador->x;
            metaY = jugador->y;
            encontrado = false;

            //BFS: continúa mientras haya nodos en la cola y no se haya encontrado la meta.
            while (!colaVacia(&colaBFS) && !encontrado) {

                if (sacarDeCola(&colaBFS, &m_actual, sizeof(Movimiento))) {
                    d = 0;
                    while (d < 4) { // 4 direcciones

                        nx = m_actual.x + dx[d];
                        ny = m_actual.y + dy[d];
                        if (nx >= 0 && nx < mapa->cols && ny >= 0 && ny < mapa->filas) {

                            if ((((int*)mat_visitado[ny])[nx] == false) && (mapa->data[ny][nx] !=PARED) && !encontrado) {

                                // Marca como visitado, guarda padre y encola
                                ((int*)mat_visitado[ny])[nx] = true;
                                ((int*)mat_padreX[ny])[nx] = m_actual.x;
                                ((int*)mat_padreY[ny])[nx] = m_actual.y;

                                m_nueva.x = nx;
                                m_nueva.y = ny;
                                m_nueva.ent = f;
                                ponerEnCola(&colaBFS, &m_nueva, sizeof(Movimiento));

                                // Si llega al jugador
                                if (nx == metaX && ny == metaY) {
                                    encontrado = true;
                                }
                            }
                        }
                        d++;
                    }
                }
            }

            //Reconstrucción del camino
            if (encontrado) {

                r_actualX = metaX;
                r_actualY = metaY;

                retrocediendo = true;
                anteriorX = f->x;
                anteriorY = f->y;

                // El bucle busca el nodo 'anteriorX, anteriorY' y su padre es el fantasma 'f'
                while (retrocediendo) {

                    if (!(r_actualX == f->x && r_actualY == f->y)) {

                        anteriorX = r_actualX;
                        anteriorY = r_actualY;
                        r_actualX = ((int*)mat_padreX[anteriorY])[anteriorX];
                        r_actualY = ((int*)mat_padreY[anteriorY])[anteriorX];

                        // si el padre al que saltamos es el fantasma
                        if (r_actualX == f->x && r_actualY == f->y) {
                            retrocediendo = false;
                        }

                    } else {
                        retrocediendo = false;
                    }
                }

                // Al salir (anteriorX, anteriorY) es el primer paso
                nx = anteriorX;
                ny = anteriorY;

                // Encolar el movimiento final
                if (nx >= 0 && nx < mapa->cols && ny >= 0 && ny < mapa->filas) {
                    val = mapa->data[ny][nx];
                    transitable = (val == CAMINO || val == PREMIO || val == VIDA || val == SALIDA);

                    if (transitable) {
                        mov_final.ent= f;
                        mov_final.x=nx;
                        mov_final.y=ny;
                        ponerEnCola(movs, &mov_final, sizeof(Movimiento));
                    }
                }
            }
        }
        i++;
    }

    vaciarCola(&colaBFS);
    destruirMatriz(mapa->filas, (void**)mat_visitado);
    destruirMatriz(mapa->filas, (void**)mat_padreX);
    destruirMatriz(mapa->filas, (void**)mat_padreY);
}

const void * vectorObtenerElementoSegunPos(Vector * vec, unsigned pos)
{
    return((char*)vec->vec + pos*vec->tamElem); //hecho de esta forma pq luis dijo que la otra forma era ilegal!
}

// Desencola los movimientos realizados tanto por el jugador como por los fantamas y los aplica (si se puede).
void resolverMovimientos (Partida* partida, Mapa* mapa, tCola* movs, int* seccion) {

    Movimiento mov;
    Coordenada coord;
    Entidad* entidadCol;

    // Si no tiene movimientos para resolver, sale.
    if (!sacarDeCola(movs, &mov, sizeof(Movimiento))) {
        return;
    }

    if (mov.ent == &partida -> jugador) {
        // Añadir movimiento a registro de movimientos

        // Por ahora podemos dejarlo as�, las unicas entidades aparte del jugador, son los fantasmas.
        entidadCol = mapa -> entidades[mov.y][mov.x];

        if (entidadCol) {
            _colisionJugadorFantasma(partida, mapa, entidadCol, seccion);
            return;
        }

        switch (mapa -> data[mov.y][mov.x]) {
            case VIDA:
                partida -> vidasRestantes++;
                mapa -> data[mov.y][mov.x] = CAMINO;
                break;

            case PREMIO:
                partida -> premiosObt++;
                partida -> puntuacion += 100; // Asumiendo que el premio da 100 pts.
                mapa -> data[mov.y][mov.x] = CAMINO;
                break;

            case SALIDA:
                *seccion = SECCION_VICTORIA;
                break;
        }

        _aplicarMovimiento(&mov, mapa);

        coord.x = mov.x;
        coord.y = mov.y;

        if (!ponerEnListaAlFinal(&partida -> regMovs, &coord, sizeof(Coordenada)))
            printf("Error, memoria insuficiente para guardar movimiento\n");

    } else
        _resolverMovFantasma(partida, mapa, &mov, seccion);

    while (sacarDeCola(movs, &mov, sizeof(Movimiento)))
        _resolverMovFantasma(partida, mapa, &mov, seccion);
}

void mostrarCoordenada (const void* coord) {
    Coordenada* c = (Coordenada*) coord;
    printf("(%d,%d) ", c -> x, c -> y);
}

void contarMovs (void* elem, void* dst) {
    unsigned* cMovs = (unsigned*) dst;
    (*cMovs)++;
}

void _resolverMovFantasma (Partida* partida, Mapa* mapa, Movimiento* mov, int* seccion) {

    Entidad* entidadCol = mapa -> entidades[mov -> y][mov -> x];

    // Si no choca con nada, se mueve.
    if (!entidadCol) {
        _aplicarMovimiento(mov, mapa);
        return;
    }

    // Si choca con otro fantasma, no se mueve.
    if (entidadCol -> tipo == FANTASMA)
        return;

    // Aplica el movmiento para que se muestre, y luego procesa la colision.
    _aplicarMovimiento(mov, mapa);
    _colisionJugadorFantasma(partida, mapa, mov -> ent, seccion);
}

// Devuelve a todas las entidades a su posicion inicial, eliminando al fantasma de la colision.
void _colisionJugadorFantasma (Partida* partida, Mapa* mapa, Entidad* fantasmaCol, int* seccion) {
    Entidad* pf;
    VectorIterador it;

    partida -> vidasRestantes--;

    if (partida -> vidasRestantes == 0) {
        *seccion = SECCION_DERROTA;

        // Deja de mostrar al jugador en el mapa de entidades cuando pierde.
        mapa -> entidades[partida -> jugador.y][partida -> jugador.x] = NULL;
        return;
    }

    reestablecerPosInicial(&partida -> jugador, mapa);

    // Elimina al fantasma.
    mapa -> entidades[fantasmaCol -> y][fantasmaCol -> x] = NULL;
    fantasmaCol -> eliminado = true;

    vectorIteradorCrear(&it, &partida -> fantasmas);
    pf = vectorIteradorPrimero(&it);

    while (pf) {

        if (!pf -> eliminado)
            reestablecerPosInicial(pf, mapa);

        pf = vectorIteradorSiguiente(&it);
    }

    // Si un fantasma choca con el jugador, los dem�s descartan sus movimientos.
    vaciarCola(&partida -> movs);
}

void _aplicarMovimiento (Movimiento* mov, Mapa* mapa) {
    // Mueve a la entidad en el mapa de entidades.
    mapa -> entidades[mov -> ent -> y][mov -> ent -> x] = NULL;
    mapa -> entidades[mov -> y][mov -> x] = mov -> ent;

    // Cambia realmente la posicion de la entidad.
    mov -> ent -> x = mov -> x;
    mov -> ent -> y = mov -> y;

    // Reinicia el contador de ticks, reestableciendo el cooldown.
    mov -> ent -> ticksUltimoMov = 0;
}
