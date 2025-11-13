#include <stdio.h>

int searchInRotatedArray(int* arr, int size, int target) {
    int low = 0;
    int high = size - 1;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(arr[mid] == target) return mid;

        if(arr[low] <= arr[mid]) {
            if(arr[low] <= target && target <= arr[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        } else {
            if(arr[mid] <= target && target <= arr[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return -1;
}

int main() {
    int arr[9] = {7, 8, 9, 1, 2, 3, 4, 5, 6};
    int n = 9;
    int target = 1;

    int found = searchInRotatedArray(arr, n, target);
    printf("Found at index: %d\n", found);

    return 0;
}