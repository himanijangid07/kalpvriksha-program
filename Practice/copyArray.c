#include <stdio.h>
#include <stdlib.h>

void copyArray(int *source, int *destination,  int size) {
    if(size <= 0) printf("Invalid size\n");

    for(int index = 0; index < size; index++) {
        *(destination + index) = *(source + index);
    }
}

void printArray(int *array, int size) {
    for(int index = 0; index < size; index++) {
        printf("%d ", *(array + index));
    }
    printf("\n");
}

int main() {
    int size;
    printf("Enter the number of elements: ");
    while(scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input\n");
        while(getchar() != '\n');
    }

    int *source = (int *)malloc(size * sizeof(int));
    int *destination = (int *)malloc(size * sizeof(int));

    if(source == NULL || destination == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter the elements: ");
    for(int index = 0; index < size; index++) {
        while(scanf("%d", (source + index)) != 1) {
            printf("Invalid input\n");
            while(getchar() != '\n');
        }
    }

    printf("Original array: ");
    printArray(source, size);

    printf("Copied array: ");
    copyArray(source, destination, size);
    printArray(destination, size);
    
    free(source);
    free(destination);
    return 0;
}