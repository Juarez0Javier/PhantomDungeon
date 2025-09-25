#include "otrasFunciones.h"

#include <stdlib.h>

void clearConsole() {
    #if _WIN32 || WIN32 // TODO: Check if it works !!!
        system("cls");
    #else
        system("clear");
    #endif
}
