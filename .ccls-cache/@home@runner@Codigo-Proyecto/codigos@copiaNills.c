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
} Board;

typedef struct {
    int size;     
    int row;     // Fila inicial (A = 0, B = 1, ..., Z = 25)
    int col;     // Columna inicial (0, 1, 2, ...)
    char orient; // 'H' para horizontal, 'V' para vertical

} Barco;

typedef struct Node {
    Barco barco;
    struct Node *next;
} Node;

// Declaración de funciones
void inicializarTablero(Board *tablero, int width, int height);
void liberarTablero(Board *tablero);
void colocarBarco(Board *tablero, Barco *barco);
void mostrarBarco(Board *tablero, Barco *barco);
void organizarFlota(Board *tablero);
void mostrarTablero(Board *tablero);
void limpiarPantalla();
void presioneTeclaParaContinuar();
void mostrarMenuPrincipal();
void mostrarMenuBatalla(Board *tablero);
void elegirTamanoTablero(Board *tablero);
bool validarPosicion(Board *tablero, Barco *barco);

void elegirTamanoTablero(Board *tablero){
    int width, height;

    limpiarPantalla();
    printf("Selecciona el tamaño del tablero:\n");
    printf("Ancho (entre 5 y 20): ");
    scanf("%d", &width);
    while (getchar() != '\n'); // Limpiar buffer de entrada
    printf("Altura (entre 5 y 20): ");
    scanf("%d", &height);
    while (getchar() != '\n'); // Limpiar buffer de entrada

    if (width < 5 || width > 20 || height < 5 || height > 20) {
        printf("Tamaño no válido. El tamaño del tablero debe estar entre 5 y 20.\n");
        presioneTeclaParaContinuar();
    } else {
        liberarTablero(tablero); // Liberar el tablero actual antes de cambiar el tamaño
        inicializarTablero(tablero, width, height);
        printf("Tablero actualizado a %d x %d.\n", width, height);
        presioneTeclaParaContinuar();
    }
}

void inicializarTablero(Board *tablero, int width, int height) {
    tablero->width = width;
    tablero->height = height;
    for (int i = 0; i < tablero->height; i++) {
        for (int j = 0; j < tablero->width; j++) {
            tablero->tablero[i][j] = '~'; // Initialize with a default value like '~'
        }
    }
    tablero->resultadosAtaques = sorted_map_create(NULL); // Crear mapa ordenado
    tablero->barcos = list_create();
}

void liberarTablero(Board *tablero) {
    map_clean(tablero->resultadosAtaques);
    list_clean(tablero->barcos);
    free(tablero);
}

void colocarBarco(Board *tablero, Barco *barco) {
    int x = barco->row;
    int y = barco->col;
    if (barco->orient == 'H') { // Horizontal placement
        for (int i = 0; i < barco->size; i++) {
            if (y + i >= tablero->width) { // Check if ship goes out of bounds
                printf("Error: Ship goes out of bounds.\n");
                return;
            }
            tablero->tablero[x][y + i] = '*';
        }
    } else if (barco->orient == 'V') { // Vertical placement
        for (int i = 0; i < barco->size; i++) {
            if (x + i >= tablero->height) { // Check if ship goes out of bounds
                printf("Error: Ship goes out of bounds.\n");
                return;
            }
            tablero->tablero[x + i][y] = '*';
        }
    }
    list_pushBack(tablero->barcos, barco); // Add ship to the list
}

bool validarPosicion(Board *tablero, Barco *barco) {
    int x = barco->row;
    int y = barco->col;
    if (barco->orient == 'H') { // Horizontal placement
        for (int i = 0; i < barco->size; i++) {
            if (y + i >= tablero->width) { // Check if ship goes out of bounds
                return false;
            }
            if (tablero->tablero[x][y + i] != '~') { // Check if position is occupied
                return false;
            }
        }
    } else if (barco->orient == 'V') { // Vertical placement
        for (int i = 0; i < barco->size; i++) {
            if (x + i >= tablero->height) { // Check if ship goes out of bounds
                return false;
            }
            if (tablero->tablero[x + i][y] != '~') { // Check if position is occupied
                return false;
            }
        }
    }
    return true; // Position is available
}

void mostrarBarco(Board *tablero, Barco *barco) {
    limpiarPantalla();
    printf("El barco se ha colocado exitosamente en el tablero.\n");
    printf("Así se verá tu barco en el tablero:\n\n");
    colocarBarco(tablero, barco);
    mostrarTablero(tablero); // Mostrar el tablero actualizado
    printf("\nPresiona Enter para continuar...");
    presioneTeclaParaContinuar();
}
void mostrarTablero(Board *tablero) {
    limpiarPantalla();
    printf("    "); // Espacio para alinear con las columnas

    // Mostrar letras de las columnas
    for (int i = 0; i < tablero->width; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    // Mostrar números de las filas y contenido del tablero
    for (int i = 0; i < tablero->height; i++) {
        printf("%2d ", i + 1); // Números de las filas
        for (int j = 0; j < tablero->width; j++) {
            printf(" %c ", tablero->tablero[i][j]); // Contenido del tablero
        }
        printf("\n");
    }
}
void organizarFlota(Board *tablero){
    for (int i = 0; i < 4; i++) {
        limpiarPantalla();
        mostrarTablero(tablero);
        printf("\nOrganizando la flota - Barco de tamaño %d:\n", i + 1);

        Barco *barco = malloc(sizeof(Barco));
        if (barco == NULL) {
            printf("ERROR: Memoria no se pudo asignar.\n");
            return;
        }

        char fila;
        int columna;
        printf("Ingrese la posición inicial (ej. A3 para fila A y columna 3): ");
        scanf(" %c%d", &fila, &columna);
        while (getchar() != '\n'); // Limpiar buffer de entrada

        // Validar y convertir fila a índice (A = 0, B = 1, ..., Z = 25)
        
    }
    
}

int main(){

    return 0;
}

