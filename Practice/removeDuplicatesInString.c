#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* removeDuplicates(char* str) {
    int length = strlen(str);
    char* stack = (char*)malloc(length * sizeof(char));
    int top = -1;

    for(int index = 0; index < length; index++) {
        if(top >= 0 && stack[top] == str[index]) {
            top--;
        } else {
            stack[++top] = str[index];
        }
    }

    stack[top + 1] = '\0';

    return stack;
}

int main() {
    char s[] = "abbaca";
    char* result = removeDuplicates(s);
    printf("Result: %s\n", result);

    free(result);
    return 0;
}