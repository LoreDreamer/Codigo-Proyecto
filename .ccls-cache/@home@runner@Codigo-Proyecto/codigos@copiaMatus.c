#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

// Estructura para el tablero
typedef struct {
    Map *board;
    int width;
    int height;
    List *barcos; // Asumiendo que List está definido y operativo
} Board;

typedef struct {
    int size;
    int row;     // Fila inicial (A = 0, B = 1, ..., Z = 25)
    int col;     // Columna inicial (0, 1, 2, ...)
    char orient; // 'H' para horizontal, 'V' para vertical
} Barco;

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

// Función para elegir el tamaño del tablero
void elegirTamanoTablero(Board *tablero) {
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

// Función para inicializar el tablero
void inicializarTablero(Board *tablero, int width, int height) {
    tablero->width = width;
    tablero->height = height;
    tablero->board = sorted_map_create(NULL); // Crear mapa ordenado
    tablero->barcos = list_create();
}

// Función para liberar el tablero
void liberarTablero(Board *tablero) {
    map_clean(tablero->board);
    list_clean(tablero->barcos);
}

// Función para colocar un barco en el tablero
void colocarBarco(Board *tablero, Barco *barco) {
    int x = barco->col;
    int y = barco->row;

    if (barco->orient == 'H') {
        for (int i = 0; i < barco->size; i++) {
            char key[10];
            snprintf(key, sizeof(key), "%d,%d", y, x + i);
            printf("Colocando barco en %s\n", key); // Mensaje de depuración
            map_insert(tablero->board, strdup(key), "*");
        }
    } else if (barco->orient == 'V') {
        for (int i = 0; i < barco->size; i++) {
            char key[10];
            snprintf(key, sizeof(key), "%d,%d", y + i, x);
            printf("Colocando barco en %s\n", key); // Mensaje de depuración
            map_insert(tablero->board, strdup(key), "*");
        }
    }

    // Guardar el barco en la lista de barcos
    list_pushBack(tablero->barcos, barco);
}

// Función para validar si la posición del barco está ocupada
bool validarPosicion(Board *tablero, Barco *barco) {
    if (barco->orient == 'H') {
        for (int i = 0; i < barco->size; i++) {
            char key[10];
            snprintf(key, sizeof(key), "%d,%d", barco->row, barco->col + i);
            printf("Validando posición %s\n", key); // Mensaje de depuración
            if (map_search(tablero->board, key) != NULL) {
                return false; // Posición ocupada
            }
        }
    } else if (barco->orient == 'V') {
        for (int i = 0; i < barco->size; i++) {
            char key[10];
            snprintf(key, sizeof(key), "%d,%d", barco->row + i, barco->col);
            printf("Validando posición %s\n", key); // Mensaje de depuración
            if (map_search(tablero->board, key) != NULL) {
                return false; // Posición ocupada
            }
        }
    }
    return true; // Posición disponible
}

// Función para mostrar cómo se verá el barco en el tablero
void mostrarBarco(Board *tablero, Barco *barco) {
    limpiarPantalla();
    printf("El barco se ha colocado exitosamente en el tablero.\n");
    printf("Así se verá tu barco en el tablero:\n\n");
    colocarBarco(tablero, barco);
    mostrarTablero(tablero); // Mostrar el tablero actualizado
    printf("\nPresiona Enter para continuar...");
    presioneTeclaParaContinuar();
}

// Función para organizar la flota
void organizarFlota(Board *tablero) {
    Barco *barcos = malloc(4 * sizeof(Barco)); 
    if (barcos == NULL) {
        printf("ERROR: Memoria no se pudo asignar.\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        limpiarPantalla();
        mostrarTablero(tablero);
        printf("\nOrganizando la flota - Barco de tamaño %d:\n", i + 1); // Usar i + 1 para mostrar el tamaño del barco correctamente

        char fila;
        int columna;

        printf("Ingrese la posición inicial (ej. A3 para fila A y columna 3): ");
        scanf(" %c%d", &fila, &columna);
        while (getchar() != '\n'); // Limpiar buffer de entrada

        // Validar y convertir fila a índice (A = 0, B = 1, ..., Z = 25)
        if (isalpha(fila)) {
            fila = toupper(fila);
            barcos[i].row = fila - 'A';
            if (barcos[i].row < 0 || barcos[i].row >= tablero->height) {
                printf("Fila no válida. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                i--;
                continue;
            }
        } else {
            printf("Fila no válida. Intente de nuevo.\n");
            presioneTeclaParaContinuar();
            i--;
            continue;
        }

        // Validar columna
        if (columna < 1 || columna > tablero->width) {
            printf("Columna no válida. Intente de nuevo.\n");
            presioneTeclaParaContinuar();
            i--;
            continue;
        }
        barcos[i].col = columna - 1; // Convertir a índice base 0

        // Pedir orientación si es necesario
        if (i < 3) { // No pedir orientación para el último barco (índice 3)
            printf("Seleccione la orientación (H - horizontal, V - vertical): ");
            scanf(" %c", &barcos[i].orient);
            while (getchar() != '\n'); // Limpiar buffer de entrada

            // Validar orientación
            if (barcos[i].orient != 'H' && barcos[i].orient != 'V') {
                printf("Orientación no válida. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                i--;
                continue;
            }

            // Validar espacio disponible
            if (barcos[i].orient == 'H' && barcos[i].col + i + 1 > tablero->width) { // i + 1 para ajustar tamaño del barco
                printf("No hay suficiente espacio hacia la derecha para este barco. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                i--;
                continue;
            }
            if (barcos[i].orient == 'V' && barcos[i].row + i + 1 > tablero->height) { // i + 1 para ajustar tamaño del barco
                printf("No hay suficiente espacio hacia abajo para este barco. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                i--;
                continue;
            }
        }

        // Validar si la posición está ocupada
        if (!validarPosicion(tablero, &barcos[i])) {
            printf("La posición seleccionada está ocupada por otro barco. Intente de nuevo.\n");
            presioneTeclaParaContinuar();
            i--;
            continue;
        }

        // Colocar el barco en el tablero
        colocarBarco(tablero, &barcos[i]);

        // Mostrar cómo se verá el barco en el tablero
        mostrarBarco(tablero, &barcos[i]);
    }

    free(barcos);
}


// Función para mostrar el tablero
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
            char key[10];
            snprintf(key, sizeof(key), "%d,%d", i, j);
            char *value = (char *)map_search(tablero->board, key); // Buscar en el mapa
            if (value == NULL) {
                printf("~ "); // Valor por defecto si la clave no se encuentra
            } else {
                printf("%s ", value);
            }
        }
        printf("\n");
    }
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