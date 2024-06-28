#include "tdas/list.h"
#include "tdas/hash.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAXCOLS 10
#define MAXROWS 10

// Estructura para el tablero
typedef struct {

  char **tablero;
  HashMap *resultadosAtaques;
  int width;
  int height;
  List *barcos;
  List *ataquesRealizadosUsuario; // guardar keys ataques usuario
  List *ataquesRealizadosMaquina; // guardar keys ataques maquina
  bool flotaOrganizada;
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

} NodeBarco;

typedef struct {
  int row;
  int col;
} Ataque;

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
void atacarBarco(Board *tablero, Board *tableroVacio);
void marcarAtaqueExitoso(Board *tablero, int row, int col);
void marcarAtaqueFallido(Board *tablero, int row, int col);
void agregarAtaque(List *lista, int row, int col);
bool verificarVictoria(Board tablero);       // Declaración de la función
void atacarBarcoComputadora(Board *tablero); // Declaración de la función
void inicializarBatalla(Board *tablero);

int main() {

  srand(time(NULL)); // Inicializar generador de números aleatorios

  char opcion;
  Board tablero;

  inicializarTablero(&tablero, 10, 10); // Inicializar tablero 10x10 por defecto
  limpiarPantalla();

  do {

    mostrarMenuPrincipal(); // Mostrar el menú principal
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    while (getchar() != '\n')
      ; // Limpiar buffer de entrada
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

void elegirTamanoTablero(Board *tablero) {

  int width, height;

  limpiarPantalla();

  puts("========================================\n");
  usleep(500000);
  puts("         TAMAÑO DEL TABLERO\n");
  usleep(500000);
  puts("========================================\n");
  usleep(500000);

  puts("Selecciona el tamaño del tablero.\n");
  printf("Ancho (Entre 5 y 10 espacios): ");

  scanf("%d", &width);

  while (getchar() != '\n')
    ; // Limpiar buffer de entrada

  printf("Alto (Entre 5 y 10 espacios): ");

  scanf("%d", &height);

  while (getchar() != '\n')
    ; // Limpiar buffer de entrada

  if (width < 5 || width > 10 || height < 5 || height > 10) {

    printf(
        "\nTamaño no válido. El tamaño del tablero debe estar entre 5 y 10.\n");

    fflush(stdout); // Flush the output buffer
    sleep(2);

    printf("Volviendo a la configuración de la flota...");
    fflush(stdout); // Flush the output buffer
    sleep(2);

  } else {

    liberarTablero(
        tablero); // Liberar el tablero actual antes de cambiar el tamaño
    inicializarTablero(tablero, width, height);
    printf("\nTablero actualizado a %d x %d.\n", width, height);
    sleep(2);
  }
}

void inicializarTablero(Board *tablero, int width, int height) {
    tablero->width = width;
    tablero->height = height;

    // Asignar memoria para las filas
    tablero->tablero = malloc(height * sizeof(char *));
    if (tablero->tablero == NULL) {
        printf("Error al asignar memoria para las filas del tablero.\n");
        exit(EXIT_FAILURE);
    }

    // Asignar memoria para las columnas de cada fila
    for (int i = 0; i < height; i++) {
        tablero->tablero[i] = malloc(width * sizeof(char));
        if (tablero->tablero[i] == NULL) {
            printf("Error al asignar memoria para las columnas del tablero.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Inicializar el tablero con un valor predeterminado, como '~'
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tablero->tablero[i][j] = '~';
        }
    }

    tablero->resultadosAtaques = createMap(height * width); // Crear mapa ordenado
    tablero->barcos = list_create();
    tablero->ataquesRealizadosMaquina = list_create();
    tablero->ataquesRealizadosUsuario = list_create();
    tablero->flotaOrganizada = false;
}




void liberarTablero(Board *tablero) {
    for (int i = 0; i < tablero->height; i++) {
        free(tablero->tablero[i]);
    }
    free(tablero->tablero);

    // Liberar otros recursos si es necesario
    map_clean(tablero->resultadosAtaques);
    list_clean(tablero->barcos);
    list_clean(tablero->ataquesRealizadosMaquina);
    list_clean(tablero->ataquesRealizadosUsuario);
}
void agregarAtaque(List *lista, int row, int col) {
  Ataque *ataque = (Ataque *)malloc(sizeof(Ataque));
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
  while ((ataque = list_next(lista)) != NULL) {
    printf("Ataque en posición: %c%d\n", 'A' + ataque->row, ataque->col + 1);
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

  puts("========================================\n");
  usleep(500000);
  puts("    COLOCANDO BARCOS EN SUS POSICIONES\n");
  usleep(500000);
  puts("========================================\n");
  usleep(500000);

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
      if (tablero->tablero[i][j] == '*' || tablero->tablero[i][j] == '#' ||
          tablero->tablero[i][j] == 'O' || tablero->tablero[i][j] == 'X') {
        printf(" %c ",
               tablero->tablero[i][j]); // Mostrar solo resultados de ataques
      } else {
        printf(" ~ "); // Espacios no atacados
      }
    }
    printf("\n");
  }

  printf("\nAtaques realizados:\n");
  puts("========================================");
  recorrerListaAtaques(tablero->ataquesRealizadosUsuario);
  puts("========================================");
  sleep(2);
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

    while (getchar() != '\n')
      ; // Limpiar buffer de entrada

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
    barco->size = i + 1;      // Tamaño del barco

    if (i < 4) {

      printf("Seleccione la orientación (H - horizontal, V - vertical): ");
      scanf(" %c", &barco->orient);

      while (getchar() != '\n')
        ; // Limpiar buffer de entrada

      // Validar orientación
      if (barco->orient != 'H' && barco->orient != 'V') {

        printf("Orientación no válida. Intente de nuevo.\n");
        sleep(2);
        free(barco);
        i--;

        continue;
      }
      // Validar espacio disponible
      if (barco->orient == 'H' &&
          barco->col + barco->size > tablero->width) { // Corrected line
        printf("No hay suficiente espacio hacia la derecha para este barco. "
               "Intente de nuevo.\n");
        sleep(2);
        free(barco);
        i--;

        continue;
      }
      if (barco->orient == 'V' && barco->row + barco->size > tablero->height) {
        printf("No hay suficiente espacio hacia abajo para este barco. Intente "
               "de nuevo.\n");
        sleep(2);
        free(barco);
        i--;

        continue;
      }
      // Validar si la posición está ocupada
      if (!validarPosicion(tablero, barco)) {
        printf("La posición seleccionada está ocupada por otro barco. Intente "
               "de nuevo.\n");
        sleep(2);
        free(barco);
        i--;
        continue;
      }

      colocarBarco(tablero, barco);
    }
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

void marcarAtaqueExitoso(Board *tablero, int row, int col) {
  char posicion[5];
  snprintf(posicion, sizeof(posicion), "%c%d", 'A' + row, col + 1);

  // Insertar el ataque en el mapa de resultados
  insertMap(tablero->resultadosAtaques, posicion, "X"); // "X" indica que fue un ataque exitoso
  // Agregar el ataque a la lista de ataques realizados por el usuario
  agregarAtaque(tablero->ataquesRealizadosUsuario, row, col);
}

void marcarAtaqueFallido(Board *tablero, int row, int col) {
  char posicion[5];
  snprintf(posicion, sizeof(posicion), "%c%d", 'A' + row, col + 1);

  // Insertar el ataque en el mapa de resultados
  // Insertar el ataque en el mapa de resultados
  insertMap(tablero->resultadosAtaques, posicion, "O"); // "O" indica que fue un ataque fallido
  // Agregar el ataque a la lista de ataques realizados por el usuario
  agregarAtaque(tablero->ataquesRealizadosUsuario, row, col);
}

void atacarBarco(Board *tablero, Board *tableroVacio) {
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

    // Generar la clave de la posición
    char posicion[5];
    snprintf(posicion, sizeof(posicion), "%c%d", 'A' + row, col + 1);

    // Validar que la coordenada no haya sido atacada
    Pair *result = searchMap(tablero->resultadosAtaques, posicion);
    if (result != NULL) {
        printf("Ya atacaste esta posición. Intente de nuevo.\n");
        sleep(2); // Pausa de 2 segundos para que el mensaje sea visible
        return;
    }

    // Realizar el ataque
    if (tablero->tablero[row][col] == '*') {
        printf("¡Ataque exitoso! Barco golpeado en %c%d.\n", fila, columna);
        usleep(2000000);
        printf("Marcando golpe en tablero->tablero[%d][%d] y tableroVacio->tablero[%d][%d]\n", row, col, row, col);
        usleep(2000000);
        tablero->tablero[row][col] = 'X'; // Marcar el golpe en el tablero
        tableroVacio->tablero[row][col] = 'X'; // Marcar el golpe en el tablero vacío
        marcarAtaqueExitoso(tablero, row, col); 
    } else if (tablero->tablero[row][col] == '~') {
        printf("Ataque fallido. No hay barco en %c%d.\n", fila, columna);
        usleep(2000000);
        printf("Marcando fallo en tablero->tablero[%d][%d] y tableroVacio->tablero[%d][%d]\n", row, col, row, col);
        usleep(2000000);
        tablero->tablero[row][col] = 'O'; // Marcar el fallo en el tablero
        tableroVacio->tablero[row][col] = 'O'; // Marcar el fallo en el tablero vacío
        marcarAtaqueFallido(tablero, row, col);
    }

    mostrarTablero(tableroVacio);
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
  return rand() % 2 ==
         0; // true si el jugador inicia, false si la computadora inicia
}

void inicializarBatalla(Board *tablero) {
  Board tableroComputadora;
  Board tableroVacio;
  inicializarTablero(&tableroComputadora, tablero->width, tablero->height);
  inicializarTablero(&tableroVacio, tablero->width, tablero->height);
  colocarBarcosAleatoriamente(&tableroComputadora);

  bool jugadorInicia = determinarPrimerTurno();
  printf("El %s inicia la batalla.\n", jugadorInicia ? "jugador" : "computadora");

  while (true) {
    if (jugadorInicia) {
      atacarBarco(&tableroComputadora, &tableroVacio);
      if (verificarVictoria(tableroComputadora)) {
        printf("¡Felicidades! Has ganado la batalla.\n");
        presioneTeclaParaContinuar();
        break;
      }
      atacarBarcoComputadora(tablero);
      if (verificarVictoria(*tablero)) {
        printf("La computadora ha ganado la batalla.\n");
        presioneTeclaParaContinuar();
        break;
      }
    } else {
      atacarBarcoComputadora(tablero);
      if (verificarVictoria(*tablero)) {
        printf("La computadora ha ganado la batalla.\n");
        presioneTeclaParaContinuar();
        break;
      }
      atacarBarco(&tableroComputadora, &tableroVacio);
      if (verificarVictoria(tableroComputadora)) {
        printf("¡Felicidades! Has ganado la batalla.\n");
        presioneTeclaParaContinuar();
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

    printf("Computadora ataca en tablero->tablero[%d][%d]\n", row, col);
    usleep(2000000);
    if (tablero->tablero[row][col] == '*') {
        printf("¡La computadora ha acertado en %c%d!\n", 'A' + row, col + 1); 
        usleep(2000000);
        tablero->tablero[row][col] = 'X';
    } else {
        printf("La computadora ha fallado en %c%d.\n", 'A' + row, col + 1);
        usleep(2000000);
        tablero->tablero[row][col] = 'O';
    }
    mostrarTablero(tablero); // Mostrar el tablero actualizado
}


bool verificarVictoria(Board tablero) {
    int contadorImpactos = 0;

    for (int i = 0; i < tablero.width; ++i) {
      for (int j = 0; j < tablero.height; ++j) {
        if (tablero.tablero[i][j] == 'X') {
          contadorImpactos++;
          if (contadorImpactos == 10) {
            return true;
          }
        }
      }
    }
  return false;
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
  while ((c = getchar()) != '\n' && c != EOF)
    ;
  getchar(); // Capturar el Enter
}
// Función para mostrar el menú principal
void mostrarMenuPrincipal() {

  limpiarPantalla();
  puts("========================================\n");
  usleep(500000);
  puts("             BATTLESHIP\n");
  usleep(500000);
  puts("========================================\n");
  usleep(500000);
  puts("1) Iniciar Batalla");
  usleep(500000);
  puts("2) Salir\n");
  usleep(500000);
}

// Función para mostrar el menú de batalla
void mostrarMenuBatalla(Board *tablero) {

  char option;
  bool flotaOrganizada = false;

  while (true) {

    limpiarPantalla();

    puts("========================================\n");
    usleep(500000);
    puts("     CONFIGURACIÓN DE LA FLOTA\n");
    usleep(500000);
    puts("========================================\n");
    usleep(500000);

    puts("0) Elegir tamaño tablero");
    usleep(500000);
    puts("1) Organizar tu flota");
    usleep(500000);
    puts("2) Iniciar Batalla\n");
    usleep(500000);

    fflush(stdout);

    printf("Elige una opción: ");
    scanf(" %c", &option);

    while (getchar() != '\n' && getchar() != EOF)
      ; // Limpiar buffer de entrada

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
        sleep(2);
        inicializarBatalla(tablero); // Función para iniciar la batalla
        return;

      } else {

        printf("Primero debes organizar tu flota.\n");
        sleep(2);
      }
      break;

    default:

      printf("Opción inválida. Intente de nuevo.\n");
      sleep(2);
      break;
    }
  }
  while (option != '2' || !flotaOrganizada)
    ;
}