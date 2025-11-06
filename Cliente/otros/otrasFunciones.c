#include "otrasFunciones.h"

#include <stdlib.h>

void clearConsole() {
    #if _WIN32 || WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
