#ifndef LRU_H
#define LRU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define hashSize 1000

typedef struct Node {
  int key;
  char *value;
  struct Node *prev;
  struct Node *next;
} Node;

typedef struct HashMap {
  Node *node;
  struct HashMap *next;
} HashMap;

typedef struct Cache {
  Node *front;
  Node *rear;
  int count;
  int capacity;
} Cache;

int hash(int key);
void movetofront(Cache *cache, Node *node);
void insertAtFront(Cache *cache, Node *node);
void deleteNode(Node *node, HashMap *hashMap[]);
void removeNode(Cache *cache,HashMap *hashMap[]);
Cache *createCache(int capacity);
void put(int key, char *value, HashMap *hashMap[], Cache *cache);
char *get(int key, HashMap *hashMap[], Cache *cache);

#endif