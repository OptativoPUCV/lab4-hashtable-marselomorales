#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


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

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  if(map==NULL || key==NULL) return;

  long posicion = hash(key,map->capacity);

  while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key != NULL)
    {
      posicion = (posicion+1)%map->capacity; 
    }
  map->buckets[posicion] = createPair(key, value);
  map->size++;
  map->current = posicion;
}

void enlarge(HashMap * map) 
{
  if(map==NULL) return;

  //Inicializamos el size del mapa en 0
  map->size = 0;

  //Conservamos el viejo mapa considerando la antigua capacidad de este
  Pair **old_array = map->buckets;
  long antiguaCapacidad = map->capacity;
  //Duplicamos la capacidad del mapa
  long nuevaCapacidad = map->capacity * 2;
  //Reservamos la nueva capacidad del mapa
  map->capacity = nuevaCapacidad;
  map->buckets = calloc(nuevaCapacidad, sizeof(HashMap*));  
  //Recorremos el mapa viejo buscando los datos y los insertamos en el nuevo mapa
  for (int i = 0; i < antiguaCapacidad; i++)
    {
      if (old_array[i] != NULL)
      {
        insertMap(map, old_array[i]->key, old_array[i]->value);
        free(old_array[i]);
        (map->size)++;
      }
    } 
  //Liberamos la memoria del viejo mapa
  free(old_array);
  
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));

  map->buckets = (Pair **)calloc(capacity,sizeof(Pair *));

  map->size = 0;
  map->capacity = capacity;
  map->current = -1;

  return map;
  return NULL;
}

void eraseMap(HashMap * map,  char * key) 
{
  if(map==NULL || key==NULL) return;

  long posicion = hash(key,map->capacity);

  while(map->buckets[posicion]!=NULL)
  {
    if(map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key,key))
    {
      free(map->buckets[posicion]);
      map->buckets[posicion] = NULL;
      map->size--;
      return;
    }
    posicion = (posicion+1)%map->capacity;
  }
  
}

Pair * searchMap(HashMap * map,  char * key) 
{   
    if(map == NULL || key == NULL) return NULL;

    long posicion = hash(key, map->capacity);

    while(map->buckets[posicion] != NULL) 
    {
        if(map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key, key)) 
        {
            map->current = posicion;
            return map->buckets[posicion];
        }
        posicion = (posicion + 1) % map->capacity; // Avanzar a la siguiente posición
    }

    return NULL;
}


Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
