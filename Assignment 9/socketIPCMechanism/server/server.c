#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

pthread_mutex_t lock;
#define PORT 8080

void* handleClient(void* arg) {
    int sock = *(int*)arg;
    free(arg);

    int choice, amount;
    FILE* fp;

    pthread_mutex_lock(&lock);
    fp = fopen("accountDB.txt", "r+");

    int balance;
    fscanf(fp, "%d", &balance);

    read(sock, &choice, sizeof(choice));

    if(choice == 1) {
        read(sock, &amount, sizeof(amount));

        if(balance >= amount) {
            balance -= amount;
            fprintf(fp, "%d ", balance);
            write(sock, "Withdraw Success\n", 16);
        } else {
            write(sock, "Insufficient Balance\n", 22);
        }
    } else if(choice == 2) {
        read(sock, &amount, sizeof(amount));
        balance += amount;
        fprintf(fp, "%d ", balance);
        write(sock, "Deposit Success\n", 15);
    } else if(choice == 3) {
        char msg[50];
        sprintf(msg, "Balance: %d\n", balance);
        write(sock, msg, strlen(msg));
    }

    fclose(fp);
    pthread_mutex_unlock(&lock);
    close(sock);
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    pthread_mutex_init(&lock, NULL);

    while(1) {
        int client = accept(server_fd, NULL, NULL);
        pthread_t tid;
        int* p = malloc(sizeof(int));
        *p = client;

        pthread_create(&tid, NULL, handleClient, p);
    }

    return 0;
}