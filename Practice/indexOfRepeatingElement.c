#include <stdio.h>
#include <stdlib.h>

int firstRepeating(int* arr, int size) {
    int maxVal = arr[0];
    
    for(int index = 1; index < size; index++) {
        if(maxVal < arr[index]) {
            maxVal = arr[index];
        }
    }

    int* visited = (int*)calloc(maxVal + 1, sizeof(int));
    int answer = -1;

    for(int index = size - 1; index >= 0; index--) {
        if(visited[arr[index]]) {
            answer = arr[index];
        }

        visited[arr[index]] = 1;
    }

    free(visited);
    return answer;
}

int main() {
    int arr[10] = {1, 3, 2, 5, 2, 3, 4, 7, 8, 9};
    int size = sizeof(arr) / sizeof(arr[0]);

    int ans = firstRepeating(arr, size);

    printf("First Repeating Element: %d\n", ans);

    return 0;
}