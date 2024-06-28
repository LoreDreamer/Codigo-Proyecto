#ifndef STARTUP_H
#define STARTUP_H

#include <stdbool.h>

typedef struct Board Board;  // Declaración anticipada de la estructura Board

// Función para limpiar la pantalla
void limpiarPantalla();

// Función para pausar la ejecución hasta que se presione una tecla
void presioneTeclaParaContinuar();

// Función para mostrar el menú principal del juego
void mostrarMenuPrincipal();

// Función para inicializar el tablero con las dimensiones especificadas
void inicializarTablero(Board *tablero, int width, int height);

// Función para liberar la memoria asignada al tablero
void liberarTablero(Board *tablero);

// Función para mostrar el tablero con un encabezado específico basado en el parámetro
void mostrarTablero(Board *tablero, int parametro);

// Función para determinar aleatoriamente quién toma el primer turno
bool determinarPrimerTurno();

// Función para llenar el tablero con un valor predeterminado
void repoblarTablero(Board *tablero);

// Función para mostrar las instrucciones del juego
void instrucciones();

#endif
