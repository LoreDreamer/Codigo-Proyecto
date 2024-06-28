#include "configuration.h"  // Incluye el archivo de configuración del juego Battleship
#include "startup.h"         // Incluye el archivo de inicio del juego
#include "battle.h"          // Incluye el archivo que contiene las funciones de batalla

#include "../tdas/list.h"   
#include "../tdas/hash.h"  

#include <stdbool.h>        
#include <unistd.h>          
#include <stdio.h>         
#include <stdlib.h>          
#include <string.h>          
#include <ctype.h>           

#define MAXCOLS 10          
#define MAXROWS 10           

struct Board {

  char **tablero;           // Matriz que representa el tablero del juego
  HashMap *resultadosAtaques; // Mapa para almacenar los resultados de los ataques realizados
  int width;                // Ancho del tablero
  int height;               // Alto del tablero
  List *barcos;             // Lista de los barcos en el tablero
  bool flotaOrganizada;     // Indicador de si la flota está organizada o no

}; 

struct Barco {

  int size;                 // Tamaño del barco
  int row;                  // Fila inicial de la posición del barco
  int col;                  // Columna inicial de la posición del barco
  char orient;              // Orientación del barco ('H' para horizontal, 'V' para vertical)

};

struct Ataque {

  int row;                // Fila del ataque
  int col;                // Columna del ataque

};


//Funcion para la ubicacion aleatoria de barcos para la IA 
void colocarBarcosAleatoriamente(Board *tablero) {

  int barcosSizes[] = {1, 2, 3, 4};  // Tamaños posibles de los barcos

  int numBarcos = sizeof(barcosSizes) / sizeof(barcosSizes[0]);  // Número total de barcos

  for (int i = 0; i < numBarcos; i++) {

    Barco *barco = malloc(sizeof(Barco));  // Crear un nuevo barco

    if (barco == NULL) {

      printf("ERROR: Memoria no se pudo asignar.\n");  // Manejo de error si no se puede asignar memoria
      return;

    }

    barco->size = barcosSizes[i];  // Asignar el tamaño del barco actual
    bool colocado = false;  // Variable para verificar si se ha colocado el barco en el tablero

    while (!colocado) {

      barco->orient = (rand() % 2) ? 'H' : 'V'; // Orientación aleatoria del barco

      barco->row = rand() % tablero->height;  // Fila aleatoria para la posición del barco
      barco->col = rand() % tablero->width;   // Columna aleatoria para la posición del barco

      if (validarPosicion(tablero, barco)) {  // Verificar si la posición del barco es válida

        colocarBarco(tablero, barco);  // Colocar el barco en el tablero
        colocado = true;  // Marcar que el barco ha sido colocado correctamente

      }
    }
  }
}
//Funcion para marcar los ataques con exito en el tablero
void marcarAtaqueExitoso(Board *tablero, int row, int col) {

  char posicion[5];
  snprintf(posicion, sizeof(posicion), "%c%d", 'A' + row, col + 1);  // Formato de posición (fila, columna)

  // Insertar el ataque en el mapa de resultados con resultado "X" (ataque exitoso)
  insertMap(tablero->resultadosAtaques, posicion, "X");

}
//Funcion para marcar los ataques errados en el tablero
void marcarAtaqueFallido(Board *tablero, int row, int col) {

  char posicion[5];
  snprintf(posicion, sizeof(posicion), "%c%d", 'A' + row, col + 1);  // Formato de posición (fila, columna)

  // Insertar el ataque en el mapa de resultados con resultado "O" (ataque fallido)
  insertMap(tablero->resultadosAtaques, posicion, "O");

}

