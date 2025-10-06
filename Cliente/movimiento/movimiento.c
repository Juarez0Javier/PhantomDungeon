#include "movimiento.h"

// Internas
void resolverMovFantasma(Partida* partida, Mapa* mapa, Movimiento* mov, int* seccion);
void colisionJugadorFantasma (Partida* partida, Mapa* mapa, Entidad* fantasmaCol, int* seccion);
void reestablecerPosInicial (Entidad* e, Mapa* mapa);
void aplicarMovimiento (Movimiento* mov, Mapa* mapa);


// --------------------------- Principales ---------------------------

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
void calcularMovFantasmas(Vector* fantasmas, Mapa* mapa, tCola* movs, Entidad* jugador, unsigned deltaTime) {
    // --- Variables ---
    int dx[4] = {0, 0, -1, 1};  // Movimiento ortogonal X (arriba, abajo, izquierda, derecha)
    int dy[4] = {-1, 1, 0, 0};  // Movimiento ortogonal Y
    Entidad* f;                  // Fantasma actual
    int visitado[mapa->filas][mapa->cols];    // Matriz para marcar celdas visitadas
    int padreX[mapa->filas][mapa->cols];     // Para reconstruir el camino (X)
    int padreY[mapa->filas][mapa->cols];     // Para reconstruir el camino (Y)
    int colaX[mapa->filas * mapa->cols];     // Cola BFS X
    int colaY[mapa->filas * mapa->cols];     // Cola BFS Y
    int inicio, fin;             // Índices de la cola BFS
    int metaX, metaY;            // Posición del jugador
    int encontrado;              // bandera para indicar si llegamos al jugador
    int actualX, actualY;        // Nodo actual BFS
    int nx, ny;                  // Vecinos (esto quedaria mas lindo con una estructura coordenada maybe)
    int px, py, tx;              // Para retroceder al primer paso
    char val;                     // Valor de la celda
    int transitable;              // bandera para saber si la celda es transitable

    // recorre todos los fantasmas
    for (size_t i = 0; i < fantasmas->tam; i++) {
        f = (Entidad*)((char*)fantasmas->vec + i * fantasmas->tamElem);

        // Saltea si fue eliminado o aun no puede moverse.
        if (f->eliminado) continue;

        // Si no puede moverse por cooldown, va aumentando los ticks hasta que eventualmente pueda moverse.
        if (f -> ticksUltimoMov < f -> ticksEntreMovs) {
            f -> ticksUltimoMov += deltaTime;
            continue;
        }

        // Reinicia BFS para este fantasma
        for (int y = 0; y < mapa->filas; y++)
            for (int x = 0; x < mapa->cols; x++)
                visitado[y][x] = 0;

        inicio = 0;
        fin = 0;

        // Inicializa cola BFS con la posición del fantasma
        colaX[fin] = f->x;
        colaY[fin] = f->y;
        fin++;
        visitado[f->y][f->x] = 1;
        padreX[f->y][f->x] = f->x;
        padreY[f->y][f->x] = f->y;

        // Posición objetivo: jugador
        metaX = jugador->x;
        metaY = jugador->y;
        encontrado = 0;

        // BFS puro
        while (inicio < fin && !encontrado) {
            actualX = colaX[inicio];
            actualY = colaY[inicio];
            inicio++;

            // Revisa los 4 vecinos ortogonales
            for (int d = 0; d < 4; d++) {
                nx = actualX + dx[d];
                ny = actualY + dy[d];

                // Saltea si esta fuera del mapa
                if (nx < 0 || nx >= mapa->cols || ny < 0 || ny >= mapa->filas) continue;
                // Saltea si ya fue visitado
                if (visitado[ny][nx]) continue;
                // Saltea si hay pared
                if (mapa->data[ny][nx] == PARED) continue;

                // Marca como visitado y guarda padre
                visitado[ny][nx] = 1;
                padreX[ny][nx] = actualX;
                padreY[ny][nx] = actualY;

                // Añade a la cola BFS
                colaX[fin] = nx;
                colaY[fin] = ny;
                fin++;

                // Si llega al jugador, termina búsqueda
                if (nx == metaX && ny == metaY) {
                    encontrado = 1;
                    break;
                }
            }
        }

        // --- Retroceder hasta el primer paso desde el jugador ---
        px = metaX;
        py = metaY;
        while (!(padreX[py][px] == f->x && padreY[py][px] == f->y)) {
            tx = px;
            px = padreX[py][tx];
            py = padreY[py][tx];
        }

        // --- Encolar el movimiento si es transitable ---
        if (px < mapa->cols && py < mapa->filas) {
            val = mapa->data[py][px];
            transitable = (val == CAMINO || val == PREMIO || val == VIDA || val == SALIDA);
            if (transitable) {
                Movimiento mov = {px, py, f};
                ponerEnCola(movs, &mov, sizeof(Movimiento));
            }
        }
    }
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
            colisionJugadorFantasma(partida, mapa, entidadCol, seccion);
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

        aplicarMovimiento(&mov, mapa);

        coord.x = mov.x;
        coord.y = mov.y;

        if (!ponerEnListaAlFinal(&partida -> regMovs, &coord, sizeof(Coordenada), true, NULL)) 
            printf("Error, memoria insuficiente para guardar movimiento\n");

    } else
        resolverMovFantasma(partida, mapa, &mov, seccion);

    while (sacarDeCola(movs, &mov, sizeof(Movimiento)))
        resolverMovFantasma(partida, mapa, &mov, seccion);
}

void mostrarCoordenada (const void* coord) {
    Coordenada* c = (Coordenada*) coord;
    printf("(%d,%d) ", c -> x, c -> y);
}

// --------------------------- Internas ---------------------------

void resolverMovFantasma (Partida* partida, Mapa* mapa, Movimiento* mov, int* seccion) {

    Entidad* entidadCol = mapa -> entidades[mov -> y][mov -> x];

    // Si no choca con nada, se mueve.
    if (!entidadCol) {
        aplicarMovimiento(mov, mapa);
        return;
    }

    // Si choca con otro fantasma, no se mueve.
    if (entidadCol -> tipo == FANTASMA)
        return;

    // Aplica el movmiento para que se muestre, y luego procesa la colision.
    aplicarMovimiento(mov, mapa);
    colisionJugadorFantasma(partida, mapa, mov -> ent, seccion);
}

// Devuelve a todas las entidades a su posicion inicial, eliminando al fantasma de la colision.
void colisionJugadorFantasma (Partida* partida, Mapa* mapa, Entidad* fantasmaCol, int* seccion) {
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

void aplicarMovimiento (Movimiento* mov, Mapa* mapa) {
    // Mueve a la entidad en el mapa de entidades.
    mapa -> entidades[mov -> ent -> y][mov -> ent -> x] = NULL;
    mapa -> entidades[mov -> y][mov -> x] = mov -> ent;

    // Cambia realmente la posicion de la entidad.
    mov -> ent -> x = mov -> x;
    mov -> ent -> y = mov -> y;

    // Reinicia el contador de ticks, reestableciendo el cooldown.
    mov -> ent -> ticksUltimoMov = 0;
}