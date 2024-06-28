#include "tdas/list.h"              
#include "tdas/hash.h"

#include "functionFolder/startup.h"  
#include "functionFolder/configuration.h" 
#include "functionFolder/battle.h"

#include <ctype.h>                   
#include <stdbool.h>                 
#include <stdio.h>                 
#include <stdlib.h>                 
#include <string.h>                  
#include <time.h>                   
#include <unistd.h>       

#define MAXCOLS 10                   
#define MAXROWS 10                   

struct Board {                       // Definir la estructura para el tablero / jugador

  char **tablero;                    // Matriz que representa el tablero
  HashMap *resultadosAtaques;        // Mapa hash para almacenar los resultados de los ataques
  int width;                         // Ancho del tablero
  int height;                        // Alto del tablero
  List *barcos;                      // Lista para almacenar los barcos
  bool flotaOrganizada;              // Indicador de si la flota está organizada

};

struct Barco {                       // Definir la estructura para cada barco

  int size;                          // Tamaño del barco
  int row;                           // Fila inicial (A = 0, B = 1, ..., Z = 25)
  int col;                           // Columna inicial (0, 1, 2, ...)
  char orient;                       // 'H' para horizontal, 'V' para vertical

};

struct Ataque {                      // Definir la estructura para cada ataque

  int row;                           // Fila del ataque
  int col;                           // Columna del ataque

};

int main() {

  srand(time(NULL));                 // Inicializar el generador de números aleatorios

  char opcion;                       // Variable para almacenar la opción del usuario
  Board tablero;                     // Declarar una instancia de la estructura Board

  inicializarTablero(&tablero, 10, 10); // Inicializar el tablero 10x10 por defecto
  limpiarPantalla();                 // Limpiar la pantalla antes de mostrar el menú

  do {
    
    mostrarMenuPrincipal();          // Mostrar el menú principal
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);           // Leer la opción ingresada por el usuario

    while (getchar() != '\n');       // Limpiar el buffer de entrada

    switch (opcion) {

    case '1':
      
      mostrarMenuConfiguracion(&tablero); // Mostrar el menú de configuración
      break;

    case '2':
      
      instrucciones();               // Mostrar las instrucciones del juego
      break;

    case '3':
      
      puts("Saliendo del juego..."); // Mostrar mensaje de salida
      liberarTablero(&tablero);      // Liberar la memoria del tablero
      break;

    default:
      
      puts("Opción no válida.\n");   // Mostrar mensaje de opción no válida
      presioneTeclaParaContinuar();  // Esperar a que el usuario presione Enter
      break;

    }

  } while (opcion != '3');           // Continuar el ciclo hasta que el usuario elija salir

  return 0;                          // Salir del programa con código de éxito
}
