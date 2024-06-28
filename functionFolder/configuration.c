#include "configuration.h"
#include "startup.h"
#include "battle.h"

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

  char **tablero;

  HashMap *resultadosAtaques;

  int width;
  int height;

  List *barcos;
  bool flotaOrganizada;

};

struct Barco {

    int size;     
    int row;     // Fila inicial (A = 0, B = 1, ..., Z = 25)
    int col;     // Columna inicial (0, 1, 2, ...)
    char orient; // 'H' para horizontal, 'V' para vertical

};
//Funcion para colocar un barco en el tablero
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
//Funcion para sacar un barco del tablero si es necesario
void sacarBarcoTablero(Board *tablero, Barco *barco) {

  int x = barco->row;          // Fila inicial del barco
  int y = barco->col;          // Columna inicial del barco

  if (barco->orient == 'H') {  // Si el barco está orientado horizontalmente

    for (int i = 0; i < barco->size; i++) {
      tablero->tablero[x][y + i] = '~';  // Eliminar parte del barco del tablero
    }

  } else {                     // Si el barco está orientado verticalmente

    for (int i = 0; i < barco->size; i++) {
      tablero->tablero[x + i][y] = '~';  // Eliminar parte del barco del tablero
    }

  }
}

//Funcion para cambiar de posicion el barco que nosotros queramos
void cambioDePosicion(Board *tablero) {

  char opcion; // Variable para almacenar la opción del usuario
  int size; // Variable para almacenar el tamaño del barco a cambiar de posición

  do {

    limpiarPantalla(); // Limpiar la pantalla
    mostrarTablero(tablero, 2); // Mostrar el tablero con una opción de formato específica
    sleep(2); // Pausar la ejecución durante 2 segundos

    printf("\n¿Te gustaría cambiar la posición de algún barco? (S/N): ");
    scanf(" %c", &opcion); // Leer la opción del usuario

    while (getchar() != '\n'); // Limpiar el buffer de entrada

    if (opcion == 'N' || opcion == 'n') // Si el usuario no desea cambiar la posición
      return; // Salir de la función

    printf("Ingrese el tamaño de barco a cambiar de posición (1-4): ");

    while (size < 1 || size > 4) { // Validar que el tamaño del barco esté entre 1 y 4

      scanf("%d", &size); // Leer el tamaño del barco

      if (size < 1 || size > 4) // Si el tamaño no es válido
        printf("Valor inválido, reingrese un valor: "); // Solicitar un nuevo valor

    }

    while (getchar() != '\n'); // Limpiar el buffer de entrada

    Barco *barco = list_first(tablero->barcos); // Obtener el primer barco en la lista

    while (barco != NULL) { // Iterar sobre la lista de barcos

      if (barco->size == size) { // Si se encuentra un barco del tamaño especificado
        break; // Salir del bucle
      }

      barco = list_next(tablero->barcos); // Obtener el siguiente barco en la lista
    }

    if (barco == NULL) { // Si no se encuentra un barco del tamaño especificado

      printf("No se encontró un barco de ese tamaño. Intente de nuevo.\n");
      continue; // Volver al inicio del bucle

    }

    // Guardar la posición original del barco
    int original_row = barco->row;
    int original_col = barco->col;
    char original_orient = barco->orient;

    // Sacar el barco del tablero
    sacarBarcoTablero(tablero, barco);

    char fila; // Variable para almacenar la nueva fila
    int columna; // Variable para almacenar la nueva columna

    printf("Ingrese la nueva posición inicial (ej. A3 para fila A y columna 3): ");
    scanf(" %c%d", &fila, &columna); // Leer la nueva posición inicial

    while (getchar() != '\n'); // Limpiar el buffer de entrada

    if (isalpha(fila)) { // Si la fila es una letra

      fila = toupper(fila); // Convertir la letra a mayúscula
      barco->row = fila - 'A'; // Calcular la fila en base a la letra

      if (barco->row < 0 || barco->row >= tablero->height) { // Verificar si la fila es válida

        printf("Fila no válida. Intente de nuevo.\n");

        // Restaurar la posición original del barco
        barco->row = original_row;
        barco->col = original_col;
        barco->orient = original_orient;

        colocarBarco(tablero, barco); // Colocar el barco en la posición original
        sleep(2); // Pausar la ejecución durante 2 segundos

        continue; // Volver al inicio del bucle

      }

    } else { // Si la fila no es una letra

      printf("Fila no válida. Intente de nuevo.\n");

      // Restaurar la posición original del barco
      barco->row = original_row;
      barco->col = original_col;
      barco->orient = original_orient;

      colocarBarco(tablero, barco); // Colocar el barco en la posición original
      sleep(2); // Pausar la ejecución durante 2 segundos

      continue; // Volver al inicio del bucle
    }

    barco->col = columna - 1; // Calcular la columna en base al número ingresado

    printf("Seleccione la nueva orientación (H - horizontal, V - vertical): ");
    scanf(" %c", &barco->orient); // Leer la nueva orientación del barco

    while (getchar() != '\n'); // Limpiar el buffer de entrada

    if (barco->orient != 'H' && barco->orient != 'V') { // Verificar si la orientación es válida

      printf("Orientación no válida. Intente de nuevo.\n");

      // Restaurar la posición original del barco
      barco->row = original_row;
      barco->col = original_col;
      barco->orient = original_orient;

      colocarBarco(tablero, barco); // Colocar el barco en la posición original
      sleep(2); // Pausar la ejecución durante 2 segundos

      continue; // Volver al inicio del bucle

    }

    if (barco->orient == 'H' && barco->col + barco->size > tablero->width) { // Verificar si hay suficiente espacio para una colocación horizontal

      printf("No hay suficiente espacio hacia la derecha para este barco. Intente de nuevo.\n");

      // Restaurar la posición original del barco
      barco->row = original_row;
      barco->col = original_col;
      barco->orient = original_orient;

      colocarBarco(tablero, barco); // Colocar el barco en la posición original
      sleep(2); // Pausar la ejecución durante 2 segundos

      continue; // Volver al inicio del bucle

    }

    if (barco->orient == 'V' && barco->row + barco->size > tablero->height) { // Verificar si hay suficiente espacio para una colocación vertical

      printf("No hay suficiente espacio hacia abajo para este barco. Intente de nuevo.\n");

      // Restaurar la posición original del barco
      barco->row = original_row;
      barco->col = original_col;
      barco->orient = original_orient;

      colocarBarco(tablero, barco); // Colocar el barco en la posición original
      sleep(2); // Pausar la ejecución durante 2 segundos

      continue; // Volver al inicio del bucle

    }

    if (!validarPosicion(tablero, barco)) { // Verificar si la nueva posición está ocupada por otro barco

      printf("La posición seleccionada está ocupada por otro barco. Intente de nuevo.\n");

      // Restaurar la posición original del barco
      barco->row = original_row;
      barco->col = original_col;
      barco->orient = original_orient;

      colocarBarco(tablero, barco); // Colocar el barco en la posición original
      sleep(2); // Pausar la ejecución durante 2 segundos

      continue; // Volver al inicio del bucle

    }

    colocarBarco(tablero, barco); // Colocar el barco en la nueva posición

  } while (opcion != 'N' && opcion != 'n'); // Repetir mientras el usuario no elija salir
}

