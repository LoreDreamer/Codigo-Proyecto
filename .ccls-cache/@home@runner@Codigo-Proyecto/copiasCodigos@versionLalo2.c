#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define MAXCOLS 10
#define MAXROWS 10

// Estructura para el tablero
typedef struct {

  char allyBoard[10][10];
  char enemyBoard[10][10];

  List* allyShips;
  List* enemyShips;

  Map* allyHistory;
  Map* enemyHistory;

  size_t remainingAllyShips;
  size_t remainingEnemyShips;

  char allyMockBoard[10][10];
  char enemyMockBoard[10][10];

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

void inicializarJuego(typeGame *game);
void menuPrincipal();
void presioneTeclaParaContinuar();
void limpiarPantalla();
void configurationMenu();
void mostrarTablero(char board[10][10]);
void crearFlotas(typeGame *game);
void liberarGame(typeGame *game);
void poblarTableros(char boardA[10][10], char boardB[10][10]);
void establecerFlotas(typeGame* game);
void displayBoard(char board[10][10]);
void placeShip(char board[10][10], int row, int col, int length, char orientation);
void configurePlayerFleet(char board[10][10], List *ships, char *playerName);
int is_equal_str(void *key1, void *key2);
int is_equal_int(void *key1, void *key2);
bool isPlacementValid(char board[10][10], int row, int col, int length, char orientation);

// gcc tdas/*.c main2.c -Wno-unused-result -o main2

int main() {

    srand(time(NULL)); // Inicializar generador de números aleatorios

    char opcion;
    typeGame game;

    do {

        inicializarJuego(&game); // Inicializar tablero 10x10 por defecto
        limpiarPantalla();
        menuPrincipal(); // Mostrar el menú principal

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {

            case '1':

                configurationMenu(&game);
                break;

            case '2':

                printf("\nSaliendo del sistema...\n");
                // liberarTablero(&tablero);
                break;

            default:

                printf("\nOpción inválida, ingrese nuevamente una opción.\n\n");
                break;

        }

        if (opcion != '1') {
            presioneTeclaParaContinuar();
            liberarGame(&game);
        }
    } while (opcion != '2');

    limpiarPantalla();

    return 0;
}

void inicializarJuego(typeGame *game) {

    // Inicializar tableros

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {

            game->allyBoard[i][j] = '~';
            game->enemyBoard[i][j] = '~';

            game->allyMockBoard[i][j] = '~';
            game->enemyMockBoard[i][j] = '~';

        }
    }

    // Inicializar listas

    game->allyShips = list_create();
    game->enemyShips = list_create();

    // Inicializar mapas

    game->allyHistory = map_create(is_equal_str);
    game->enemyHistory = map_create(is_equal_str);

    // Inicializar contadores

    game->remainingAllyShips = 5;
    game->remainingEnemyShips = 5;

}

void liberarGame(typeGame *game) {

    list_clean(game->allyShips);
    list_clean(game->enemyShips);

    map_clean(game->allyHistory);
    map_clean(game->enemyHistory);

    game->remainingAllyShips = 0;
    game->remainingEnemyShips = 0;

}

int is_equal_str(void *key1, void *key2) { return strcasecmp((char *) key1, (char *) key2) == 0;
}

int is_equal_int(void *key1, void *key2) { return *(int *)key1 == *(int *)key2; }

void menuPrincipal() {

  puts("\n================================\n");
  puts("          BATTLESHIP        \n");
  puts("================================\n");

  puts("1. Jugar");
  puts("2. Salir\n");

}

void presioneTeclaParaContinuar() { // Función para pausar el programa

  puts("Presione una tecla para continuar...");
  getchar(); 
  getchar(); 

}

void limpiarPantalla() {

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    fflush(stdout);

}

void configurationMenu(typeGame *game) {

    limpiarPantalla();

    puts("================================\n");
    puts("          BATTLESHIP        \n");
    puts("================================\n");

    printf("¡Bienvenidos a Battleship!\n\n");
    printf("Antes de empezar el combate contra su enemigo, debe configurar su flota.\n\n");

    sleep(2);

    limpiarPantalla();

    puts("================================\n");
    puts("          BATTLESHIP        \n");
    puts("================================\n");

    printf("Admiral, este es el sector enemigo:\n\n");
    mostrarTablero(game->allyBoard);
    crearFlotas(game); sleep(2);


    printf("Como puede ver, no tenemos información al respecto de su flota.\n"); sleep(3);
    printf("¡Debemos prepararnos para el combate!\n\n"); sleep(2);

    printf("Admiral, aquí tiene a los barcos disponibles:\n\n"); sleep(3);

    typeBoat *aux = list_first(game->allyShips);

    while (aux != NULL) {

        switch (aux->size) {

            case 1:

                printf("- Barco Patrulla - Tamaño: %d\n", aux->size);
                break;

            case 2:

                printf("- Submarino - Tamaño: %d\n", aux->size);
                break;

            case 3:

                printf("- Destructor - Tamaño: %d\n", aux->size);
                break;

            case 4:

                printf("- Crucero - Tamaño: %d\n", aux->size);
                break;

            case 5:

                printf("- Portaaviones - Tamaño: %d\n\n", aux->size);
                break;

            default:

                printf("- Barco Desconocido - Tamaño: %d\n", aux->size);
                break;

        }
        usleep(500000);
        aux = list_next(game->allyShips);
    }

    sleep(3);

    establecerFlotas(game);
}

