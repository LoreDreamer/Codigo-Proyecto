#ifndef STARTUP_H
#define STARTUP_H

#include <stdbool.h>

typedef struct Board Board; // Declaración anticipada de la estructura Board

void limpiarPantalla(); // Función para limpiar la pantalla

void presioneTeclaParaContinuar(); // Función para pausar la ejecución hasta que se presione una tecla

void mostrarMenuPrincipal(); // Función para mostrar el menú principal del juego

void inicializarTablero(Board *tablero, int width, int height); // Función para inicializar el tablero con las dimensiones especificadas

void liberarTablero(Board *tablero); // Función para liberar la memoria asignada al tablero

void mostrarTablero(Board *tablero, int parametro); // Función para mostrar el tablero con un encabezado específico basado en el parámetro

bool determinarPrimerTurno(); // Función para determinar aleatoriamente quién toma el primer turno

void repoblarTablero(Board *tablero); // Función para llenar el tablero con un valor predeterminado

void instrucciones(); // Función para mostrar las instrucciones del juego

#endif
