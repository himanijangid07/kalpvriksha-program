#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1009

typedef struct Node {
    int key;
    char* value;

    struct Node* prev;
    struct Node* next;
} Node;

typedef struct HashNode {
    int key;
    Node* node;
    struct HashNode* next;
} HashNode;

typedef struct LRUCache {
    int capacity;
    int size;

    Node* head;
    Node* tail;

    HashNode* hashMap[HASH_SIZE];
} LRUCache;

int hashFunction(int key) {
    return key % HASH_SIZE;
}

LRUCache* createCache(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));

    if(!cache) {
        printf("Memory allocation failed\n");
        return;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for(int index = 0; index < HASH_SIZE; index++) {
        cache->hashMap[index] = NULL;
    }

    return cache;
}

int main() {
    char* command[50];
    LRUCache* cache = NULL;

    while(1) {
        scanf("%s", command);
        
        if(strcmp(command, "createCache") == 0) {
            int size;
            scanf("%d", &size);
            cache = createCache(size);
        } else if(strcmp(command, "put") == 0) {
            int key;
            char value[100];
            scanf("%d %s", &key, value);
            put(cache, key, value);
        } else if(strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char* result = get(cache, key);
            if(result) {
                printf("%s\n", result);
            } else {
                printf("NULL\n");
            }
        } else if(strcmp(command, "exit") == 0) {
            break;
        }
    }

    return 0;
}