#include <stdio.h>
#include <stdlib.h>

void isPalindromeArray(int *arr, int size) {
    int left = 0;
    int right = size - 1;
    int isPalindrome = 1;

    while(left < right) {
        if(*(arr + left) != *(arr + right)) {
            isPalindrome = 0;
            break;
        }
        left++;
        right--;
    }

    if(isPalindrome) {
        printf("Palindrome\n");
    } else {
        printf("Not a Palindrome\n");
    }
}

int main() {
    int size, index;
    printf("Enter the size of the array: ");
    while(scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input. Please enter a positive integer.\n");
        while(getchar() != '\n');
    }

    int *arr = (int *)malloc(size * sizeof(int));
    
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for(index = 0; index < size; index++) {
        while(scanf("%d", (arr + index)) != 1 || *(arr + index) < 0) {
            printf("Invalid input.\n");
            while(getchar() != '\n');
        }
    }

    isPalindromeArray(arr, size);

    free(arr);

    return 0;
}