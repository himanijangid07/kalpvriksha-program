#include <stdio.h>

void sortElements(int* arr, int size) {
    int low = 0, mid = 0, high = size - 1;

    while(mid <= high) {
        if(arr[mid] == 0) {
            int temp = arr[low];
            arr[low] = arr[mid];
            arr[mid] = temp;
            low++;
            mid++;
        } else if(arr[mid] == 1) {
            mid++;
        } else {
            int temp = arr[mid];
            arr[mid] = arr[high];
            arr[high] = temp;
            high--;
        }
    }
}

int main() {
    int arr[6] = {1, 0, 1, 2, 0, 1};
    int size = sizeof(arr) / sizeof(arr[0]);

    sortElements(arr, size);
    for(int index = 0; index < size; index++) {
        printf("%d ", arr[index]);
    }

    printf("\n");
    return 0;
}