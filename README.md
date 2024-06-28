# BATTLESHIP

En este *repl* se puede encontrar un código que implementa el clásico juego de mesa Batalla Naval, adaptado para ser jugado en la consola. El juego permite al usuario jugar contra la computadora, colocando sus barcos, realizando ataques a tabero enemigo, y verificando quién gana a través de un menú interactivo. 

## Compilación del código.
Para ejecutar el main primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c main.c -Wno-unused-result -o main
````

Y luego ejecutar:
````
./main
````


## Listado de opciones del primer menú del juego con sus ejemplos respectivos.

### El menú interactivo de este código es el siguiente:

````
========================================

             BATTLESHIP

========================================

1) Iniciar Batalla
2) Instrucciones
3) Salir
````

A continuación se explica el modo de uso y ejemplos de cada opción del menú mostrado anteriormente.



#### **Opción 1:**

-La opción 1 'Iniciar Batalla', muestra el siguiente menú interactivo:

````
========================================

     CONFIGURACIÓN DE LA FLOTA

========================================

0) Elegir tamaño tablero
1) Organizar tu flota
2) Iniciar Batalla

Elige una opción: 
````


#### **Opción 2:**

-La opción 2 'Instrucciones', muestra las instrucciones del juego, con ejemplos para hacer uso correcto de las opciones y funciones del programa, además de señalar la simbología utilizada para cada caso del juego.

````
========================================

            INSTRUCCIONES

========================================

¡Bienvenido a Battleship, nuevo jugador!
Debes de tener varias preguntas con respecto al juego, ¿cierto?
¡Pues aquí tienes las instrucciones!

El jugador competirá contra una computadora en una batalla naval en la cual se contará con un tablero y diferentes barcos que deberá colocar en esta.

Antes de empezar la partida, el jugador tiene la posibilidad de cambiar el tamaño de su tablero y también tendrá la opción de configurar su flota previo a la batalla.

Durante la fase de batalla, el jugador debe hacer uso de coordenadas para bombardear el sector enemigo y destruir la flota enemiga. Por otro lado, la computadora intentará hacer lo mismo con el jugador.

Simbología del tablero:

~: Mar
- El mar es un sector vacío que se muestra previo a que el sector sea atacado.

X: Disparo acertado
- El disparo acertado indica que se le ha acertado a un objetivo

O: Disparo fallido
- El disparo fallido indica que no habían objetivos en el área de ataque

Si uno de los dos bandos pierde su flotilla completa, el juego terminará con el jugador sobreviviente siendo el ganador

Como dato adicional, el jugador no sabrá si ha hundido un barco enemigo o no, pero sí si ha hundido todos los barcos enemigos.

¡Buena suerte, jugador!

Presione Enter para continuar...
````


#### **Opción 3:**

-La opción 3 'Salir', finaliza la ejecución del programa.

````
Ingrese su opción: 3
Saliendo del juego...
````

## Listado de opciones que aparecen en el segundo menú del juego con sus ejemplos respectivos.

### El segundo menú interactivo de este código es el siguiente:

````
========================================

     CONFIGURACIÓN DE LA FLOTA

========================================

0) Elegir tamaño tablero
1) Organizar tu flota
2) Iniciar Batalla

Elige una opción: 
````

#### **Opción 1.0:**

-La opción 1.0 'Elegir tamaño tablero', despliega una pantalla en la cual se puede personalizar el tamaño del tablero.

````
========================================

         TAMAÑO DEL TABLERO

========================================

Selecciona el tamaño del tablero.

Ancho (Entre 5 y 10 espacios): N
Alto (Entre 5 y 10 espacios): N

Tablero actualizado a N x N.
````


#### **Opción 1.1:**

-La opción 1.1 'Organizar tu flota', despliega el tablero del jugador estando vacío, para que el jugador indique las posiciones a utilizar por cada uno de sus barcos.

````
========================================

      CONFIGURACIÓN DE LA FLOTA

========================================

     1  2  3  4  5 

A    ~  ~  ~  ~  ~ 
B    ~  ~  ~  ~  ~ 
C    ~  ~  ~  ~  ~ 
D    ~  ~  ~  ~  ~ 
E    ~  ~  ~  ~  ~ 

Organizando la flota - Barco de tamaño 1:
Ingrese la posición inicial (ej. A3 para fila A y columna 3): 
````

Además en este apartado, al finalizar la organización de los barcos, el programa muestra por pantalla la distribución de la flota y pregunta si el jugador quiere cambiar la posición de alguno de sus barcos.

````
========================================

            TU TABLERO

========================================

     1  2  3  4  5 