//Funcion para atacar el tablero del contrincante
void atacarBarco(Board *tablero, Board *tableroVacio) {

  char fila;
  int columna;

  printf("\nIngrese la posición a atacar (ej. A3 para fila A y columna 3): ");

  scanf(" %c%d", &fila, &columna);  // Leer la posición del ataque desde la entrada estándar
  while (getchar() != '\n');  // Limpiar el buffer de entrada

  if (!isalpha(fila)) {

    printf("Fila no válida. Intente de nuevo.\n");  // Verificar si la fila es válida
    return;

  }

  fila = toupper(fila);  // Convertir la fila a mayúscula

  int row = fila - 'A';  // Calcular la fila en formato numérico (0-indexado)
  int col = columna - 1;  // Calcular la columna en formato numérico (0-indexado)

  if (row < 0 || row >= tablero->height || col < 0 || col >= tablero->width) {

    printf("Ataque fuera de rango. Intente de nuevo.\n");  // Verificar si el ataque está dentro del rango del tablero
    return;

  }

  char posicion[5];
  snprintf(posicion, sizeof(posicion), "%c%d", 'A' + row, col + 1);  // Formato de posición (fila, columna)

  Pair *result = searchMap(tablero->resultadosAtaques, posicion);  // Buscar la posición en el mapa de resultados

  if (result != NULL) {

    printf("Ya atacaste esta posición. Intente de nuevo.\n");  // Verificar si la posición ya ha sido atacada
    sleep(2); // Pausa de 2 segundos para que el mensaje sea visible
    
    atacarBarco(tablero, tableroVacio);  // Volver a solicitar la posición de ataque

  }

  if (tablero->tablero[row][col] == '*') {

    printf("¡Ataque exitoso! Barco golpeado en %c%d.\n", fila, columna);  // Mensaje de ataque exitoso
    usleep(2000000);  // Espera de 2 segundos

    tablero->tablero[row][col] = 'X'; // Marcar el golpe en el tablero
    tableroVacio->tablero[row][col] = 'X'; // Marcar el golpe en el tablero vacío

    marcarAtaqueExitoso(tablero, row, col);  // Registrar el ataque exitoso en el mapa de resultados

  } else if (tablero->tablero[row][col] == '~') {

    printf("Ataque fallido. No hay barco en %c%d.\n", fila, columna);  // Mensaje de ataque fallido
    usleep(2000000);  // Espera de 2 segundos

    tablero->tablero[row][col] = 'O'; // Marcar el fallo en el tablero
    tableroVacio->tablero[row][col] = 'O'; // Marcar el fallo en el tablero vacío

    marcarAtaqueFallido(tablero, row, col);  // Registrar el ataque fallido en el mapa de resultados
    
  }
}
void agregarAtaque(List *lista, int row, int col) {

  struct Ataque *ataque = (struct Ataque *) malloc(sizeof(struct Ataque));
  // Crear una nueva estructura de ataque

  if (ataque == NULL) {

    printf("ERROR: No se pudo asignar memoria para el ataque.\n");  // Mensaje de error
    return;

  }

  ataque->row = row;  // Asignar la fila del ataque
  ataque->col = col;  // Asignar la columna del ataque

  list_pushBack(lista, ataque);  // Agregar el ataque a la lista

}


// Función para verificar si se ha alcanzado la condición de victoria
bool verificarVictoria(Board tablero) {

  int contadorImpactos = 0;

  // Recorrer todas las posiciones del tablero
  for (int i = 0; i < tablero.width; ++i) {
    for (int j = 0; j < tablero.height; ++j) {

      // Si se encuentra un barco impactado ('X'), incrementar el contador de impactos
      if (tablero.tablero[i][j] == 'X') {
        contadorImpactos++;

        // Si se han impactado 10 veces (todos los barcos), retornar true (victoria)
        if (contadorImpactos == 10) {
          return true;
        }
        
      }
    }
  }

  // Si no se han impactado 10 veces, retornar false (no hay victoria)
  return false;
}

