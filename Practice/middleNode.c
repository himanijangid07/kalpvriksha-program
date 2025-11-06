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

struct Node* middleNode(struct Node* head) {
    if(head == NULL || head->next == NULL) {
        return head;
    }

    struct Node* slow = head;
    struct Node* fast = head;

    while(fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

int main() {
    int a[6] = {1, 2, 3, 4, 5, 6};
    int n = 6;

    struct Node* head = createNode(a[0]);
    struct Node* tail = head;

    for(int i=1; i<n; i++) {
        struct Node* nextNode = createNode(a[i]);
        tail->next = nextNode;
        tail = nextNode;
    }

    struct Node* middle = middleNode(head);
    printf("%d\n", middle->data);

    struct Node* temp = head;
    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}