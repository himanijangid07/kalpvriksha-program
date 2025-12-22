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
    int burst_time;
    int io_start_time;
    int io_duration;
    int remaining_burst;
    int executed_time;
    int remaining_io;
    State state;
    int arrival_time;
    int completion_time;
    int killed_time;
    struct PCB *next;
} PCB;

typedef struct {
    PCB *head;
    PCB *tail;
    int size;
} Queue;

typedef struct HashNode {
    int pid;
    PCB *pcb;
    struct HashNode *next;
} HashNode;

typedef struct KillEvent {
    int pid;
    int time;
    struct KillEvent *next;
} KillEvent;

Queue ready_queue;
Queue waiting_queue;
Queue terminated_queue;
HashNode *hash_table[HASH_SIZE];
KillEvent *kill_events = NULL;

int total_processes = 0;

char *trim(char *str) {
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}

void initQueue(Queue *queue) {
    queue->head = queue->tail = NULL;
    queue->size = 0;
}

void enqueue(Queue *queue, PCB *pcb) {
    pcb->next = NULL;
    if (queue->tail)
        queue->tail->next = pcb;
    else
        queue->head = pcb;
    queue->tail = pcb;
    queue->size++;
}

PCB *dequeue(Queue *queue) {
    if (!queue->head)
        return NULL;
    PCB *pcb = queue->head;
    queue->head = pcb->next;
    if (!queue->head)
        queue->tail = NULL;
    pcb->next = NULL;
    queue->size--;
    return pcb;
}

