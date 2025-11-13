#include <stdio.h>

int peakElement(int* arr, int size) {
    int low = 0;
    int high = size - 1;

    while(low < high) {
        int mid = low + (high - low) / 2;
        if(arr[mid] < arr[mid + 1]) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2};
    int size = 10;

    int peak = peakElement(arr, size);
    printf("Peak element index: %d\n", peak);

    return 0;
}