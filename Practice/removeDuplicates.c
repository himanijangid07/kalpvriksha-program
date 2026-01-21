#include <stdio.h>
#include <stdlib.h>

int removeDuplicates(int *nums, int size) {
    if(size == 0) return 0;

    int *current, *end, *next, *shift;
    end = nums + size;

    for(current = nums; current < end; current++) {
        for(next = current + 1; next < end;) {
            if(*next == *current) {
                for(shift = next; shift < end - 1; shift++) {
                    *(shift) = *(shift + 1);
                }
                end--;
            } else {
                next++;
            }
        }
    }
    return end - nums;
}

int main() {
    int size;
    printf("Enter the number of elements: ");
    while(scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input\n");
        while(getchar() != '\n');
    }

    int *nums = (int *)malloc(size * sizeof(int));

    if(nums == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter the elements: ");
    for(int index = 0; index < size; index++) {
        while(scanf("%d", (nums + index)) != 1) {
            printf("Invalid input\n");
            while(getchar() != '\n');
        }
    }

    int newLength = removeDuplicates(nums, size);

    printf("Array after removing duplicates: ");
    for(int *index = nums; index < nums + newLength; index++) {
        printf("%d ", *index);
    }

    free(nums);
    return 0;
}