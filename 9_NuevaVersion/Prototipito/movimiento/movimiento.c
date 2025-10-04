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
            ponerEnCola(movs, &mov, sizeof(Movimiento));
        }
    }
}

// Calcula el proximo movimiento de cada fantasma.
void calcularMovFantasmas (Vector* fantasmas, Mapa* mapa, tCola* movs) {
    // A completar.
    // Tener en cuenta que no se debe calcular el movimiento de fantasmas que fueron eliminados.
}

// Desencola los movimientos realizados tanto por el jugador como por los fantamas y los aplica (si se puede).
void resolverMovimientos (Partida* partida, Mapa* mapa, tCola* movs, int* seccion) {

    Movimiento mov;
    Entidad* entidadCol;

    // Si no tiene movimientos para resolver, sale.
    if (!sacarDeCola(movs, &mov, sizeof(Movimiento))) {
        return;
    }

    if (mov.ent == &partida -> jugador) {
        // Añadir movimiento a registro de movimientos

        // Por ahora podemos dejarlo así, las unicas entidades aparte del jugador, son los fantasmas.
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

    } else
        resolverMovFantasma(partida, mapa, &mov, seccion);

    while (sacarDeCola(movs, &mov, sizeof(Movimiento)))
        resolverMovFantasma(partida, mapa, &mov, seccion);
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

    // Si un fantasma choca con el jugador, los demás descartan sus movimientos.
    vaciarCola(&partida -> movs);
}

void aplicarMovimiento (Movimiento* mov, Mapa* mapa) {
    // Mueve a la entidad en el mapa de entidades.
    mapa -> entidades[mov -> ent -> y][mov -> ent -> x] = NULL;
    mapa -> entidades[mov -> y][mov -> x] = mov -> ent;

    // Cambia realmente la posicion de la entidad.
    mov -> ent -> x = mov -> x;
    mov -> ent -> y = mov -> y;
}