//Funcion para organizar la flota de barcos en nuestro tablero
void organizarFlota(Board *tablero) {

  repoblarTablero(tablero); // Repoblar el tablero

  for (int i = 0; i < 4; i++) { // Iterar sobre los tamaños de los barcos de 1 a 4

    limpiarPantalla(); // Limpiar la pantalla
    mostrarTablero(tablero, 1); // Mostrar el tablero con una opción de formato específica

    printf("\nOrganizando la flota - Barco de tamaño %d:\n", i + 1);

    Barco *barco = malloc(sizeof(Barco)); // Asignar memoria para un nuevo barco

    if (barco == NULL) { // Verificar si la asignación de memoria falló

      printf("ERROR: Memoria no se pudo asignar.\n");
      return;
    }

    char fila; // Variable para almacenar la fila inicial
    int columna; // Variable para almacenar la columna inicial

    printf("Ingrese la posición inicial (ej. A3 para fila A y columna 3): ");
    scanf(" %c%d", &fila, &columna); // Leer la posición inicial

    while (getchar() != '\n'); // Limpiar el buffer de entrada

    if (isalpha(fila)) { // Verificar si la fila es una letra

      fila = toupper(fila); // Convertir la letra a mayúscula
      barco->row = fila - 'A'; // Calcular la fila en base a la letra

      if (barco->row < 0 || barco->row >= tablero->height) { // Verificar si la fila es válida

        printf("Fila no válida. Intente de nuevo.\n");
        sleep(2);

        free(barco); // Liberar la memoria del barco
        i--; // Decrementar el contador para volver a intentar

        continue; // Volver al inicio del bucle
      }

    } else { // Si la fila no es una letra

      printf("Fila no válida. Intente de nuevo.\n");
      sleep(2);

      free(barco); // Liberar la memoria del barco
      i--; // Decrementar el contador para volver a intentar

      continue; // Volver al inicio del bucle
    }

    barco->col = columna - 1; // Calcular la columna en base al número ingresado
    barco->size = i + 1; // Asignar el tamaño del barco

    printf("Seleccione la orientación (H - horizontal, V - vertical): ");
    scanf(" %c", &barco->orient); // Leer la orientación del barco

    while (getchar() != '\n'); // Limpiar el buffer de entrada

    if (barco->orient != 'H' && barco->orient != 'V') { // Verificar si la orientación es válida

      printf("Orientación no válida. Intente de nuevo.\n");
      sleep(2);

      free(barco); // Liberar la memoria del barco
      i--; // Decrementar el contador para volver a intentar

      continue; // Volver al inicio del bucle
    }

    if (barco->orient == 'H' && barco->col + barco->size > tablero->width) { // Verificar si hay suficiente espacio para una colocación horizontal

      printf("No hay suficiente espacio hacia la derecha para este barco. Intente de nuevo.\n");
      sleep(2);

      free(barco); // Liberar la memoria del barco
      i--; // Decrementar el contador para volver a intentar

      continue; // Volver al inicio del bucle
    }

    if (barco->orient == 'V' && barco->row + barco->size > tablero->height) { // Verificar si hay suficiente espacio para una colocación vertical

      printf("No hay suficiente espacio hacia abajo para este barco. Intente de nuevo.\n");
      sleep(2);

      free(barco); // Liberar la memoria del barco
      i--; // Decrementar el contador para volver a intentar

      continue; // Volver al inicio del bucle
    }

    if (!validarPosicion(tablero, barco)) { // Verificar si la nueva posición está ocupada por otro barco

      printf("La posición seleccionada está ocupada por otro barco. Intente de nuevo.\n");
      sleep(2);

      free(barco); // Liberar la memoria del barco
      i--; // Decrementar el contador para volver a intentar

      continue; // Volver al inicio del bucle
    }

    colocarBarco(tablero, barco); // Colocar el barco en la nueva posición
  }

  cambioDePosicion(tablero); // Permitir al usuario cambiar la posición de los barcos

  limpiarPantalla(); // Limpiar la pantalla
  mostrarTablero(tablero, 2); // Mostrar el tablero con una opción de formato específica

  printf("\nFlota organizada exitosamente.\n");

  fflush(stdout); // Vaciar el buffer de salida
  sleep(2);

  printf("Volviendo a configuración de flota...\n");

  fflush(stdout); // Vaciar el buffer de salida
  sleep(2);

  tablero->flotaOrganizada = true; // Marcar la flota como organizada
}
//Funcion para elegir el tamaño deseado del tablero
void elegirTamanoTablero(Board *tablero) {

  int width, height; // Variables para almacenar el ancho y alto del tablero

  limpiarPantalla(); // Limpiar la pantalla

  puts("========================================\n"); usleep(350000);
  puts("         TAMAÑO DEL TABLERO\n"); usleep(350000);
  puts("========================================\n"); usleep(350000);

  puts("Selecciona el tamaño del tablero.\n");

  printf("Ancho (Entre 5 y 10 espacios): ");
  scanf("%d", &width); // Leer el ancho del tablero

  while (getchar() != '\n'); // Limpiar el buffer de entrada

  printf("Alto (Entre 5 y 10 espacios): ");
  scanf("%d", &height); // Leer el alto del tablero

  while (getchar() != '\n'); // Limpiar el buffer de entrada

  if (width < 5 || width > 10 || height < 5 || height > 10) { // Verificar si el tamaño es válido

    printf("\nTamaño no válido. El tamaño del tablero debe estar entre 5 y 10.\n");

    fflush(stdout); // Vaciar el buffer de salida
    sleep(2);

    printf("Volviendo a la configuración de la flota...");
    fflush(stdout); // Vaciar el buffer de salida
    sleep(2);

  } else {
    
    liberarTablero(tablero); // Liberar el tablero actual antes de cambiar el tamaño
    inicializarTablero(tablero, width, height); // Inicializar el nuevo tablero con el tamaño especificado
    
    printf("\nTablero actualizado a %d x %d.", width, height);
    sleep(2);
  }
}
//Funcion para validar si la posicion ingresada por el usuario esta ocupada por otro barco
bool validarPosicion(Board *tablero, Barco *barco) {

  int x = barco->row;
  int y = barco->col;

  if (barco->orient == 'H') { // Colocación horizontal

    for (int i = 0; i < barco->size; i++) {

      if (y + i >= tablero->width) { // Verificar si el barco se sale de los límites
        return false;
      }

      if (tablero->tablero[x][y + i] != '~') { // Verificar si la posición está ocupada
        return false;
      }
    }

  } else if (barco->orient == 'V') { // Colocación vertical

    for (int i = 0; i < barco->size; i++) {

      if (x + i >= tablero->height) { // Verificar si el barco se sale de los límites
        return false;
      }

      if (tablero->tablero[x + i][y] != '~') { // Verificar si la posición está ocupada
        return false;
      }
    }
  }

  return true; // La posición está disponible
}
void mostrarMenuConfiguracion(Board *tablero) {

  char option; // Variable para almacenar la opción del usuario
  bool flotaOrganizada = false; // Variable para verificar si la flota ha sido organizada

  while (true) { // Bucle infinito para mostrar el menú hasta que se inicie la batalla

    limpiarPantalla(); // Limpiar la pantalla

    // Mostrar el menú de configuración de la flota
    puts("========================================\n"); usleep(350000);
    puts("     CONFIGURACIÓN DE LA FLOTA\n"); usleep(350000);
    puts("========================================\n"); usleep(350000);

    puts("0) Elegir tamaño tablero"); usleep(350000);
    puts("1) Organizar tu flota"); usleep(350000);
    puts("2) Iniciar Batalla\n"); usleep(350000);

    fflush(stdout); // Vaciar el buffer de salida

    printf("Elige una opción: ");
    scanf(" %c", &option); // Leer la opción del usuario

    while (getchar() != '\n' && getchar() != EOF); // Limpiar el buffer de entrada

    switch (option) { // Manejar las diferentes opciones del menú

    case '0':
      
      elegirTamanoTablero(tablero); // Llamar a la función para elegir el tamaño del tablero
      break;

    case '1':

      if (tablero->flotaOrganizada) { // Se anuncia que se borra la organización anterior si es que ya se            organizó

        printf("\nBorrando organización anterior...\n\n");
        fflush(stdout); // Vaciar el buffer de salida

        sleep(2);
        
      }
      
      organizarFlota(tablero); // Llamar a la función para organizar la flota
      
      break;

    case '2':
      
      if (tablero->flotaOrganizada) { // Verificar si la flota ha sido organizada
        
        printf("\nIniciando la batalla...\n");
        sleep(2);
        
        inicializarBatalla(tablero); // Función para iniciar la batalla
        
        return; // Salir del bucle y de la función
        
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

  while (option != '2' || !flotaOrganizada); // Mantener el bucle hasta que se elija iniciar la batalla y la flota esté organizada
  
}