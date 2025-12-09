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

void hashMapPut(int pid, PCB* pcb) {
    int index = hashIndex(pid);
    HashNode* hn = hashTable[index];

    while(hn) {
        if(hn->pid == pid) {
            return;
        }
        hn = hn->next;
    }

    hn = (HashNode*)malloc(sizeof(HashNode));
    hn->pid = pid;
    hn->pcb = pcb;
    hn->next = hashTable[index];
    hashTable[index] = hn;
}

PCB* hashMapGet(int pid) {
    int index = hashIndex(pid);
    HashNode* hn = hashTable[index];

    while(hn) {
        if(hn->pid = pid) {
            return hn->pcb;
        }
        hn = hn->next;
    }
    return NULL;
}

void hashMapRemove(int pid) {
    int index = hashIndex(pid);
    HashNode* hn = hashTable[index];
    HashNode* prev = NULL;

    while(hn) {
        if(prev) {
            prev->next = hn->next;
        } else {
            hashTable[index] = hn->next;
        }
        free(hn);
        return;
    }
    prev = hn;
    hn = hn->next;
}

void addKillEvent(int pid, int time) {
    KillEvent* ke = (KillEvent*)malloc(sizeof(KillEvent));
    ke->pid = pid;
    ke->time = time;
    ke->next = killEvents;
    killEvents = ke;
}

PCB* createPCB(const char* name, int pid, int burst, int io_start, int io_duration) {
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    strncpy(pcb->name, name, MAX_NAME - 1);
    pcb->name[MAX_NAME - 1] = '\0';
    pcb->pid = pid;
    pcb->burst = burst;
    pcb->io_start = io_start;
    pcb->io_duration = io_duration;
    pcb->remanining_burst = burst;
    pcb->executed = 0;
    pcb->remaining_io = 0;
    pcb->state = READY;
    pcb->arrival = 0;
    pcb->completion_time = -1;
    pcb->next = NULL;
    return pcb;
}

int removeFromQueuesAndTerminate(int pid, int tick) {
    PCB* pcb = hashMapGet(pid);
    if(!pcb) return 0;

    PCB* removed = queueRemoveByPid(&readyQ, pid);
    if(removed) {
        removed->state = TERMINATED;
        removed->completion_time = tick;
        queuePush(&terminatedQ, removed);
        hashMapRemove(pid);
        return 1;
    }

    removed = queueRemoveByPid(&waitingQ, pid);
    if(removed) {
        removed->state = TERMINATED;
        removed->completion_time = tick;
        queuePush(&terminatedQ, removed);
        hashMapRemove(pid);
        return 1;
    }
    return 0;
}

void printResults() {
    printf("PID\tName\tCPU\tIO\tTurnaround\tWaiting\n");
    PCB* pcb = terminatedQ.head;

    while(pcb) {
        int turnAround = pcb->completion_time - pcb->arrival;
        int waiting = turnAround - pcb->burst;
        if(waiting < 0) waiting = 0;

        printf("%d\t%s\t%d\t%d\t%d\t\t%d\n", pcb->pid, pcb->name, pcb->burst, pcb->io_duration, turnAround, waiting);
        pcb = pcb->next;
    }
}

void cleanUp() {
    for(int index = 0; index < HASH_SIZE; index++) {
        HashNode* hn = hashTable[index];

        while(hn) {
            HashNode* next = hn->next;
            free(hn);
            hn = next;
        }
    }

    while(killEvents) {
        KillEvent* killNext = killEvents->next;
        free(killEvents);
        killEvents = killNext;
    }

    PCB* pcb = terminatedQ.head;
    while(pcb) {
        PCB* next = pcb->next;
        free(pcb);
        pcb = next;
    }
}

int main() {
    if(totalProcesses == 0) {
        return 0;
    }

    printResults();
    cleanUp();

    return 0;
}