void mostrarTablero(char board[10][10]) {

    printf("  ");

    for (int i = 0; i < 10; i++) 
        printf(" %2d", i + 1);

    printf("\n\n");

    for (int i = 0; i < 10; i++) {

        printf("%c ", 'A' + i);

        for (int j = 0; j < 10; j++) {
            printf("  %c", board[i][j]);
        }

        printf("\n");
        usleep(70000);

    }
    printf("\n\n");

    sleep(1);
}

void crearFlotas(typeGame *game) {


    for (int size = 1; size <= 5; size++) {

        typeBoat *boat = malloc(sizeof(typeBoat));

        if (boat == NULL) {

            perror("Failed to allocate memory for boat");
            exit(EXIT_FAILURE);

        }

        boat->size = size;
        boat->row = 0;
        boat->col = 0;
        boat->orient = 'H';

        list_pushBack(game->allyShips, boat); // Push allocated boat to list
    }

    for (int size = 1; size <= 5; size++) {

        typeBoat *boat = malloc(sizeof(typeBoat)); // Allocate memory for boat

        if (boat == NULL) {

            perror("Failed to allocate memory for boat");
            exit(EXIT_FAILURE);

        }

        boat->size = size;
        boat->row = 0;
        boat->col = 0;
        boat->orient = 'H';

        list_pushBack(game->enemyShips, boat); // Push allocated boat to list
    }
}

void configurePlayerFleet(char board[10][10], List *ships, char *playerName) {

    limpiarPantalla();

    printf("%s, este es su tablero:\n\n", playerName);
    mostrarTablero(board);

    typeBoat *boat;
    char row_char;
    int row, col;
    char orientation;

    for (boat = (typeBoat *)list_first(ships); boat != NULL; boat = (typeBoat *)list_next(ships)) {

        bool validPlacement = false;

        while (!validPlacement) {

            printf("Colocando barco de tamaño %d. Ingrese la fila (A-J) y columna (1-10) inicial: ", boat->size);

            if (scanf(" %c%d", &row_char, &col) != 2) {

                printf("Entrada inválida. Intente nuevamente.\n");
                while (getchar() != '\n'); // Clear the buffer
                continue;

            }

            row_char = toupper(row_char); // Convert to uppercase
            row = row_char - 'A'; // Convert letter to row index

            bool validOrientation = false;

            while (!validOrientation) {

                printf("Ingrese la orientación (H para horizontal, V para vertical): ");
                scanf(" %c", &orientation);

                while (getchar() != '\n'); // Clear the buffer

                orientation = toupper(orientation);

                if (orientation == 'H' || orientation == 'V') {

                    validOrientation = true;

                } else {

                    printf("\nOrientación inválida. Intente nuevamente.\\nn");

                }
            }

            if (isPlacementValid(board, row, col - 1, boat->size, orientation)) {

                placeShip(board, row, col - 1, boat->size, orientation);

                validPlacement = true;

                printf("Barco colocado con éxito.\n");
                limpiarPantalla();

                puts("================================\n");
                puts("          BATTLESHIP        \n");
                puts("================================\n");
                mostrarTablero(board);

            } else {

                printf("\nColocación inválida. Intente nuevamente.\n\n");

            }
        }
    }
}

bool isPlacementValid(char board[10][10], int row, int col, int length, char orientation) {

    if (orientation == 'H') {

        if (col + length > 10) return false; // Out of bounds horizontally

        for (int j = col; j < col + length; j++) 

            if (board[row][j] != '~') return false; // Overlapping another ship

    } else { // Vertical placement

        if (row + length > 10) return false; // Out of bounds vertically

        for (int i = row; i < row + length; i++) 

            if (board[i][col] != '~') return false; // Overlapping another ship

    }
    return true;
}

void placeShip(char board[10][10], int row, int col, int length, char orientation) {

    if (orientation == 'H') {

        for (int j = col; j < col + length; j++) 
            board[row][j] = 'O';

    } else { // Vertical placement

        for (int i = row; i < col + length; i++) 
            board[i][col] = 'O';

    }
}

void establecerFlotas(typeGame* game) {

    configurePlayerFleet(game->allyBoard, game->allyShips, "Admiral");

}