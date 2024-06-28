#ifndef BATTLE_H
#define BATTLE_H

#include <stdbool.h>
#include "../tdas/list.h"
#include "../tdas/map.h"

typedef struct Board Board;
typedef struct Barco Barco;

void mostrarMenuBatalla(Board *tablero);
void atacarBarco(Board *tablero);
void marcarAtaque(Board *tablero, int row, int col);
bool barcoUndido(Board *tablero, Barco *barco, Map *resultadosAtaques);
void agregarAtaque(List *lista, int row, int col);
bool verificarVictoria(Board tablero); // Declaración de la función
void atacarBarcoComputadora(Board *tablero); // Declaración de la función
void inicializarBatalla(Board *tablero);

#endif