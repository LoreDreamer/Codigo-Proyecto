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
int is_equal_str(void *key1, void *key2);
int is_equal_int(void *key1, void *key2);




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
      sleep(1);
      pve();
      break;
    case '2':
      printf("Creando juego contra jugador...");
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

  printf("Bienvenidos a Battleship!\n\n");
  printf("Ustedes dos jugadores se enfrentarán en una batalla de hasta la muerte...\n");
  printf("Antes de eso, nos gustaría saber los nombres de los jugadores.\n\n");

  printf("Jugador 1, ingrese su nombre: ");
  scanf(" %s", game.allyCaptain);

  printf("Jugador 2, ingrese su nombre: ");
  scanf(" %s", game.enemyCaptain);



}

void pve() {

  printf("HATE NIGGERS\n");

}