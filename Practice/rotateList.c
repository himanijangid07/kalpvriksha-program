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

struct Node* rotateList(struct Node* head, int k) {
    if(head == NULL || head->next == NULL || k == 0) return head;

    struct Node* temp = head;
    int count = 1;

    while(temp->next != NULL) {
        temp = temp->next;
        count++;
    }

    temp->next = head;
    k = k % count;

    int stepsToNewTail = count - k;

    struct Node* newTail = head;

    for(int i=1; i<stepsToNewTail; i++) {
        newTail = newTail->next;
    }

    struct Node* newHead = newTail->next;
    newTail->next = NULL;

    return newHead;
}

void printList(struct Node* head) {
    struct Node* temp = head;

    while(temp != NULL) {
        printf("%d->", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    int number = 5;

    struct Node* head = createNode(arr[0]);
    struct Node* tail = head;

    for(int index=1; index<number; index++) {
        struct Node* nextNode = createNode(arr[index]);
        tail->next = nextNode;
        tail = nextNode;
    }

    printf("Original list: ");
    printList(head);

    head = rotateList(head, 3);
    printf("After rotating: ");
    printList(head);

    struct Node* temp = head;
    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}