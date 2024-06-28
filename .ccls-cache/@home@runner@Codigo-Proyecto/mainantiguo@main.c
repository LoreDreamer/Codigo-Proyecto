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
void inicializarTablero(Board *tablero, int width, int height); //**
void liberarTablero(Board *tablero);//*
void colocarBarco(Board *tablero, Barco *barco);//*
void mostrarBarco(Board *tablero, Barco *barco);//**
void organizarFlota(Board *tablero);//*
void mostrarTablero(Board *tablero);//*
void limpiarPantalla();//*
void presioneTeclaParaContinuar();//*
void mostrarMenuPrincipal();//*
void mostrarMenuBatalla(Board *tablero);//*
void elegirTamanoTablero(Board *tablero);//**
bool validarPosicion(Board *tablero, Barco *barco);//*

int main() {
    
    srand(time(NULL)); // Inicializar generador de números aleatorios
    
    char opcion;
    Board tablero;
    
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

void elegirTamanoTablero(Board *tablero){

    int width, height;

    limpiarPantalla();
    printf("Selecciona el tamaño del tablero:\n");
    printf("Ancho (entre 5 y 10): ");
    scanf("%d", &width);
    while (getchar() != '\n'); // Limpiar buffer de entrada
    printf("Altura (entre 5 y 10): ");
    scanf("%d", &height);
    while (getchar() != '\n'); // Limpiar buffer de entrada

    if (width < 5 || width > 10 || height < 5 || height > 10) {
        printf("Tamaño no válido. El tamaño del tablero debe estar entre 5 y 10.\n");
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

    // Mostrar números de las columnas
    for (int i = 0; i < tablero->width; i++) {
        printf("%2d ", i + 1);
    }
    printf("\n");

    // Mostrar letras de las filas y contenido del tablero
    for (int i = 0; i < tablero->height; i++) {
        printf("%c   ", 'A' + i); // Letras de las filas

        for (int j = 0; j < tablero->width; j++) {
            printf(" %c ", tablero->tablero[i][j]); // Contenido del tablero
        }
        printf("\n");
    }
}



void organizarFlota(Board *tablero) {
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
        if (isalpha(fila)) {
            fila = toupper(fila);
            barco->row = fila - 'A';
            if (barco->row < 0 || barco->row >= tablero->height) {
                printf("Fila no válida. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                free(barco);
                i--;
                continue;
            }
        } else {
            printf("Fila no válida. Intente de nuevo.\n");
            presioneTeclaParaContinuar();
            free(barco);
            i--;
            continue;
        }
        barco->col = columna - 1; // Convertir a índice base 0
        barco->size = i + 1;      // Tamaño del barco

        // Pedir orientación si es necesario
        if (i < 4) {
            printf("Seleccione la orientación (H - horizontal, V - vertical): ");
            scanf(" %c", &barco->orient);
            while (getchar() != '\n'); // Limpiar buffer de entrada

            // Validar orientación
            if (barco->orient != 'H' && barco->orient != 'V') {
                printf("Orientación no válida. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                free(barco);
                i--;
                continue;
            }
            // Validar espacio disponible
            if (barco->orient == 'H' && barco->col + barco->size > tablero->width) { // Corrected line
                printf("No hay suficiente espacio hacia la derecha para este barco. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                free(barco);
                i--;
                continue;
            }
            if (barco->orient == 'V' && barco->row + barco->size > tablero->height) {
                printf("No hay suficiente espacio hacia abajo para este barco. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                free(barco);
                i--;
                continue;
            }
            // Validar si la posición está ocupada
            if (!validarPosicion(tablero, barco)) {
                printf("La posición seleccionada está ocupada por otro barco. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                free(barco);
                i--;
                continue;
            }
            // Colocar el barco en el tablero
            colocarBarco(tablero, barco);
            mostrarTablero(tablero);
        }
    }
    printf("Flota organizada exitosamente.\n");
    mostrarTablero(tablero);
    presioneTeclaParaContinuar();  // Pausar para que el usuario vea el mensaje
}

// Función para limpiar la pantalla (dependiente del sistema operativo)
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    fflush(stdout); // Limpiar el buffer de salida
}
// Función para pausar y esperar la tecla Enter
void presioneTeclaParaContinuar() {
    int c;
    printf("Presione Enter para continuar...");
    while ((c = getchar()) != '\n' && c != EOF);
    getchar(); // Capturar el Enter
}
// Función para mostrar el menú principal
void mostrarMenuPrincipal() {
    limpiarPantalla();
    puts("========================================");
    puts("     Battleship (2024)");
    puts("========================================\n");
    puts("1) Iniciar Batalla");
    puts("2) Salir\n");
}

// Función para mostrar el menú de batalla
void mostrarMenuBatalla(Board *tablero) {
    char option;
    do {
        limpiarPantalla();
        puts("========================================");
        puts("     Tu Turno");
        puts("========================================\n");
        puts("0) Elegir tamaño tablero");
        puts("1) Organizar tu flota");
        puts("2) Iniciar Batalla\n");
        fflush(stdout);
        printf("Elige una opción: ");
        scanf(" %c", &option);
        while (getchar() != '\n' && getchar() != EOF); // Limpiar buffer de entrada
        switch (option) {
            case '0':
                elegirTamanoTablero(tablero);
                break;
            case '1':
                organizarFlota(tablero);
                break;
            case '2':
                printf("Iniciando la batalla...\n");
                presioneTeclaParaContinuar();
                break;
            default:
                printf("Opción inválida. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                break;
        }
    } while (option != '2');
}