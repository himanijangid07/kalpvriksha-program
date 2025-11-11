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

struct Node* removeNthNodeFromEnd(struct Node* head, int node) {
    struct Node* fast = head;

    for(int index = 0; index < node; index++) {
        fast = fast->next;
    }

    if(fast == NULL) {
        struct Node* newHead = head->next;
        free(head);
        return newHead;
    }

    struct Node* slow = head;
    while(fast->next != NULL) {
        fast = fast->next;
        slow = slow->next;
    }

    struct Node *delNode = slow->next;
    slow->next = delNode->next;
    free(delNode);

    return head;
}

void printList(struct Node* temp) {
    while(temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main() {
    int a[7] = {1, 2, 3, 4, 5, 6, 7};
    int n = 7;

    struct Node* head = createNode(a[0]);
    struct Node* tail = head;

    for(int index = 1; index < n; index++) {
        struct Node* nextNode = createNode(a[index]);
        tail->next = nextNode;
        tail = nextNode;
    }

    printf("Original list: ");
    printList(head);

    head = removeNthNodeFromEnd(head, 4);
    printf("After removing nth node: ");
    printList(head);

    struct Node* temp = head;
    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}