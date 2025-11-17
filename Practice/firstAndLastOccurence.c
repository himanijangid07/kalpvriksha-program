#include <stdio.h>

int firstOccurence(int* arr, int size, int target) {
    int low = 0, high = size - 1, first = -1;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(arr[mid] == target) {
            first = mid;
            high = mid - 1;
        } else if(arr[mid] > target) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return first;
}

int lastOccurence(int* arr, int size, int target) {
    int low = 0, high = size - 1, last = -1;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(arr[mid] == target) {
            last = mid;
            low = mid + 1;
        } else if(arr[mid] > target) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return last;
}

int main() {
    int arr[6] = {1, 4, 4, 4, 7, 9};
    int size = sizeof(arr) / sizeof(arr[0]);

    int result[2] = {-1, -1};

    result[0] = firstOccurence(arr, size, 4);
    result[1] = lastOccurence(arr, size, 4);

    for(int index = 0; index < 2; index++) {
        printf("%d ", result[index]);
    }
    printf("\n");

    return 0;
}