// Función para que la computadora realice un ataque aleatorio al tablero del jugador
void atacarBarcoComputadora(Board *tablero) {

  int row = rand() % tablero->height;  // Generar fila aleatoria
  int col = rand() % tablero->width;   // Generar columna aleatoria

  // Repetir si la posición ya ha sido atacada ('X' o 'O')
  while (tablero->tablero[row][col] == 'X' || tablero->tablero[row][col] == 'O') { 
    
    row = rand() % tablero->height;
    col = rand() % tablero->width;
    
  }

  // Mostrar el ataque de la computadora en la consola
  // printf("Computadora ataca en tablero->tablero[%d][%d]\n", row, col);
  // usleep(2000000);  // Esperar 2 segundos antes de continuar

  // Procesar el resultado del ataque
  if (tablero->tablero[row][col] == '*') {
    
    printf("¡La computadora ha acertado en %c%d!\n", 'A' + row, col + 1); 
    usleep(2000000);  // Esperar 2 segundos antes de continuar
    tablero->tablero[row][col] = 'X';  // Marcar como acierto en el tablero
    
  } else {
    
    printf("La computadora ha fallado en %c%d.\n", 'A' + row, col + 1);
    usleep(2000000);  // Esperar 2 segundos antes de continuar
    tablero->tablero[row][col] = 'O';  // Marcar como fallo en el tablero
    
  }
}

// Función para inicializar la batalla entre el jugador y la computadora
void inicializarBatalla(Board *tablero) {

  Board tableroComputadora;  // Tablero de la computadora
  Board tableroVacio;        // Tablero vacío para la computadora

  // Inicializar los tableros
  inicializarTablero(&tableroComputadora, tablero->width, tablero->height);
  inicializarTablero(&tableroVacio, tablero->width, tablero->height);

  // Colocar los barcos aleatoriamente en el tablero de la computadora
  colocarBarcosAleatoriamente(&tableroComputadora);

  // Determinar quién inicia la batalla
  bool jugadorInicia = determinarPrimerTurno();

  // Mostrar quién inicia la batalla
  printf("\n%s inicia la batalla.\n", jugadorInicia ? "El jugador" : "La computadora");
  sleep(2);

  // Ciclo principal de la batalla
  while (true) {

    if (jugadorInicia) { // Parte el jugador

      limpiarPantalla();

      mostrarTablero(&tableroVacio, 3); // Mostrar tablero vacío y tablero del jugador
      printf("\n\n");
      mostrarTablero(tablero, 2);
      printf("\n");

      sleep(2);

      atacarBarco(&tableroComputadora, &tableroVacio); // Realizar el ataque del jugador a la computadora

      if (verificarVictoria(tableroComputadora)) { // Verificar si el jugador ha ganado la batalla
        
        printf("¡Felicidades! Has ganado la batalla.\n");
        presioneTeclaParaContinuar();
        
        break;
        
      }

      limpiarPantalla();

      mostrarTablero(&tableroVacio, 3); // Mostrar tablero vacío y tablero del jugador después del ataque
      printf("\n\n");
      mostrarTablero(tablero, 2);
      printf("\n");

      sleep(2);

      atacarBarcoComputadora(tablero); // Turno de la computadora

      if (verificarVictoria(*tablero)) { // Verificar si la computadora ha ganado la batalla
        
        printf("La computadora ha ganado la batalla.\n");
        presioneTeclaParaContinuar();
        
        break;
        
      }

    } else { // Parte la computadora

      limpiarPantalla();

      mostrarTablero(&tableroVacio, 3); // Mostrar tablero vacío y tablero del jugador
      printf("\n\n");
      mostrarTablero(tablero, 2);
      printf("\n");

      sleep(2);

      atacarBarcoComputadora(tablero); // Realizar el ataque de la computadora al jugador

      if (verificarVictoria(*tablero)) { // Verificar si la computadora ha ganado la batalla
        
        printf("La computadora ha ganado la batalla.\n");
        presioneTeclaParaContinuar();
        
        break;
        
      }

      limpiarPantalla();

      mostrarTablero(&tableroVacio, 3); // Mostrar tablero vacío y tablero del jugador después del ataque
      printf("\n\n");
      mostrarTablero(tablero, 2);
      printf("\n");

      sleep(2);
      
      atacarBarco(&tableroComputadora, &tableroVacio); // Turno del jugador

      if (verificarVictoria(tableroComputadora)) { // Verificar si el jugador ha ganado la batalla
        
        printf("¡Felicidades! Has ganado la batalla.\n");
        presioneTeclaParaContinuar();
        
        break;
        
      }
    }
  }

  // Liberar la memoria de los tableros al finalizar la batalla
  liberarTablero(&tableroComputadora);
  liberarTablero(&tableroVacio);
  
}