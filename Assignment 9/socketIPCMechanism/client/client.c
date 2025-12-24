#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    while (1) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        connect(sock, (struct sockaddr *)&addr, sizeof(addr));

        int choice, amount;
        char buffer[200];

        printf("\n1. Withdraw\n2. Deposit\n3. Balance\n4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 4) {
            close(sock);
            break;
        }

        write(sock, &choice, sizeof(choice));

        if (choice == 1 || choice == 2) {
            printf("Enter amount: ");
            scanf("%d", &amount);
            write(sock, &amount, sizeof(amount));
        }
        int bytes = read(sock, buffer, sizeof(buffer) - 1);
        buffer[bytes] = '\0';
        printf("Server: %s", buffer);

        close(sock);
    }

    return 0;
}