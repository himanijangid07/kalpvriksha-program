#include <stdio.h>

int main() {
    int size, count;

    printf("Enter the size of the array: ");
    while(scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input.\n");
        while(getchar() != '\n');
    }

    int arr[size], visited[size];

    for(int index = 0; index < size; index++) {
        while(scanf("%d", &arr[index]) != 1) {
            printf("Invalid input.\n");
            while(getchar() != '\n');
        }
        visited[index] = 0;
    }

    for(int index = 0; index < size; index++) {
        if(visited[index] == 1) {
            continue;
        }
        count = 1;

        for(int iterator = index + 1; iterator < size; iterator++) {
            if(arr[index] == arr[iterator]) {
                count++;
                visited[iterator] = 1;
            }
        }

        printf("Element %d occurs %d times\n", arr[index], count);
    }

    return 0;
}