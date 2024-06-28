#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hash.h"
#include <unistd.h>


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
  Pair ** buckets;
  long size; //cantidad de datos/pairs en la tabla
  long capacity; //capacidad de la tabla
  long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2) {
  if(key1==NULL || key2==NULL) return 1;
  if(strcmp((char*)key1,(char*)key2) == 0) return 0;
  return 1;
}


void insertMap(HashMap * map, char * key, void * value) {
    if (map == NULL || key == NULL) return;
    int posicion = hash(key, map->capacity);
    int indiceOriginal = posicion;
  
    while (1) {
        Pair *current_pair = map->buckets[posicion];
        if (current_pair == NULL || current_pair->key == NULL) {
            Pair *new_pair = createPair(key, value);
            map->buckets[posicion] = new_pair;
            map->size++;
            map->current = posicion;
            return;
        } else if (is_equal(current_pair->key, key)) {
            current_pair->value = value;
            return;
        }

        posicion = (posicion + 1) % map->capacity;
        if (posicion == indiceOriginal) {
            return;
        }
    }
}




void enlarge(HashMap *map) {
  if (map == NULL) return;
  enlarge_called = 1; // No borrar (para propósitos de prueba)
  Pair **old_buckets = map->buckets;
  map->capacity *= 2;
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->size = 0;

  for (long i = 0; i < map->capacity / 2; i++) {
    if (old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
      insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
    }
  }
  free(old_buckets);
}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *) malloc(sizeof(HashMap));
  if(map != NULL){
    map->buckets = (Pair **)malloc(capacity * sizeof(Pair *));
    if(map->buckets != NULL){
      for (long i = 0; i < capacity; i++) {
          map->buckets[i] = NULL;
      }
      map -> size = 0;
      map -> capacity = capacity;
      map -> current = -1;
    }else{
      free(map);
      map = NULL;
    }
  }
  return map;
}

void eraseMap(HashMap *map, char *key) {
  if (map == NULL || key == NULL) return;
  int posicion = hash(key, map->capacity); 
  int posOriginal = posicion;
  while (1) {
    Pair *current_pair = map->buckets[posicion];
    if (current_pair == NULL) {
      return;
    } else if (is_equal(current_pair->key, key)) {
      current_pair->key = NULL;
      map->size--;
      return;
    }
    posicion = (posicion + 1) % map->capacity;
    if (posicion == posOriginal) {
      return;
    }
  }
}

Pair * searchMap(HashMap * map, char * key) {   
    if (map == NULL || key == NULL) return NULL;
    int posicion = hash(key, map->capacity);
    int posOriginal = posicion;

    while (1) {
        Pair *current_pair = map->buckets[posicion];
        if (current_pair == NULL) {
            map->current = -1;
            return NULL;
        } else if (is_equal(current_pair->key, key)) {
            map->current = posicion; 
            return current_pair;
        }
      
        posicion = (posicion + 1) % map->capacity;
        if (posicion == posOriginal) {
            map->current = -1;
            return NULL;
        }
    }
}




Pair * firstMap(HashMap * map) {
  if(map == NULL) return NULL;
  for (long i = 0; i < map->capacity; i++) {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
      map->current = i;
      return map->buckets[i];
    }
  }
  map->current = -1;
  return NULL;
}

Pair * nextMap(HashMap * map) {
  if (map == NULL || map->current == -1) return NULL;
  for (long i = map->current + 1; i < map->capacity; i++) {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
      map->current = i;
      return map->buckets[i];
     }
  }
  map->current = -1;
  return NULL;
}
void map_clean(HashMap *map) {
    if (map == NULL) return;

    for (long i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            free(map->buckets[i]->key);
            free(map->buckets[i]->value);
            free(map->buckets[i]);
        }
    }
    free(map->buckets);
    free(map);
}