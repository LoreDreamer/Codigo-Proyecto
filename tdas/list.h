#ifndef LIST_H
#define LIST_H

typedef struct List List;

List *list_create(); // Crea una lista vacía

void *list_first(List *L); // Devuelve el primer elemento de la lista

void *list_next(List *L); // Devuelve el siguiente elemento de la lista

void list_pushFront(List *L, void *dato); // Agrega un elemento al frente de la lista

void list_pushBack(List *L, void *dato); // Agrega un elemento al final de la lista

void list_pushCurrent(List *L, void *dato); // Agrega un elemento en la posición actual de la lista

void *list_popFront(List *L); // Elimina y devuelve el primer elemento de la lista

void *list_popBack(List *L); // Elimina y devuelve el último elemento de la lista

void *list_popCurrent(List *L); // Elimina y devuelve el elemento en la posición actual de la lista

void list_clean(List *L); // Limpia la lista y liberación de memoria

void list_sortedInsert(List *L, void *data, int (*lower_than)(void *data1, void *data2)); // Inserta un elemento en la lista ordenada

#endif