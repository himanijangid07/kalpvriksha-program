#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Node* mergeNodesBetweenZeroes(struct Node* head) {
    struct Node* dummy = createNode(-1);
    struct Node* current = dummy;
    struct Node* temp = head->next;

    int sum = 0;

    while(temp != NULL) {
        if(temp->data != 0) {
            sum += temp->data;
        } else {
            current->next = createNode(sum);
            current = current->next;

            sum = 0;
        }
        temp = temp->next;
    }
    return dummy->next;
}

void printList(struct Node* temp) {
    while(temp != NULL) {
        printf("%d->", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

int main() {
    int a[8] = {0, 1, 0, 3, 0, 2, 2, 0};
    int n = 8;

    struct Node* head = createNode(a[0]);
    struct Node* tail = head;

    for(int i=1; i<n; i++) {
        struct Node* nextNode = createNode(a[i]);
        tail->next = nextNode;
        tail = nextNode;
    }

    printf("Original list: ");
    printList(head);

    head = mergeNodesBetweenZeroes(head);
    printf("After merging: ");
    printList(head);

    struct Node* temp = head;
    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}