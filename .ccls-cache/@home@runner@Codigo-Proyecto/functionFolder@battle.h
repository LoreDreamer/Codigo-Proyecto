#ifndef BATTLE_H
#define BATTLE_H

#include <stdbool.h>    
#include "../tdas/list.h"
#include "../tdas/hash.h"

// Declaración de estructuras utilizadas en las funciones

typedef struct Board Board;
typedef struct Barco Barco;
typedef struct Ataque Ataque;

// Declaración de las funciones disponibles en el módulo de batalla

void inicializarBatalla(Board *tablero); // Función para iniciar la batalla

void colocarBarcosAleatoriamente(Board *tablero); // Función para colocar barcos aleatoriamente en el tablero

void atacarBarco(Board *tablero, Board *tableroVacio); // Función para que el jugador ataque un barco enemigo

void agregarAtaque(List *lista, int row, int col); // Función para agregar un ataque a una lista de ataques

bool verificarVictoria(Board tablero); // Función para verificar si se ha ganado la partida

void atacarBarcoComputadora(Board *tablero); // Función para que la computadora ataque al jugador

void marcarAtaqueExitoso(Board *tablero, int row, int col); // Función para marcar un ataque exitoso en el tablero

void marcarAtaqueFallido(Board *tablero, int row, int col); // Función para marcar un ataque fallido en el tablero

#endif // Fin de la directiva BATTLE_H
