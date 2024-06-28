#include "others.h"
#include "../tdas/list.h"
#include "../tdas/hash.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

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



void limpiarPantalla() {
  
#ifdef _WIN32
  
    system("cls");
#else
  
    system("clear");
#endif
  
    fflush(stdout); // Limpiar el buffer de salida
}

void presioneTeclaParaContinuar() {
  
    int c;
  
    printf("Presione Enter para continuar...");
  
    while ((c = getchar()) != '\n' && c != EOF);
  
    getchar(); // Capturar el Enter
  
}

void mostrarMenuPrincipal() {

  limpiarPantalla();
    
  puts("========================================\n"); usleep(500000);
  puts("             BATTLESHIP\n"); usleep(500000);
  puts("========================================\n"); usleep(500000);
    
  puts("1) Iniciar Batalla"); usleep(500000);
  puts("2) Salir\n"); usleep(500000);

}


void inicializarTablero(Board *tablero, int width, int height) {

    tablero->width = width;
    tablero->height = height;

    for (int i = 0; i < tablero->height; i++) {
        for (int j = 0; j < tablero->width; j++) {

            tablero->tablero[i][j] = '~'; // Initialize with a default value like '~'

        }
    }

    tablero->resultadosAtaques = sorted_map_create(NULL); // Crear mapa ordenado

    tablero->barcos = list_create();
    tablero->ataquesRealizadosMaquina = list_create();
    tablero->ataquesRealizadosUsuario = list_create();

    tablero->flotaOrganizada = false;

}

void liberarTablero(Board *tablero) {

    map_clean(tablero->resultadosAtaques);
    list_clean(tablero->barcos);
    list_clean(tablero->ataquesRealizadosUsuario);
    list_clean(tablero->ataquesRealizadosMaquina);

}

void mostrarTablero(Board *tablero) {
    
    limpiarPantalla();

    puts("========================================\n"); usleep(500000);
    puts("    COLOCANDO BARCOS EN SUS POSICIONES\n"); usleep(500000);
    puts("========================================\n"); usleep(500000);

    printf("    "); // Espacio para alinear con las columnas

    for (int i = 0; i < tablero->width; i++)
        printf("%2d ", i + 1);
    
    printf("\n");

  // Mostrar letras de las filas y contenido del tablero
  for (int i = 0; i < tablero->height; i++) {
      
    printf("%c   ", 'A' + i); // Letras de las filas

    for (int j = 0; j < tablero->width; j++) {
        
      if (tablero->tablero[i][j] == '*' || tablero->tablero[i][j] == '#' || tablero->tablero[i][j] == 'O' || tablero->tablero[i][j] == 'X') {
          
        printf(" %c ",tablero->tablero[i][j]); // Mostrar solo resultados de ataques
          
      } else {
          
        printf(" ~ "); // Espacios no atacados
        
      }
    }
      
    printf("\n");
  }

  printf("\nAtaques realizados:\n");
    
  puts("========================================");
  puts("========================================");
    
  sleep(2);
    
}