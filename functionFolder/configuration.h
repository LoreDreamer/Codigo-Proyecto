#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdbool.h>

// Declaración de las estructuras Board y Barco
typedef struct Board Board;
typedef struct Barco Barco;

// Función para colocar un barco en el tablero
void colocarBarco(Board *tablero, Barco *barco);

// Función para organizar la flota en el tablero
void organizarFlota(Board *tablero);

// Función para elegir el tamaño del tablero
void elegirTamanoTablero(Board *tablero);

// Función para mostrar el menú de configuración
void mostrarMenuConfiguracion(Board *tablero);

// Función para cambiar la posición de un barco en el tablero
void cambioDePosicion(Board *tablero);

// Función para sacar un barco del tablero
void sacarBarcoTablero(Board *tablero, Barco *barco);

// Función para validar si la posición de un barco en el tablero es válida
bool validarPosicion(Board *tablero, Barco *barco);

#endif