PCB *removeByPid(Queue *queue, int pid) {
    PCB *current = queue->head, *prev = NULL;
    while (current)
    {
        if (current->pid == pid)
        {
            if (prev)
                prev->next = current->next;
            else
                queue->head = current->next;
            if (current == queue->tail)
                queue->tail = prev;
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

void hashMapPut(int pid, PCB *pcb) {
    int index = hashIndex(pid);
    HashNode *node = malloc(sizeof(HashNode));
    node->pid = pid;
    node->pcb = pcb;
    node->next = hash_table[index];
    hash_table[index] = node;
}

PCB *hashMapGet(int pid) {
    int index = hashIndex(pid);
    HashNode *node = hash_table[index];
    while (node) {
        if (node->pid == pid)
            return node->pcb;
        node = node->next;
    }
    return NULL;
}

void hashMapRemove(int pid) {
    int index = hashIndex(pid);
    HashNode *current = hash_table[index], *prev = NULL;
    while (current) {
        if (current->pid == pid) {
            if (prev)
                prev->next = current->next;
            else
                hash_table[index] = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void addKillEvent(int pid, int time) {
    KillEvent *kill = malloc(sizeof(KillEvent));
    kill->pid = pid;
    kill->time = time;
    kill->next = kill_events;
    kill_events = kill;
}

PCB *createPCB(char *name, int pid, int burst, int io_start, int io_dur) {
    PCB *pcb = malloc(sizeof(PCB));
    strcpy(pcb->name, name);
    pcb->pid = pid;
    pcb->burst_time = burst;
    pcb->io_start_time = io_start;
    pcb->io_duration = io_dur;
    pcb->remaining_burst = burst;
    pcb->executed_time = 0;
    pcb->remaining_io = 0;
    pcb->state = READY;
    pcb->arrival_time = 0;
    pcb->completion_time = -1;
    pcb->killed_time = -1;
    pcb->next = NULL;
    return pcb;
}

void readInput() {
    initQueue(&ready_queue);
    initQueue(&waiting_queue);
    initQueue(&terminated_queue);
    memset(hash_table, 0, sizeof(hash_table));

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), stdin)) {
        char *s = trim(line);
        if (strlen(s) == 0)
            continue;

        char *tokens[5];
        int t = 0;
        char *tok = strtok(s, " \t\n");
        while (tok && t < 5) {
            tokens[t++] = tok;
            tok = strtok(NULL, " \t\n");
        }

        if (strcasecmp(tokens[0], "kill") == 0) {
            addKillEvent(atoi(tokens[1]), atoi(tokens[2]));
        } else {
            int pid = atoi(tokens[1]);
            int burst = atoi(tokens[2]);
            int io_start = (t > 3 && strcmp(tokens[3], "-") != 0) ? atoi(tokens[3]) : -1;
            int io_dur = (t > 4 && strcmp(tokens[4], "-") != 0) ? atoi(tokens[4]) : 0;

            PCB *pcb = createPCB(tokens[0], pid, burst, io_start, io_dur);
            hashMapPut(pid, pcb);
            enqueue(&ready_queue, pcb);
            total_processes++;
        }
    }
}

void applyKills(int tick, PCB **running) {
    KillEvent *current = kill_events, *prev = NULL;
    while (current) {
        if (current->time == tick) {
            PCB *pcb = hashMapGet(current->pid);
            if (pcb)
            {
                if (*running && (*running)->pid == current->pid) {
                    (*running)->state = TERMINATED;
                    (*running)->killed_time = tick;
                    enqueue(&terminated_queue, *running);
                    *running = NULL;
                } else {
                    PCB *rem = removeByPid(&ready_queue, current->pid);
                    if (!rem)
                        rem = removeByPid(&waiting_queue, current->pid);
                    if (rem) {
                        rem->state = TERMINATED;
                        rem->killed_time = tick;
                        enqueue(&terminated_queue, rem);
                    }
                }
                hashMapRemove(current->pid);
            }

            if (prev)
                prev->next = current->next;
            else
                kill_events = current->next;

            KillEvent *temp = current;
            current = current->next;
            free(temp);
            continue;
        }
        prev = current;
        current = current->next;
    }
}

void runSimulation() {
    int tick = 0;
    PCB *running = NULL;

    while (terminated_queue.size < total_processes) {
        applyKills(tick, &running);

        if (!running) {
            running = dequeue(&ready_queue);
            if (running)
                running->state = RUNNING;
        }

        if (running) {
            running->executed_time++;
            running->remaining_burst--;

            if (running->io_start_time >= 0 &&
                running->executed_time == running->io_start_time &&
                running->remaining_burst > 0 &&
                running->io_duration > 0) {
                running->remaining_io = running->io_duration;
                running->state = WAITING;
                enqueue(&waiting_queue, running);
                running = NULL;
            } else if (running->remaining_burst == 0) {
                running->state = TERMINATED;
                running->completion_time = tick + 1;
                enqueue(&terminated_queue, running);
                hashMapRemove(running->pid);
                running = NULL;
            }
        }

        PCB *current = waiting_queue.head, *prev = NULL;
        while (current) {
            current->remaining_io--;
            PCB *next = current->next;
            if (current->remaining_io <= 0) {
                if (prev)
                    prev->next = next;
                else
                    waiting_queue.head = next;
                if (current == waiting_queue.tail)
                    waiting_queue.tail = prev;
                waiting_queue.size--;
                current->state = READY;
                enqueue(&ready_queue, current);
            }
            else
                prev = current;
            current = next;
        }
        tick++;
    }
}

void printResults() {
    PCB *arr[1000];
    int number = 0;
    PCB *temp = terminated_queue.head;
    while (temp) {
        arr[number++] = temp;
        temp = temp->next;
    }

    for (int index = 0; index < number - 1; index++) {
        for (int iterator = index + 1; iterator < number; iterator++) {
            if (arr[index]->pid > arr[iterator]->pid) {
                PCB *t = arr[index];
                arr[index] = arr[iterator];
                arr[iterator] = t;
            }
        }
    }

    printf("PID\tName\tCPU\tIO\tStatus\tTurnaround\tWaiting\n");

    for (int index = 0; index < number; index++) {
        PCB *p = arr[index];

        if (p->killed_time != -1) {
            printf("%d\t%s\t%d\t%d\tKILLED at %d\t-\t\t-\n",
                   p->pid, p->name, p->burst_time,
                   p->io_duration, p->killed_time);
        } else {
            int turnaround = p->completion_time - p->arrival_time;
            int waiting = turnaround - p->burst_time;
            if (waiting < 0)
                waiting = 0;

            printf("%d\t%s\t%d\t%d\tOK\t%d\t\t%d\n",
                   p->pid, p->name, p->burst_time,
                   p->io_duration, turnaround, waiting);
        }
    }
}

int main() {
    readInput();
    if (total_processes == 0)
        return 0;
    runSimulation();
    printResults();
    return 0;
}