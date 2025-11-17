#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int stack[MAX];
    int minStack[MAX];
    int top;
} MinStack;

void initStack(MinStack* s) {
    s->top = -1;
}

int isEmpty(MinStack* s) {
    return s->top == -1;
}

int isFull(MinStack* s) {
    return s->top == MAX - 1;
}

void push(MinStack* s, int value) {
    if(isFull(s)) {
        printf("Stack Overflow\n");
        return;
    }

    s->top++;
    s->stack[s->top] = value;

    if(s->top == 0) {
        s->minStack[s->top] = value;
    } else {
        int min = s->minStack[s->top - 1];
        s->minStack[s->top] = (value < min) ? value : min;
    }
}

void pop(MinStack* s) {
    if(isEmpty(s)) {
        printf("Stack underflow\n");
        return;
    }

    s->top--;
}

int top(MinStack* s) {
    if(isEmpty(s)) {
        printf("Stack underflow\n");
        return -1;
    }

    return s->stack[s->top];
}

int getMin(MinStack* s) {
    if(isEmpty(s)) {
        printf("Stack underflow\n");
        return -1;
    }

    return s->minStack[s->top];
}

int main() {
    MinStack s;
    initStack(&s);

    push(&s, 5);
    push(&s, 2);
    push(&s, 10);
    push(&s, 1);

    printf("Minimum element: %d\n", getMin(&s));

    pop(&s);
    printf("Minimum element: %d\n", getMin(&s));

    return 0;
}