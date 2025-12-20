#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct message {
    long mtype;
    int arr[5];
};

void sort(int arr[], int size) {
    for(int index = 0; index < size; index++) {
        for(int iterator = 0; iterator < size - index - 1; iterator++) {
            if(arr[iterator] > arr[iterator + 1]) {
                int temp = arr[iterator];
                arr[iterator] = arr[iterator + 1];
                arr[iterator + 1] = temp;
            }
        }
    }
}

int main() {
    key_t key = ftok("msgfile", 65);
    int msgId = msgget(key, 0666 | IPC_CREAT);

    struct message m;
    m.mtype = 1;

    int arr[] = {8, 3, 6, 2, 1};
    memcpy(m.arr, arr, sizeof(arr));

    if(fork() == 0) {
        msgrcv(msgId, &m, sizeof(m.arr), 1, 0);
        sort(m.arr, 5);
        m.mtype = 2;
        msgsnd(msgId, &m, sizeof(m.arr), 0);
    } else {
        msgsnd(msgId, &m, sizeof(m.arr), 0);
        msgrcv(msgId, &m, sizeof(m.arr), 2, 0);

        printf("Sorted array: \n");
        for(int index = 0; index < 5; index++) {
            printf("%d ", m.arr[index]);
        }
        msgctl(msgId, IPC_RMID, NULL);
    }

    return 0;
}