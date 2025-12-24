#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void sort(int arr[], int size) {
    for(int index = 0; index < size - 1; index++) {
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
    int shmId = shmget(IPC_PRIVATE, 5 * sizeof(int), 0666 | IPC_CREAT);
    int* arr = shmat(shmId, NULL, 0);

    int data[] = {9, 5, 2, 7, 1};

    for(int index = 0; index < 5; index++) {
        arr[index] = data[index];
    }

    if(fork() == 0) {
        sort(arr, 5);
        shmdt(arr);
    } else {
        wait(NULL);
        printf("Sorted array: \n");

        for(int index = 0; index < 5; index++) {
            printf("%d ", arr[index]);
        }
        
        shmctl(shmId, IPC_RMID, NULL);
    }

    return 0;
}