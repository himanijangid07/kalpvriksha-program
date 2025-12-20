#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    int choice, amount;
    printf("1. Withdraw\n2. Deposit\n3. Balance\n");
    scanf("%d", &choice);

    write(sock, &choice, sizeof(choice));

    if(choice == 1 || choice == 2) {
        scanf("%d", &amount);
        write(sock, &amount, sizeof(amount));
    }

    char buffer[100];
    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);

    close(sock);

    return 0;
}