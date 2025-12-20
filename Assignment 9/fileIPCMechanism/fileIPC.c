#include <stdio.h>
#include <stdlib.h>
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
    int arr[] = {5, 2, 9, 1, 3};
    int size = 5;

    FILE* fp = fopen("data.txt", "w");
    for(int index = 0; index < size; index++) {
        fprintf(fp, "%d ", arr[index]);
    }

    fclose(fp);

    printf("Parent: Before sorting: \n");
    for(int index = 0; index < size; index++) {
        printf("%d ", arr[index]);
    }
    printf("\n");

    pid_t pid = fork();

    if(pid == 0) {
        int temp[5];
        fp = fopen("data.txt", "r");
        
        for(int index = 0; index < size; index++) {
            fscanf(fp, "%d", &temp[index]);
        }

        fclose(fp);
        sort(temp, size);
        fp = fopen("data.txt", "w");

        for(int index = 0; index < size; index++) {
            fprintf(fp, "%d ", temp[index]);
        }

        fclose(fp);
        exit(0);
    } else {
        wait(NULL);
        fp = fopen("data.txt", "r");

        for(int index = 0; index < size; index++) {
            fscanf(fp, "%d", &arr[index]);
        }

        fclose(fp);

        printf("Parent: After sorting: \n");
        for(int index = 0; index < size; index++) {
            printf("%d ", arr[index]);
        }
        printf("\n");
    }

    return 0;
}