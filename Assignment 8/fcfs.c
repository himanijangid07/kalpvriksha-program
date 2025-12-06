#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 1031
#define MAX_NAME 64
#define MAX_LINE 256

typedef enum {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} State;

typedef struct PCB {
    int pid;
    char name[MAX_NAME];
    int burst;
    int io_start;
    int io_duration;
    int remanining_burst;
    int executed;
    int remaining_io;
    State state;
    int arrival;
    int completion_time;
    struct PCB* next;
} PCB;

typedef struct {
    PCB* head;
    PCB* tail;
    int size;
} Queue;

typedef struct HashNode {
    int pid;
    PCB* pcb;
    struct HashNode* next;
} HashNode;

typedef struct KillEvent {
    int pid;
    int time;
    struct KillEvent* next;
} KillEvent;

Queue readyQ, waitingQ, terminatedQ;
HashNode* hashTable[HASH_SIZE];
KillEvent* killEvents = NULL;

int totalProcesses = 0;

char* trim(char* str) {
    while(isspace((unsigned char)*str)) str++;

    if(str == 0) return str;
    char* end = str + strlen(str) - 1;

    while(end > str && isspace((unsigned char) *end)) end--;
    end[1] = '\0';

    return str;
}

void queueInit(Queue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

void queuePush(Queue* queue, PCB* pcb) {
    pcb->next = NULL;

    if(queue->tail) {
        queue->tail->next = pcb;
    } else {
        queue->head = pcb;
    }
    queue->tail = pcb;
    queue->size++;
}

PCB* queuePop(Queue* queue) {
    if(!queue->head) return NULL;
    
    PCB* pcb = queue->head;
    queue->head = pcb->next;

    if(!queue->head) queue->tail = NULL;

    pcb->next = NULL;
    queue->size--;
    return pcb;
}

PCB* queueRemoveByPid(Queue* queue, int pid) {
    PCB* current = queue->head;
    PCB* prev = NULL;

    while(current) {
        if(current->pid == pid) {
            if(prev) {
                prev->next = current->next;
            } else {
                queue->head = current->next;
            }
            if(current == queue->tail) {
                queue->tail = prev;
            }
            current->next = NULL;
            queue->size--;
            return current;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

int hashIndex(int pid) {
    return (pid % HASH_SIZE + HASH_SIZE) % HASH_SIZE;
}

int main() {
    if(totalProcesses == 0) {
        return 0;
    }

    return 0;
}