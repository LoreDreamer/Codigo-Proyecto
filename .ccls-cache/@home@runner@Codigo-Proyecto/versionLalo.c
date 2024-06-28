#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {

  char allyBoard[10][10];
  char enemyBoard[10][10];

  List* allyShips;
  List* enemyShips;

  Map* allyHistory;
  Map* enemyHistory;

  size_t remainingAllyShips;
  size_t remainingEnemyShips;

  char allyCaptain[15];
  char enemyCaptain[15];

} typeGame;

typedef struct {

  char shipClass[10];
  int hp;

} typeBoat;

void pve();
void pvp();
void limpiarPantalla();
void presioneTeclaParaContinuar();
void menuPrincipal();
void configurationMenu();
void poblarTableros(char boardA[10][10], char boardB[10][10]);
void establecerFlotas(typeGame* game);
void displayBoard(char board[10][10]);
void placeShip(char board[10][10], int row, int col, int length, char orientation);
void configurePlayerFleet(char board[10][10], List *ships, char *playerName);
int is_equal_str(void *key1, void *key2);
int is_equal_int(void *key1, void *key2);
bool isPlacementValid(char board[10][10], int row, int col, int length, char orientation);

  // gcc tdas/*.c versionLalo.c -Wno-unused-result -o versionLalo
// ./versionLalo
// Prosigo con mi idea

int main() {

  char option;

  do {

    limpiarPantalla();
    menuPrincipal();
    printf("Escoja una opción: ");

    scanf(" %c", &option);

    switch (option) {

      case '1':

        printf("\nEntrando al juego...\n\n");
        configurationMenu();
        break;

      case '2':

        printf("\nSaliendo del sistema...\n");
        break;

      default:

        printf("\nOpción inválida, ingrese nuevamente una opción.\n\n");
        break;

    }

    if (option != '1')
      presioneTeclaParaContinuar();

  } while (option != '2');

  return 0;
}

int is_equal_str(void *key1, void *key2) { return strcasecmp((char *) key1, (char *) key2) == 0;
}

int is_equal_int(void *key1, void *key2) { return *(int *)key1 == *(int *)key2; } 

void menuPrincipal() {

  puts("================================\n");
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

void limpiarPantalla() { // Función para limpiar la pantalla

  system("clear"); 

}

void configurationMenu() {

  limpiarPantalla();

  puts("================================\n");
  puts("          BATTLESHIP        \n");
  puts("================================\n");

  puts("1. Jugar contra la computadora");
  puts("2. Jugar contra otro jugador");
  puts("3. Salir\n");

  char opcion;

  printf("Escoja una opción: ");
  scanf(" %s", &opcion);  

  switch(opcion) {

    case '1':

      printf("Creando juego contra computadora...");
      fflush(stdout);
      sleep(1);
      pve();
      break;

    case '2':

      printf("Creando juego contra jugador...");
      fflush(stdout);
      sleep(1);
      pvp();
      break;

    case '3':

      break;

    default:

      printf("Opción inválida, regresando al menu principal...\n\n");
      break;

  }

  presioneTeclaParaContinuar();

  if (opcion != '1' || opcion != '2')
    return;

}

void pvp() {

  limpiarPantalla();
  typeGame game;

  puts("================================\n");
  puts("          BATTLESHIP        \n");
  puts("================================\n");

  printf("¡Bienvenidos a Battleship!\n\n");
  printf("Ustedes dos jugadores se enfrentarán en una batalla de hasta la muerte...\n");
  printf("Antes de eso, nos gustaría saber los nombres de los jugadores.\n\n");

  printf("Jugador 1, ingrese su nombre: ");

  if (scanf("%s", game.allyCaptain) != 1) 
    fprintf(stderr, "Error leyendo el nombre del jugador 1.\n");

  while (getchar() != '\n'); // clear input buffer

  printf("Jugador 2, ingrese su nombre: ");

  if (scanf("%s", game.enemyCaptain) != 1) 
    fprintf(stderr, "Error leyendo el nombre del jugador 2.\n");

  while (getchar() != '\n'); // clear input buffer

  printf("\n¡Excelente! Ahora procederemos con la configuración de las flotas.\n\n");
  fflush(stdout);

  sleep(1);

  limpiarPantalla();

  puts("================================\n");
  puts("          BATTLESHIP        \n");
  puts("================================\n");

  game.allyShips = list_create();
  game.enemyShips = list_create();

  game.allyHistory = map_create(is_equal_str);
  game.enemyHistory = map_create(is_equal_str);

  game.remainingAllyShips = 5;
  game.remainingEnemyShips = 5;

  for (int i = 1; i <= 5; i++) {

    typeBoat *tempBoatA = malloc(sizeof(typeBoat));
    typeBoat *tempBoatB = malloc(sizeof(typeBoat));

    if (tempBoatA == NULL || tempBoatB == NULL) {

      fprintf(stderr, "Error al asignar memoria para los barcos.\n");
      exit(1);

    }

    printf("Jugador 1, ingrese el nombre de su barco %d: ", i);

    if (scanf("%s", tempBoatA->shipClass) != 1) {
      fprintf(stderr, "Error leyendo el nombre del barco del jugador 1.\n");
    }

    while (getchar() != '\n'); // clear input buffer

    printf("Jugador 2, ingrese el nombre de su barco %d: ", i);

    if (scanf("%s", tempBoatB->shipClass) != 1) {
      fprintf(stderr, "Error leyendo el nombre del barco del jugador 2.\n");
    }

    while (getchar() != '\n'); // clear input buffer

    tempBoatA->hp = i;
    tempBoatB->hp = i;

    list_pushBack(game.allyShips, tempBoatA);
    list_pushBack(game.enemyShips, tempBoatB);

    printf("\n");
  }

  printf("¡Excelente! Ahora procederemos con la configuración de los tableros.\n\n");

  fflush(stdout);
  sleep(1);

  limpiarPantalla();

  puts("================================\n");
  puts("          BATTLESHIP        \n");
  puts("================================\n");

  poblarTableros(game.allyBoard, game.enemyBoard);
  establecerFlotas(&game);

}

void pve() {

  printf("testeo\n");

}

void poblarTableros(char boardA[10][10], char boardB[10][10]) {

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {

      boardA[i][j] = '~';
      boardB[i][j] = '~';

    }
  }

}