A    *  *  *  *  ~ 
B    ~  *  *  *  ~ 
C    ~  ~  *  *  ~ 
D    ~  ~  ~  *  ~ 
E    ~  ~  ~  ~  ~ 

¿Te gustaría cambiar la posición de algún barco? (S/N): S
Ingrese el tamaño de barco a cambiar de posición: 
````

#### **Opción 1.2:**

-La opción 1.2 'Iniciar Batalla', dado que el jugador ya tenga su flota organizada, da inicio a la batalla jugador vs computadora, eligiendo de forma aleatoria quien inicia atacando el tablero del oponente. Seguido de esto, se mostrará el tablero del jugador y un tablero vacío que corresponde al tablero de la maquina, los cuales se irán actualizando conforme vayan atacandose mutuamente, utilizando la simbologia explicada en las instrucciones del programa.

````
Elige una opción: 2
El jugador inicia la batalla.
````

````
========================================

        TABLERO ENEMIGO

========================================

     1  2  3  4  5 

A    ~  ~  ~  ~  ~ 
B    ~  ~  ~  ~  ~ 
C    ~  ~  ~  ~  ~ 
D    ~  ~  ~  ~  ~ 
E    ~  ~  ~  ~  ~ 


========================================

            TU TABLERO

========================================

     1  2  3  4  5 

A    *  *  *  *  ~ 
B    ~  *  *  *  ~ 
C    ~  ~  *  *  ~ 
D    ~  ~  ~  *  ~ 
E    ~  ~  ~  ~  ~ 

Ingrese la posición a atacar (ej. A3 para fila A y columna 3): 

````

Finalmente, según quien gane se muestra alguno de los siguientes mensajes:

````
¡Felicidades! Has ganado la batalla.
````
````
La computadora ha ganado la batalla.
````

## Cosas a mejorar de programa

Si bien, el programa funciona correctamente de principio a fin, hay ciertas cosas que podrían funcionar mejor, tales como:

- Si uno selecciona un tamaño de tablero, a la hora de organizar los barcos, si el jugador ingresa una posición que está fuera de los límites del tablero, el programa avisará que es una posición invalida, pero esto después de pedir una orientación, cuando lo correcto sería que diera el aviso inmediatamente.

- También al organizar los barcos, se pueden poner las coordenadas tanto en mayúsculas como en minúsculas, pero en el momento de elegir la orientación de este debe ser en mayúsculas, ya que en minúsculas no lo reconoce.

- La IA implementada en este programa si bien, funciona correctamente y no ataca posiciones que ha atacado previamente, etc. No tiene un patrón de ataque en base a si ha acertado un ataque, es decir, si su último ataque ha sido exitoso, no ataca las posiciones adyacentes para terminar de hundir un posible barco de un tamaño mayor a 1.



## Contribuciones para la codificación del juego por cada integrante del grupo.

- **Eduardo Cordero**: Eduardo, como programador asistente del código, ha contribuido al código con implementaciones clave, tales como lo son todos los menú interactivos del programa, y la parte estética de este, de modo que el programa se muestre de forma llamativa y fácil de entender para el usuario, separando las funciones del código en carpetas. Además de ser programador de 'soporte', resolviendo problemas de segmentación presentes durante el proceso de codificación del juego.

- **Francisca Guzmán**: Francisca ha liderado la implementación del código como programadora principal. Se ha encargado de multiples funciones, tales como la que muestra las instrucciones, la que se encarga de la configuración de los tableros, donde se elige un tamaño personalizado de este, y se organizan los barcos en este. Además, fue la implementadora de la IA encargada de configurar el tablero de la computadora, hacer sus jugadas, etc.

- **Ignacio Matus de la Parra**: Ignacio, como programador asistente del código, ha colaborado en la implementación de funciones específicas, tales como la que verifica que haya una victoria tanto del jugador como de la maquina, también siendo programador de 'soporte', ayudando a identificar problemas y resolverlos junto a los demás integrantes. Además realizó el presente archivo README.md que introduce el proyecto de forma clara y proporciona instrucciones detalladas para compilar, ejecutar y utilizar el juego, incluyendo ejemplos específicos para cada opción del menú en tu código de Battleship. Puedes ajustar los detalles de contacto y cualquier otra especificación según tus necesidades.

- **Vicente Quezada**: Vicente ha liderado el equipo como programador principal del código, él ha implementado las *tdas* y *structs* del código y muchas de las funciones más relevantes de este, tales como la función encargada de inicializar los tableros, o la de ataque. Además ha solucionado problemas de segmentación durante el código, teniendo un rol muy activo en este código. 
