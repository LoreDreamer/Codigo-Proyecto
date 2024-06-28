#include "startup.h"

#include "../tdas/list.h"
#include "../tdas/hash.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
struct Board {

  char **tablero;               // Matriz del tablero
  HashMap *resultadosAtaques;   // Mapa para los resultados de los ataques
  int width;                    // Ancho del tablero
  int height;                   // Alto del tablero
  List *barcos;                 // Lista de barcos en el tablero
  bool flotaOrganizada;         // Indicador de si la flota está organizada

};

// Función para limpiar la pantalla
void limpiarPantalla() {

  #ifdef _WIN32
  system("cls");  // Comando para Windows

  #else
  system("clear"); // Comando para otros sistemas operativos

  #endif
  fflush(stdout); // Limpiar el buffer de salida

}

// Función para pausar y esperar que el usuario presione Enter
void presioneTeclaParaContinuar() {

  printf("Presione Enter para continuar...");

  getchar(); // Capturar el Enter
  getchar(); // Capturar el Enter nuevamente para asegurarse

}

// Función para mostrar el menú principal
void mostrarMenuPrincipal() {

  limpiarPantalla(); // Limpiar la pantalla antes de mostrar el menú

  puts("========================================\n"); usleep(350000);
  puts("             BATTLESHIP\n"); usleep(350000);
  puts("========================================\n"); usleep(350000);

  puts("1) Iniciar Batalla"); usleep(350000);
  puts("2) Instrucciones"); usleep(350000);
  puts("3) Salir\n"); usleep(350000);

}

// Función para llenar el tablero con un carácter predeterminado
void repoblarTablero(Board *tablero) {

  for (int i = 0; i < tablero->height; i++) {

    for (int j = 0; j < tablero->width; j++) {

      tablero->tablero[i][j] = '~'; // Llenar con '~' que indica agua

    }
  }
}

// Función para inicializar el tablero con el ancho y alto especificados
void inicializarTablero(Board *tablero, int width, int height) {

  tablero->width = width;   // Asignar el ancho del tablero
  tablero->height = height; // Asignar el alto del tablero

  // Asignar memoria para las filas del tablero
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

      tablero->tablero[i][j] = '~'; // Llenar con '~' que indica agua

    }
  }

  // Crear un mapa para los resultados de los ataques
  tablero->resultadosAtaques = createMap(height * width); // Crear mapa ordenado

  // Crear la lista de barcos
  tablero->barcos = list_create();

  // Inicializar el indicador de flota organizada
  tablero->flotaOrganizada = false;

}

// Función para mostrar el tablero con un encabezado específico según el parámetro
void mostrarTablero(Board *tablero, int parametro) {

  puts("========================================\n"); usleep(350000);

  // Mostrar encabezado según el valor del parámetro
  switch (parametro) {

    case 1:
      puts("      CONFIGURACIÓN DE LA FLOTA\n"); usleep(350000);
      break;

    case 2:
      puts("            TU TABLERO\n"); usleep(350000);
      break;

    case 3:
      puts("        TABLERO ENEMIGO\n"); usleep(350000);
      break;

  }

  puts("========================================\n"); usleep(350000);
  printf("    "); // Espacio para alinear con las columnas

  // Mostrar números de las columnas
  for (int i = 0; i < tablero->width; i++) { 
    printf("%2d ", i + 1);
  }

  puts("\n");

  // Mostrar letras de las filas y contenido del tablero
  for (int i = 0; i < tablero->height; i++) {
    printf("%c   ", 'A' + i); // Letras de las filas

    for (int j = 0; j < tablero->width; j++) {

      // Mostrar solo resultados de ataques
      if (tablero->tablero[i][j] == '*' || tablero->tablero[i][j] == '#' ||
          tablero->tablero[i][j] == 'O' || tablero->tablero[i][j] == 'X') {

        printf(" %c ", tablero->tablero[i][j]);

      } else {

        printf(" ~ "); // Espacios no atacados

      }
    }

    printf("\n");

  }
}

// Función para liberar la memoria asignada al tablero
void liberarTablero(Board *tablero) {

  for (int i = 0; i < tablero->height; i++) {
    free(tablero->tablero[i]);
  }

  free(tablero->tablero);

  // Liberar otros recursos si es necesario
  map_clean(tablero->resultadosAtaques);
  list_clean(tablero->barcos);

}

// Función para determinar aleatoriamente quién toma el primer turno
bool determinarPrimerTurno() { return rand() % 2 == 0; }
// true si el jugador inicia, false si la computadora inicia

// Función para mostrar las instrucciones del juego
void instrucciones() {

  limpiarPantalla();

  puts("========================================\n"); usleep(350000);
  puts("            INSTRUCCIONES\n"); usleep(350000);
  puts("========================================\n"); usleep(350000);

  puts("¡Bienvenido a Battleship, nuevo jugador!"); 
  sleep(3);

  puts("Debes de tener varias preguntas con respecto al juego, ¿cierto?");   
  sleep(3);

  puts("¡Pues aquí tienes las instrucciones!\n");
  sleep(3);

  printf("El jugador competirá contra una computadora en una batalla naval en la cual se contará con un tablero y diferentes barcos que deberá colocar en esta.\n\n");
  sleep(7);

  printf("Antes de empezar la partida, el jugador tiene la posibilidad de cambiar el tamaño de su tablero y también tendrá la opción de configurar su flota previo a la batalla.\n\n");
  sleep(7);

  printf("Durante la fase de batalla, el jugador debe hacer uso de coordenadas para bombardear el sector enemigo y destruir la flota enemiga. Por otro lado, la computadora intentará hacer lo mismo con el jugador.\n\n");
  sleep(7);

  printf("Simbología del tablero:\n\n");
  sleep(1);

  printf("~: Mar\n");
  sleep(1);
  printf("- El mar es un sector vacío que se muestra previo a que el sector sea atacado.\n\n");
  sleep(3);

  printf("X: Disparo acertado\n");
  sleep(1);
  printf("- El disparo acertado indica que se le ha acertado a un objetivo\n\n");
  sleep(3);

  printf("O: Disparo fallido\n");
  sleep(1);
  printf("- El disparo fallado indica que no habían objetivos en el área de ataque\n\n");
  sleep(3);

  printf("Si uno de los dos bandos pierde su flotilla completa, el juego terminará con el jugador sobreviviente siendo el ganador\n\n");
  sleep(7);

  printf("Como dato adicional, el jugador no sabrá si ha hundido un barco enemigo o no, pero sí si ha hundido todos los barcos enemigos.\n\n");
  sleep(7);

  printf("¡Buena suerte, jugador!\n\n");
  sleep(3);

  presioneTeclaParaContinuar();

}
