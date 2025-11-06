# Phantom Dungeon - Laberintos y Fantasmas

Phantom Dungeon es un juego de laberintos desarrollado en C. El jugador debe navegar por un laberinto recolectando premios y evitando ser capturado por fantasmas mientras busca la salida. Se utilizó la biblioteca SDL2 para la interfaz gráfica, y se cuenta con conexión a servidor para tener acceso a un ranking de jugadores.

**Características principales:**

- Interfaz gráfica con SDL
- Generación de laberintos utilizando algoritmo DFS y algunas modificaciones.
- Movimientos de fantasmas utilizando algoritmo BFS (garantiza encontrar el camino óptimo)
- Sistema de puntuación y ranking
- Arquitectura cliente-servidor (Persistencia de datos de jugadores y partidas)

**Requisitos del Sistema para la Compilación**

- Windows (probado en Windows 10/11)
- Compilador MinGW
- Code::Blocks

Ante cualquier duda, consulte el manual del juego `Manual Phantom Dungeon.pdf`. Alli se encuentra una explicación más elaborada de todo lo aquí desarrollado.

## Instalación

### Descarga de Bibliotecas

#### 1. SDL2

1. En la página https://github.com/libsdl-org/SDL/releases
2. Busque la versión 2.0.22 [(link directo)](https://github.com/libsdl-org/SDL/releases?q=2.0.22&expanded=true)
3. Y descargue `SDL2-devel-2.0.22-mingw.zip` [(link directo)](https://github.com/libsdl-org/SDL/releases/download/release-2.0.22/SDL2-devel-2.0.22-mingw.zip)
4. Copie y descomprima en `libs/SDL2_linkNeededs`
5. Seleccione la carpeta según su arquitectura:
   - `x86_64-w64-mingw32` para 64 bits
   - `i686-w64-mingw32` para 32 bits
6. Copie los siguientes archivos a `SDL2_linkNeededs`:
   - De `bin/`: `SDL2.dll`
   - De `include/SDL2/`: Todos los archivos `.h`
   - De `lib/`: Todos los archivos `.a` y `.la`

```
Repo\libs\SDL2_linkNeededs\SDL2-2.0.22\x86_64-w64-mingw32>tree
D:.
├───bin (Aquí se encuentra el .dll)
├───include
│   └───SDL2 (Aquí se encuentran los .h)
├───lib (Aquí se encuentran los .a y .la)
│   ├───cmake
│   │   └───SDL2
│   └───pkgconfig
└───share
    └───aclocal
```

#### 2. SDL2_image

1. Descarge desde: https://github.com/libsdl-org/SDL_image/releases
2. Versión 2.8.8: `SDL2_image-devel-2.8.8-mingw.zip` [(link directo a la versión)](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.8) [(link directo a la descarga)](https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.8/SDL2_image-devel-2.8.8-mingw.zip)
3. Copie a `libs/SDL2_more` y descomprima
4. Copie a `SDL2_more`:
   - `SDL2_image.dll` (de `bin/`)
   - `SDL_image.h` (de `include/`)

```
Repo\libs\SDL2_more\SDL2_image-2.8.8\x86_64-w64-mingw32>tree
D:.
├───bin (Aquí se encuentra el .dll)
├───include (Aquí se encuentra el .h)
│   └───SDL2
└───lib
    ├───cmake
    │   └───SDL2_image
    └───pkgconfig
```

#### 3. SDL2_ttf

1. Descarge desde: https://github.com/libsdl-org/SDL_ttf/releases
2. Versión 2.24.0: `SDL2_ttf-devel-2.24.0-mingw.zip` [(link directo a la version)](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0) [(link directo a la descarga)](https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-mingw.zip)
3. Copia a `libs/SDL2_more` y descomprime
4. Copia a `SDL2_more`:
   - `SDL2_ttf.dll` (de `bin/`)
   - `SDL_ttf.h` (de `include/SDL2/`)

---

**Nota**: Asegurese de tener todas las bibliotecas instaladas correctamente antes de compilar. Ante cualquier inconveniente, consulte la sección de instalación detallada en el manual de juego (`Manual Phantom Dungeon.pdf`) adjunto.

---

### Compilación

Debe compilar tanto el proyecto .cbp de Cliente como de Servidor para poder jugar.

Si desea crear el proyecto de codeblocks de Cliente por su cuenta, los linkeos deben quedar de la siguiente manera:

- Linker Settings → Link libraries:
  ```
  SDL2
  SDL2main
  SDL2_ttf
  SDL2_image
  ```
- Linker Settings → Other linker options:
  ```
  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
  ```
- Search directories → Compiler y Search directories → Linker:
  ```
  ./libs/SDL2_linkNeededs
  ./libs
  ./libs/SDL2_more
  ./libs/graphics
  ./libs/Common
  ./libs/Cola
  ./libs/Vector
  ```

## Ejecución

### Opción 1: Usando archivo .bat (Recomendado)

Simplemente ejecute `PhantomDungeon.bat` y El archivo manejará automáticamente la ejecución

### Opción 2: Desde Code::Blocks

Según cual proyecto quiera ejecutar desde codeblocks (no podrá hacerlo con ambos) debera modificar las macros respectivas:

- Cliente → En `Cliente/constants.h`: `COMPILACION_DESDE_BIN false`
- Servidor → En `Servidor/constantes.h`: `COMPILACION_DESDE_BIN false`

Estas macros tienen como valor predefinido `true` ya que la opción predeterminada es la ejecución recomendada desde el `.bat`. Al cambiarlos a `false` las rutas relativas se tomarán respecto del archivo del proyecto `.cbp` y no del binario.

Recuerde siempre ejecutar primero el proyecto servidor para que el cliente pueda conectarse. Caso contrario no podrá acceder a algunas funcionalidades.

## Cómo Jugar

### Menu

El menu tiene varias opciones

- Jugar
- Salir
- Ver configuracion
- Ver ranking (solo posible con el servidor conectado)

Seleccione Jugar, si el servidor esta conectado se le solicitará un nombre, y luego presionando Jugar de nuevo ya esta listo para empezar a divertirse.

### Objetivo del juego

- Navegar por el laberinto recolectando premios
- Evitar a los fantasmas que persiguen al jugador
- Encontrar la salida y ganar

### Controles

- Flechas del teclado o WASD para moverse
- La ventana de SDL debe estar activa para recibir las entradas de teclado

### Características del Juego

- Puntuación: Se acumula recolectando premios
- Vidas extra: Se puede recolectar vidas adicionales
- Ranking: La puntuación se guarda en un ranking global
- Configuraciones: Se puede ver y modificar configuraciones del juego

## Algoritmos Implementados

### Generación de Laberintos

- DFS (Depth-First Search) para generación de laberintos
- Ajustes post-generación para mejorar jugabilidad
- Inserción de entidades mediante mascara (evita que se posicionen todas juntas)

### Movimiento de Fantasmas

- BFS (Breadth-First Search) para pathfinding óptimo
- Sistema de cooldown para movimiento balanceado

## Desarrollo

Grupo: Los predicadores de Linux

Alumnos:

- Conforti Luis
- Gallo Valentina
- Juarez Javier
- Porras Federico

Materia: Algoritmos y Estructuras de Datos (3640)

Universidad Nacional de La Matanza - 2º Cuatrimestre 2025

## Repositorio

Para más detalles, consultas o reportar issues, visite el repositorio oficial:
https://github.com/Juarez0Javier/PhantomDungeon
