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

pos ejecutarBFS_y_obtenerPrimerPaso(Mapa* mapa, pos inicio, pos meta, int ** mat_visitado, pos ** mat_padre)
{
    tCola colaBFS;
    int dx[] = {0, 0, 1, -1};
    int dy[] = {-1, 1, 0, 0};
    int d;
    pos pos_actual, pos_siguiente;
    bool encontrado = false;
    pos r_actual;
    pos anterior;
    pos no_encontrado = {-1, -1};

    crearCola(&colaBFS);

    // Configuración inicial
    ponerEnCola(&colaBFS, &inicio, sizeof(pos));
    ((int*)mat_visitado[inicio.y])[inicio.x] = true;
    mat_padre[inicio.y][inicio.x] = inicio;

    // Bucle BFS
    while (!colaVacia(&colaBFS) && !encontrado) {

        if (sacarDeCola(&colaBFS, &pos_actual, sizeof(pos))) {
            d = 0;
            while (d < 4) {

                pos_siguiente.x = pos_actual.x + dx[d];
                pos_siguiente.y = pos_actual.y + dy[d];

                if (pos_siguiente.x >= 0 && pos_siguiente.x < mapa->cols &&
                    pos_siguiente.y >= 0 && pos_siguiente.y < mapa->filas) {

                    if ((((int*)mat_visitado[pos_siguiente.y])[pos_siguiente.x] == false) &&
                        (mapa->data[pos_siguiente.y][pos_siguiente.x] != PARED)) {

                        ((int*)mat_visitado[pos_siguiente.y])[pos_siguiente.x] = true;
                        mat_padre[pos_siguiente.y][pos_siguiente.x] = pos_actual;

                        ponerEnCola(&colaBFS, &pos_siguiente, sizeof(pos));

                        // Si llegamos al jugador, la bandera 'encontrado' se activa
                        if (pos_siguiente.x == meta.x && pos_siguiente.y == meta.y) {
                            encontrado = true;
                        }
                    }
                }
                d++;
            }
        }
    }

    vaciarCola(&colaBFS);

    // Reconstrucción y retorno
    if (encontrado) {
        r_actual = meta;
        anterior = inicio;

        while (!(r_actual.x == inicio.x && r_actual.y == inicio.y)) {
            anterior = r_actual;
            r_actual = mat_padre[anterior.y][anterior.x];
        }

        return anterior;
    } else {
        return no_encontrado;
    }
}

void calcularMovFantasmas(Vector* fantasmas, Mapa* mapa, tCola* movs, Entidad* jugador, unsigned deltaTime)
{
    pos ** mat_padre = (pos**)crearMatriz(mapa->filas, mapa->cols, sizeof(pos));
    int ** mat_visitado = (int**)crearMatriz(mapa->filas, mapa->cols, sizeof(int));
    Movimiento mov_final;
    Entidad* f;
    size_t i = 0;
    int y, x;
    pos meta = {jugador->x, jugador->y};
    pos inicio;
    pos primer_paso;
    char val;
    bool transitable;
    bool cooldownPermiteMoverse;

    while (i < fantasmas->tam) {
        cooldownPermiteMoverse = true;
        f = (Entidad*)(vectorObtenerElementoSegunPos(fantasmas, i));

        // Control de Cooldown (Delta Time)
        if (f -> ticksUltimoMov < f -> ticksEntreMovs) {
            f -> ticksUltimoMov += deltaTime;
            cooldownPermiteMoverse = false;
        }

        if (f->tipo == 'F' && f->eliminado == false && cooldownPermiteMoverse) {

            f -> ticksUltimoMov = 0;
            inicio.x = f->x;
            inicio.y = f->y;

            // Reiniciar la matriz de visitados
            y = 0;
            while (y < mapa->filas) {
                x = 0;
                while (x < mapa->cols) {
                    ((int*)mat_visitado[y])[x] = false;
                    x++;
                }
                y++;
            }

            // Llamada a la subfunción de Pathfinding
            primer_paso = ejecutarBFS_y_obtenerPrimerPaso(mapa, inicio, meta, mat_visitado, mat_padre);

            // Verifica si se encontró un camino válido
            if (primer_paso.x != -1) {

                // Encolar el movimiento final
                val = mapa->data[primer_paso.y][primer_paso.x];
                transitable = (val == CAMINO || val == PREMIO || val == VIDA || val == SALIDA || val == JUGADOR);

                if (transitable) {
                    mov_final.ent = f;
                    mov_final.x = primer_paso.x;
                    mov_final.y = primer_paso.y;
                    ponerEnCola(movs, &mov_final, sizeof(Movimiento));
                }
            }
        }
        i++;
    }

    // Liberación de recursos
    destruirMatriz(mapa->filas, (void**)mat_visitado);
    destruirMatriz(mapa->filas, (void**)mat_padre);
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
