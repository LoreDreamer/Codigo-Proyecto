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

struct Barco{

    int size;     
    int row;     // Fila inicial (A = 0, B = 1, ..., Z = 25)
    int col;     // Columna inicial (0, 1, 2, ...)
    char orient; // 'H' para horizontal, 'V' para vertical

};

typedef struct Node {

    Barco barco;
    struct Node *next;

} NodeBarco;

typedef struct {

    int row;
    int col;

} Ataque;

void agregarAtaque(List *lista, int row, int col) {

  Ataque *ataque = (Ataque *) malloc(sizeof(Ataque));

  if (ataque == NULL) {

      printf("ERROR: No se pudo asignar memoria para el ataque.\n");
      return;

  }

  ataque->row = row;
  ataque->col = col;

  list_pushBack(lista, ataque);
}

void recorrerListaAtaques(List *lista) {

  list_first(lista);

  Ataque *ataque;

  while ((ataque = list_next(lista)) != NULL)
    printf("Ataque en posición: %c%d\n", 'A' + ataque->row, ataque->col + 1);

}

void marcarAtaque(Board *tablero, int row, int col) {
  
  char pos[3];
  snprintf(pos, sizeof(pos), "%c%d", 'A' + row, col + 1);

  // Insertar el ataque en el mapa de resultados
  map_insert(tablero->resultadosAtaques, strdup(pos), strdup("X")); // "X" indica que fue un ataque exitoso

  // Agregar el ataque a la lista de ataques realizados por el usuario
  agregarAtaque(tablero->ataquesRealizadosUsuario, row, col);
  
}



bool barcoHundido(Board *tablero, Barco *barco, Map *ataquesRealizados) {
  
  int x = barco->row;
  int y = barco->col;

  if (barco->orient == 'H') { // Barco horizontal
    
      for (int i = 0; i < barco->size; i++) {
        
          char pos[3];
          snprintf(pos, sizeof(pos), "%c%d", 'A' + x, y + i + 1);
          if (!map_search(tablero->resultadosAtaques, pos)) {
              return false; // Si alguna parte del barco no ha sido atacada, el barco no está hundido
          }
      }
  } else if (barco->orient == 'V') { // Barco vertical
      for (int i = 0; i < barco->size; i++) {
          char pos[3];
          snprintf(pos, sizeof(pos), "%c%d", 'A' + x + i, y + 1);
          if (!map_search(tablero->resultadosAtaques, pos)) {
              return false; // Si alguna parte del barco no ha sido atacada, el barco no está hundido
          }
      }
  }

  return true; // Todas las partes del barco han sido atacadas
}

bool verificarVictoria(Board tablero) {
  Barco *barco;
  list_first(tablero.barcos);
  while ((barco = list_next(tablero.barcos)) != NULL) {
      if (!barcoHundido(&tablero, barco, tablero.resultadosAtaques)) {
          return false;
      }
  }
  return true;
}

void atacarBarco(Board *tablero) {
  char fila;
  int columna;

  printf("Ingrese la posición a atacar (ej. A3 para fila A y columna 3): ");
  scanf(" %c%d", &fila, &columna);
  while (getchar() != '\n'); // Limpiar buffer de entrada

  // Validar que la fila ingresada sea una letra
  if (!isalpha(fila)) {
      printf("Fila no válida. Intente de nuevo.\n");
      return;
  }

  // Convertir fila a mayúscula y a índice basado en 0
  fila = toupper(fila);
  int row = fila - 'A';

  // Convertir columna a índice basado en 0
  int col = columna - 1;

  // Validar que las coordenadas estén dentro del tablero
  if (row < 0 || row >= tablero->height || col < 0 || col >= tablero->width) {
      printf("Ataque fuera de rango. Intente de nuevo.\n");
      return;
  }

  // Verificar el resultado del ataque
  if (tablero->tablero[row][col] == '*') {
      printf("¡Ataque exitoso! Barco golpeado en %c%d.\n", fila, columna);
      tablero->tablero[row][col] = 'X'; // Marcar el golpe en el tablero
      marcarAtaque(tablero, row, col);

      // Verificar si algún barco ha sido hundido
      Barco *barco;
      list_first(tablero->barcos);
      while ((barco = list_next(tablero->barcos)) != NULL) {
          if (barcoHundido(tablero, barco, tablero->resultadosAtaques)) {
              printf("¡Felicidades! Has hundido un barco enemigo.\n");
          }
      }
  } else if (tablero->tablero[row][col] == '~') {
      printf("Ataque fallido. No hay barco en %c%d.\n", fila, columna);
      tablero->tablero[row][col] = 'O';
      marcarAtaque(tablero, row, col);
  }

  presioneTeclaParaContinuar();
}

