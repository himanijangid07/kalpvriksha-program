#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define DB_FILE "../resource/accountDB.txt"

pthread_mutex_t lock;

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);

    int choice, amount;
    char response[100];
    int balance;

    read(client_sock, &choice, sizeof(choice));

    pthread_mutex_lock(&lock);

    FILE *fp = fopen(DB_FILE, "r+");
    if (!fp) {
        sprintf(response, "File error\n");
        write(client_sock, response, strlen(response));
        pthread_mutex_unlock(&lock);
        close(client_sock);
        return NULL;
    }

    fscanf(fp, "%d", &balance);

    if (choice == 1) {
        read(client_sock, &amount, sizeof(amount));
        if (amount <= balance) {
            balance -= amount;
            sprintf(response, "Withdraw %d SUCCESS | Balance = %d\n", amount, balance);
        } else {
            sprintf(response, "Withdraw %d FAILED | Balance = %d\n", amount, balance);
        }
    } else if (choice == 2) { 
        read(client_sock, &amount, sizeof(amount));
        balance += amount;
        sprintf(response, "Deposit %d SUCCESS | Balance = %d\n", amount, balance);
    } else if (choice == 3) {
        sprintf(response, "Current Balance = %d\n", balance);
    }

    rewind(fp);
    ftruncate(fileno(fp), 0);
    fprintf(fp, "%d", balance);
    fclose(fp);

    pthread_mutex_unlock(&lock);

    write(client_sock, response, strlen(response));
    close(client_sock);
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    pthread_mutex_init(&lock, NULL);

    printf("Server running on port %d...\n", PORT);
    fflush(stdout);

    while (1) {
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(server_fd, NULL, NULL);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_sock);
        pthread_detach(tid);
    }

    return 0;
}