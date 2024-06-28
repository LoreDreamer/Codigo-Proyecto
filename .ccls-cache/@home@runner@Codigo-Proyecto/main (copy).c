#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAXCOLS 10
#define MAXROWS 10

// Estructura para el tablero
typedef struct {

    char tablero[MAXROWS][MAXCOLS];
    Map *resultadosAtaques;
    int width;
    int height;
    List *barcos; 

} typeGame;

typedef struct {

    int size;     
    int row;     // Fila inicial (A = 0, B = 1, ..., Z = 25)
    int col;     // Columna inicial (0, 1, 2, ...)
    char orient; // 'H' para horizontal, 'V' para vertical

} typeBoat ;

typedef struct Node {

    typeBoat barco;
    struct Node *next;

} Node;



int main() {
    
    srand(time(NULL)); // Inicializar generador de números aleatorios
    
    char opcion;
    typeBoat tablero;
    
    inicializarTablero(&tablero, 10, 10); // Inicializar tablero 10x10 por defecto
    
    do {
        
        mostrarMenuPrincipal(); // Mostrar el menú principal
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
        while (getchar() != '\n'); // Limpiar buffer de entrada
        switch (opcion) {
            case '1':
                mostrarMenuBatalla(&tablero);
                break;
            case '2':
                puts("Saliendo del juego...");
                liberarTablero(&tablero);
                break;
            default:
                puts("Opción no válida.");
                presioneTeclaParaContinuar();
                break;
        }
    } while (opcion != '2');
    return 0;
}