void colocarBarcosAleatoriamente(Board *tablero) {
  int barcosSizes[] = {1, 2, 3, 4}; // Tamaños de los barcos
  int numBarcos = sizeof(barcosSizes) / sizeof(barcosSizes[0]);

  for (int i = 0; i < numBarcos; i++) {
      Barco *barco = malloc(sizeof(Barco));
      if (barco == NULL) {
          printf("ERROR: Memoria no se pudo asignar.\n");
          return;
      }

      barco->size = barcosSizes[i];
      bool colocado = false;

      while (!colocado) {
          barco->orient = (rand() % 2) ? 'H' : 'V'; // Orientación aleatoria
          barco->row = rand() % tablero->height;
          barco->col = rand() % tablero->width;

          if (validarPosicion(tablero, barco)) {
              colocarBarco(tablero, barco);
              colocado = true;
          }
      }
  }
}
bool determinarPrimerTurno() {
  return rand() % 2 == 0; // true si el jugador inicia, false si la computadora inicia
}

void inicializarBatalla(Board *tablero) {
  Board tableroComputadora;
  inicializarTablero(&tableroComputadora, tablero->width, tablero->height);
  colocarBarcosAleatoriamente(&tableroComputadora);

  bool jugadorInicia = determinarPrimerTurno();
  printf("El %s inicia la batalla.\n", jugadorInicia ? "jugador" : "computadora");
  presioneTeclaParaContinuar();

  while (true) {
      if (jugadorInicia) {
          atacarBarco(tablero);
          if (verificarVictoria(tableroComputadora)) {
              printf("¡Felicidades! Has ganado la batalla.\n");
              break;
          }
          atacarBarcoComputadora(tablero);
          if (verificarVictoria(*tablero)) {
              printf("La computadora ha ganado la batalla.\n");
              break;
          }
      } else {
          atacarBarcoComputadora(tablero);
          if (verificarVictoria(*tablero)) {
              printf("La computadora ha ganado la batalla.\n");
              break;
          }
          atacarBarco(tablero);
          if (verificarVictoria(tableroComputadora)) {
              printf("¡Felicidades! Has ganado la batalla.\n");
              break;
          }
      }
  }
  liberarTablero(&tableroComputadora);
}

void atacarBarcoComputadora(Board *tablero) {
  int row = rand() % tablero->height;
  int col = rand() % tablero->width;

  // Repetir el ataque si la posición ya ha sido atacada
  while (tablero->tablero[row][col] == 'X' || tablero->tablero[row][col] == 'O') {
      row = rand() % tablero->height;
      col = rand() % tablero->width;
  }

  if (tablero->tablero[row][col] == '*') {
      printf("¡La computadora ha acertado en %c%d!\n", 'A' + row, col + 1);
      tablero->tablero[row][col] = 'X';
  } else {
      printf("La computadora ha fallado en %c%d.\n", 'A' + row, col + 1);
      tablero->tablero[row][col] = 'O';
  }
  presioneTeclaParaContinuar();
}


// Función para mostrar el menú de batalla
void mostrarMenuBatalla(Board *tablero) {
  char option;
  bool flotaOrganizada = false;
  while (true) {
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
              if (tablero->flotaOrganizada) {
                  printf("Iniciando la batalla...\n");
                  presioneTeclaParaContinuar();
                  inicializarBatalla(tablero); // Función para iniciar la batalla
                  return;
              } else {
                  printf("Primero debes organizar tu flota.\n");
                  presioneTeclaParaContinuar();

              }
              break;
          default:
              printf("Opción inválida. Intente de nuevo.\n");
              presioneTeclaParaContinuar();
              break;
      }
  } while (option != '2' || !flotaOrganizada);
}