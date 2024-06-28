#ifndef OTHERS_H
#define OTHERS_H

typedef struct Board Board;  // Forward declaration only


void limpiarPantalla();
void presioneTeclaParaContinuar();
void mostrarMenuPrincipal();

void inicializarTablero(Board *tablero, int width, int height);
void liberarTablero(Board *tablero);
void mostrarTablero(Board *tablero);

#endif