#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdbool.h>

// Declaración de las estructuras Board y Barco

typedef struct Board Board;
typedef struct Barco Barco;

void colocarBarco(Board *tablero, Barco *barco); // Función para colocar un barco en el tablero

void organizarFlota(Board *tablero); // Función para organizar la flota en el tablero

void elegirTamanoTablero(Board *tablero); // Función para organizar la flota en el tablero

void mostrarMenuConfiguracion(Board *tablero); // Función para mostrar el menú de configuración

void cambioDePosicion(Board *tablero); // Función para cambiar la posición de un barco en el tablero

void sacarBarcoTablero(Board *tablero, Barco *barco); // Función para cambiar la posición de un barco en el tablero

bool validarPosicion(Board *tablero, Barco *barco); // Función para validar si la posición de un barco en el tablero es válida

#endif
