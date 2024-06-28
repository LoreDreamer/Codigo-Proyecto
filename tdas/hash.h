//
//  HashMap.h
//  TablaHashLab
//
//  Created by Matias Barrientos on 11-08-18.
//  Copyright © 2018 Matias Barrientos. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h


typedef struct HashMap HashMap;

typedef struct Pair {

     char * key;
     void * value;

} Pair;

HashMap * createMap(long capacity); // Crea un mapa con la capacidad especificada

void insertMap(HashMap * table, char * key, void * value); // Inserta un par <key, value> en el mapa

void eraseMap(HashMap * table, char * key); // Elimina el par asociado a la clave especificada del mapa

Pair * searchMap(HashMap * table, char * key); // Busca el par asociado a la clave especificada en el mapa

Pair * firstMap(HashMap * table); // Devuelve el primer par del mapa

Pair * nextMap(HashMap * table); // Devuelve el siguiente par del mapa

void enlarge(HashMap * map); // Aumenta la capacidad del mapa

void map_clean(HashMap * map); // Libera la memoria asignada al mapa

#endif /* HashMap_h */