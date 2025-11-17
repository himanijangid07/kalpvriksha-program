#include <stdio.h>
#include <stdlib.h>

int* kClosestElements(int* arr, int size, int k, int target, int* returnSize) {
    *returnSize = k;

    int left = 0, right = size - 1;

    while(right - left + 1 > k) {
        if(abs(arr[left] - target) > abs(arr[right] - target)) {
            left++;
        } else {
            right--;
        }
    }

    int* result = (int*)malloc(k * sizeof(int));
    for(int index = 0; index < k; index++) {
        result[index] = arr[left + index];
    }
    return result;
}

int main() {
    int arr[6] = {1, 2, 3, 4, 5, 6};
    int size = sizeof(arr) / sizeof(arr[0]);
    int k = 4, target = 3, returnSize;

    int* ans = kClosestElements(arr, size, k, target, &returnSize);

    for(int index = 0; index < k; index++) {
        printf("%d ", ans[index]);
    }

    printf("\n");

    free(ans);
    return 0;
}