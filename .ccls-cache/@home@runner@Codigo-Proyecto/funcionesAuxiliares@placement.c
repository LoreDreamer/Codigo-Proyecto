#include "placement.h"
#include "others.h"
#include "../tdas/list.h"
#include "../tdas/map.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCOLS 10
#define MAXROWS 10

struct Board {

    char tablero[MAXROWS][MAXCOLS];

    int width;
    int height;

    Map *resultadosAtaques;

    List *barcos; 
    List *ataquesRealizadosUsuario; 
    List *ataquesRealizadosMaquina;

    bool flotaOrganizada;

};

struct Barco {

    int size;     
    int row;     // Fila inicial (A = 0, B = 1, ..., Z = 25)
    int col;     // Columna inicial (0, 1, 2, ...)
    char orient; // 'H' para horizontal, 'V' para vertical

};


void elegirTamanoTablero(Board *tablero){

    int width, height;

    limpiarPantalla();
    puts("========================================\n"); usleep(500000);
    puts("         TAMAÑO DEL TABLERO\n"); usleep(500000);
    puts("========================================\n"); usleep(500000);


    puts("Seleccionando el tamaño del tablero...\n"); usleep(500000);
    printf("Ancho del tablero (entre 5 y 10): ");
    scanf("%d", &width);
    
    while (getchar() != '\n');

    printf("Altura del tablero (entre 5 y 10): ");
    scanf("%d", &height);

    while (getchar() != '\n');

    if (width < 5 || width > 10 || height < 5 || height > 10) {

        printf("Tamaño no válido. El tamaño del tablero debe estar entre 5 y 10.\n");

        fflush(stdout);
        sleep(2);

        printf("Voliendo al menú de configuración...\n");

        fflush(stdout);
        sleep(2);
        

    } else {

        liberarTablero(tablero); // Liberar el tablero actual antes de cambiar el tamaño
        inicializarTablero(tablero, width, height);

        printf("\nTablero actualizado a %d x %d.\n\n", width, height);
        fflush(stdout);
        sleep(2);

    }
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

            if (y + i >= tablero->width) 
                return false;

            if (tablero->tablero[x][y + i] != '~')
                return false;

        }

    } else if (barco->orient == 'V') { // Vertical placement

        for (int i = 0; i < barco->size; i++) {

            if (x + i >= tablero->height)
                return false;

            if (tablero->tablero[x + i][y] != '~')
                return false;

        }

    }

    return true; // Position is available
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
                sleep(2);
                free(barco);
                i--;

                continue;
            }

        } else {

            printf("Fila no válida. Intente de nuevo.\n");
            sleep(2);
            free(barco);
            i--;

            continue;
        }

        barco->col = columna - 1; // Convertir a índice base 0
        barco->size = i + 1; // Tamaño del barco

        printf("Seleccione la orientación (H - horizontal, V - vertical): ");
        scanf(" %c", &barco->orient);

        while (getchar() != '\n'); // Limpiar buffer de entrada

        // Validar orientación
        if (barco->orient != 'H' && barco->orient != 'V') {

            printf("Orientación no válida. Intente de nuevo.\n");
            sleep(2);
            free(barco);
            i--;

            continue;

        }

        if (barco->orient == 'H' && barco->col + barco->size > tablero->width) { 

            printf("No hay suficiente espacio a la derecha para este barco. Intente de nuevo.\n");
            sleep(2);
            free(barco);
            i--;

            continue;

        }

        if (barco->orient == 'V' && barco->row + barco->size > tablero->height) {

            printf("No hay suficiente espacio hacia abajo para este barco. Intente de nuevo.\n");

            sleep(2);
            free(barco);
            i--;

            continue;
        }

        // Validar si la posición está ocupada
        if (!validarPosicion(tablero, barco)) {

            printf("La posición seleccionada está ocupada por otro barco. Intente de nuevo.\n");

            sleep(2);
            free(barco);
            i--;

            continue;

        }

        colocarBarco(tablero, barco);
    }

    tablero->flotaOrganizada = true;

    mostrarTablero(tablero);

    printf("\nFlota organizada exitosamente.\n");

    fflush(stdout); // Flush the output buffer
    sleep(2);

    printf("Volviendo a configuración de flota...\n");

    fflush(stdout); // Flush the output buffer
    sleep(2);
}