void displayBoard(char board[10][10]) {

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {

      printf("%c ", board[i][j]);
      usleep(30000);

    }
    printf("\n");
  }

}

bool isPlacementValid(char board[10][10], int row, int col, int length, char orientation) {

  if (orientation == 'H') {

    if (col + length > 10) return false; // Out of bounds horizontally

    for (int j = col; j < col + length; j++) 
      if (board[row][j] != '~') return false; // Overlapping another ship

  } else {

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

  } else {

    for (int i = row; i < row + length; i++) 
      board[i][col] = 'O';

  }
}

void configurePlayerFleet(char board[10][10], List *ships, char *playerName) {

  printf("Jugador %s, este es su tablero:\n\n", playerName);
  displayBoard(board);
  printf("\n");

  typeBoat *boat;
  int row, col;
  char orientation;

  for (boat = (typeBoat *)list_first(ships); boat != NULL; boat = (typeBoat *)list_next(ships)) {

    bool validPlacement = false;

    while (!validPlacement) {

      printf("Colocando %s (%d unidades). Ingrese la fila (0-9) y columna (0-9) inicial: ", boat->shipClass, boat->hp);

      if (scanf("%d %d", &row, &col) != 2) {

        printf("Entrada inválida. Intente nuevamente.\n");
        while (getchar() != '\n'); // Clear the buffer
        continue;

      }

      bool validOrientation = false;

      while (!validOrientation) {

        printf("Ingrese la orientación (H para horizontal, V para vertical): ");
        scanf(" %c", &orientation);

        while (getchar() != '\n'); // Clear the buffer

        orientation = toupper(orientation);

        if (orientation == 'H' || orientation == 'V') {
          validOrientation = true;
        } else {
          printf("Orientación inválida. Intente nuevamente.\n");
        }

      }

      if (isPlacementValid(board, row, col, boat->hp, orientation)) {

        placeShip(board, row, col, boat->hp, orientation);
        validPlacement = true;

        printf("\nTablero actualizado:\n\n");
        displayBoard(board);
        printf("\n");

      } else {
        printf("Colocación inválida. Intente nuevamente.\n");
      }
    }
  }
}
void establecerFlotas(typeGame* game) {

  configurePlayerFleet(game->allyBoard, game->allyShips, game->allyCaptain);

  configurePlayerFleet(game->enemyBoard, game->enemyShips, game->enemyCaptain);

}