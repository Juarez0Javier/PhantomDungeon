#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stdio.h>

// Lee un string de cualquier archivo.
void leerStr(char* str, unsigned tam, FILE* iostream);
void trimStr(char* str);

// Lee y valida un string.
void ingrStrNoVacio(char* str, unsigned tam, const char* msjError);

#endif // STRING_H_INCLUDED
