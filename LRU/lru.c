#include "lru.h"

#include <stdio.h>
#include <stdlib.h>


int hash(int key) { 
    return key % hashSize; 
}

void movetofront(Cache *cache, Node *node) {
  if (node == cache->front) {
    return;
  }
  if (node == cache->rear) {
    cache->rear = node->prev;
    cache->rear->next = NULL;
  } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }
  node->prev = NULL;
  node->next = cache->front;
  cache->front->prev = node;
  cache->front = node;
}

void insertAtFront(Cache *cache, Node *node) {
  node->prev = NULL;
  node->next = cache->front;
  if (cache->front) {
    cache->front->prev = node;
  }

  cache->front = node;
  cache->count++;
}

void deleteNode(Node *node, struct HashMap *hashMap[]) {
  int index = hash(node->key);
  struct HashMap *curr = hashMap[index];
  struct HashMap *prev = NULL;
  while (curr != NULL) {
    if (curr->node == node) {
      if (curr->next == NULL) {
        if (prev == NULL) {
          hashMap[index] = NULL;
        } else {
          prev->next = NULL;
        }
      } else {
        if (prev == NULL) {
          hashMap[index] = curr->next;
        } else {
          prev->next = curr->next;
        }
      }
      free(curr);
      break;
    }
    prev = curr;
    curr = curr->next;
  }
  free(node);
}

void removeNode(Cache *cache, struct HashMap *hashMap[]) {
  if (cache->rear == NULL) {
    return;
  }
  Node *temp = cache->rear;
  if (cache->rear == cache->front) {
    cache->rear = NULL;
    cache->front = NULL;
  } else {
    cache->rear = temp->prev;
    cache->rear->next = NULL;
  }
  cache->count--;
  deleteNode(temp, hashMap);
}

Cache *createCache(int capacity) {
  Cache *cache = (Cache *)malloc(sizeof(Cache));
  cache->capacity = capacity;
  cache->count = 0;
  cache->front = NULL;
  cache->rear = NULL;
  return cache;
}

void put(int key, char *value, struct HashMap *hashMap[], Cache *cache) {
  int index = hash(key);
  struct HashMap *temp = hashMap[index];
  while (temp != NULL) {
    if (temp->node->key == key) {
      temp->node->value = value;
      movetofront(cache, temp->node);
      return;
    }
    temp = temp->next;
  }

  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->key = key;
  newNode->value = value;
  newNode->prev = NULL;
  newNode->next = NULL;
  if (hashMap[index] == NULL) {
    struct HashMap *newHashMapNode =
        (struct HashMap *)malloc(sizeof(struct HashMap));
    newHashMapNode->node = newNode;
    newHashMapNode->next = NULL;
    hashMap[index] = newHashMapNode;
  } else {
    struct HashMap *temp = hashMap[index];
    while (temp->next != NULL) {
      temp = temp->next;
    }
    struct HashMap *newHashMapNode =
        (struct HashMap *)malloc(sizeof(struct HashMap));
    newHashMapNode->node = newNode;
    newHashMapNode->next = NULL;
    temp->next = newHashMapNode;
  }
  if (cache->count == cache->capacity) {
    removeNode(cache,hashMap);
  }
  insertAtFront(cache, newNode);
}

char *get(int key, struct HashMap *hashMap[], Cache *cache) {
  int index = hash(key);
  struct HashMap *temp = hashMap[index];
  while (temp != NULL) {
    if (temp->node->key == key) {
      movetofront(cache, temp->node);
      return temp->node->value;
    }
    temp = temp->next;
  }
  return NULL;
}
