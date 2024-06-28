#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAXCOLS 20
#define MAXROWS 20

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
    tablero->resultadosAtaques = sorted_map_create(NULL); // Crear mapa ordenado para resultados de ataques
    tablero->barcos = list_create();
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            tablero->tablero[i][j] = '~'; // Inicializar con agua
        }
    }
}

// Función para liberar el tablero
void liberarTablero(Board *tablero) {
    map_clean(tablero->resultadosAtaques);
    list_clean(tablero->barcos);
}

// Función para colocar un barco en el tablero
void colocarBarco(Board *tablero, Barco *barco) {
    int x = barco->col;
    int y = barco->row;

    if (barco->orient == 'H') {
        for (int i = 0; i < barco->size; i++) {
            tablero->tablero[y][x + i] = '*';
        }
    } else if (barco->orient == 'V') {
        for (int i = 0; i < barco->size; i++) {
            tablero->tablero[y + i][x] = '*';
        }
    }

    // Guardar el barco en la lista de barcos
    list_pushBack(tablero->barcos, barco);
}

// Función para validar si la posición del barco está ocupada
// Función para validar si la posición del barco está ocupada
bool validarPosicion(Board *tablero, Barco *barco) {
    if (barco->orient == 'H') {
        // Verificar si el barco cabe horizontalmente y si las posiciones están libres
        if (barco->col + barco->size > tablero->width) {
            return false; // El barco se sale del tablero horizontalmente
        }
        for (int i = 0; i < barco->size; i++) {
            if (tablero->tablero[barco->row][barco->col + i] != '~') {
                return false; // Posición ocupada horizontalmente
            }
        }
    } else if (barco->orient == 'V') {
        // Verificar si el barco cabe verticalmente y si las posiciones están libres
        if (barco->row + barco->size > tablero->height) {
            return false; // El barco se sale del tablero verticalmente
        }
        for (int i = 0; i < barco->size; i++) {
            if (tablero->tablero[barco->row + i][barco->col] != '~') {
                return false; // Posición ocupada verticalmente
            }
        }
    }
    return true; // Posición disponible
}


// Función para mostrar cómo se verá el barco en el tablero
void mostrarBarco(Board *tablero, Barco *barco) {
    colocarBarco(tablero, barco); // Colocar el barco en el tablero antes de mostrarlo
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

        char columna;
        int fila;

        printf("Ingrese la posición inicial (ej. C4 para columna C y fila 4): ");
        scanf(" %c%d", &columna, &fila);
        while (getchar() != '\n'); // Limpiar buffer de entrada

        // Validar y convertir columna a índice (A = 0, B = 1, ..., Z = 25)
        if (isalpha(columna)) {
            columna = toupper(columna);
            barcos[i].col = columna - 'A';
            if (barcos[i].col < 0 || barcos[i].col >= tablero->width) {
                printf("Columna no válida. Intente de nuevo.\n");
                presioneTeclaParaContinuar();
                i--;
                continue;
            }
        } else {
            printf("Columna no válida. Intente de nuevo.\n");
            presioneTeclaParaContinuar();
            i--;
            continue;
        }

        // Validar fila y convertir a índice base 0
        if (fila < 1 || fila > tablero->height) {
            printf("Fila no válida. Intente de nuevo.\n");
            presioneTeclaParaContinuar();
            i--;
            continue;
        }
        barcos[i].row = fila - 1;

        // Pedir orientación si es necesario
        if (i < 4) { // No pedir orientación para el último barco (índice 4)
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
        } else {
            barcos[i].orient = 'H'; // Orientación horizontal por defecto para el último barco
        }

        // Ajustar tamaño del barco
        barcos[i].size = i + 1;

        // Validar si la posición está ocupada
        if (!validarPosicion(tablero, &barcos[i])) {
            printf("La posición seleccionada no es válida. Intente de nuevo.\n");
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
    printf("   "); // Espacio para alinear con las columnas

    // Mostrar letras de las columnas
    for (int i = 0; i < tablero->width; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    // Mostrar números de las filas y contenido del tablero poto poto poto ptoo
    for (int i = 0; i < tablero->height; i++) {
        printf("%2d ", i + 1); // Números de las filas
        for (int j = 0; j < tablero->width; j++) {
            printf("%c ", tablero->tablero[i][j]);
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