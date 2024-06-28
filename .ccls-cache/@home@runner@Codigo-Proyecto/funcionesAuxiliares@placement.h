#ifndef PLACEMENT_H
#define PLACEMENT_H
#include <stdbool.h>

typedef struct Board Board;
typedef struct Barco Barco;

void colocarBarco(Board *tablero, Barco *barco);
void organizarFlota(Board *tablero);
void elegirTamanoTablero(Board *tablero);
bool validarPosicion(Board *tablero, Barco *barco);

#endif