#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void sort(int arr[], int size) {
    for (int index = 0; index < size - 1; index++) {
        for (int iterator = 0; iterator < size - index - 1; iterator++) {
            if (arr[iterator] > arr[iterator + 1]) {
                int temp = arr[iterator];
                arr[iterator] = arr[iterator + 1];
                arr[iterator + 1] = temp;
            }
        }
    }
}

int main() {
    int fd[2];
    pipe(fd);

    int arr[] = {7, 4, 1, 9, 2};
    int size = 5;

    pid_t pid = fork();

    if (pid == 0) {
        close(fd[1]);
        read(fd[0], arr, sizeof(arr));
        sort(arr, size);
        close(fd[0]);

        write(fd[1], arr, sizeof(arr));
    } else {
        close(fd[0]);
        write(fd[1], arr, sizeof(arr));
        wait(NULL);

        read(fd[0], arr, sizeof(arr));
        printf("Sorted array: \n");

        for (int index = 0; index < size; index++) {
            printf("%d ", arr[index]);
        }
    }

    return 0;
}