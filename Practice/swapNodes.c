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

struct Node* reverse(struct Node* head) {
    struct Node* temp = head;
    struct Node* prev = NULL;

    while(temp != NULL) {
        struct Node* front = temp->next;
        temp->next = prev;
        prev = temp;
        temp = front;
    }
    return prev;
}

struct Node* findkthNode(struct Node* temp, int k) {
    k -= 1;

    while(temp != NULL && k > 0) {
        temp = temp->next;
        k--;
    }
    return temp;
}

struct Node* swapNodes(struct Node* head, int k) {
    if(head == NULL || head->next == NULL) {
        return head;
    }

    struct Node* temp = head;
    struct Node* prev = NULL;

    while(temp != NULL) {
        struct Node* kthNode = findkthNode(temp, k);

        if(kthNode == NULL) break;

        struct Node* nextNode = kthNode->next;
        kthNode->next = NULL;

        struct Node* newHead = reverse(temp);

        if(prev == NULL) {
            head = newHead;
        } else {
            prev->next = newHead;
        }

        prev = temp;
        temp = nextNode;
        prev->next = nextNode;
    }
    return head;
}

void printList(struct Node* temp) {
    while(temp != NULL) {
        printf("%d->", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

int main() {
    int a[5] = {1, 2, 3, 4, 5};
    int n = 5;

    struct Node* head = createNode(a[0]);
    struct Node* tail = head;

    for(int i = 1; i < n; i++) {
        struct Node* nextNode = createNode(a[i]);
        tail->next = nextNode;
        tail = nextNode;
    }

    printf("Original list: ");
    printList(head);

    head = swapNodes(head, 2);
    printf("After swapping: ");
    printList(head);

    struct Node* temp = head;
    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}