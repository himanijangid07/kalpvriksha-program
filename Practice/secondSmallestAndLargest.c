#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int secondSmallest(int *nums, int size) {
    if(size < 2) {
        printf("Array must contain at least two elements \n");
        return -1;
    }

    int smallest = nums[0];
    int secondSmallest = INT_MAX;

    for(int index = 1; index < size; index++) {
        if(nums[index] < smallest) {
            secondSmallest = smallest;
            smallest = nums[index];
        } else if(nums[index] > smallest && nums[index] < secondSmallest) {
            secondSmallest = nums[index];
        }
    }

    if(secondSmallest == INT_MAX) {
        printf("All elements are the same\n");
        return -1;
    }
    return secondSmallest;
}

int secondLargest(int *nums, int size)
{
    if (size < 2)
    {
        printf("Array must contain at least two elements \n");
        return -1;
    }

    int largest = nums[0];
    int secondLargest = INT_MIN;

    for (int index = 1; index < size; index++)
    {
        if (nums[index] > largest)
        {
            secondLargest = largest;
            largest = nums[index];
        }
        else if (nums[index] < largest && nums[index] > secondLargest)
        {
            secondLargest = nums[index];
        }
    }

    if(secondLargest == INT_MIN) {
        printf("All elements are the same\n");
        return -1;
    }

    return secondLargest;
}

int main() {
    int size;
    printf("Enter the number of elements: ");
    while(scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input\n");
        while(getchar() != '\n');
    }

    int nums[size];
    printf("Enter the elements: ");
    for(int index = 0; index < size; index++) {
        while(scanf("%d", &(nums[index])) != 1) {
            printf("Invalid input\n");
            while(getchar() != '\n');
        }
    }  
    
    int secondSmallestValue = secondSmallest(nums, size);
    int secondLargestValue = secondLargest(nums, size);

    if(secondSmallestValue != -1) {
        printf("Second Smallest: %d\n", secondSmallestValue);
    }

    if(secondLargestValue != -1) {
        printf("Second Largest: %d\n", secondLargestValue);
    }

    return 0;
}