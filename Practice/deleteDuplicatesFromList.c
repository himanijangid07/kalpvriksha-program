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

struct Node* deleteDuplicates(struct Node* head) {
    if(head == NULL || head->next == NULL) return head;

    struct Node dummy;
    dummy.next = head;

    struct Node* prev = &dummy;
    struct Node* slow = head;
    struct Node* fast = head->next;

    while(fast != NULL) {
        if(slow->data == fast->data) {
            while(fast != NULL && fast->data == slow->data) {
                fast = fast->next;
            }
            prev->next = fast;
        } else {
            prev = slow;
        }

        slow = fast;

        if(fast != NULL) {
            fast = fast->next;
        }
    }
    return dummy.next;
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
    int a[10] = {1, 1, 2, 3, 3, 3, 4, 5, 5, 6};
    int n = 10;

    if(n == 0) return 0;

    struct Node* head = createNode(a[0]);
    struct Node* tail = head;

    for(int i=1; i<n; i++) {
        struct Node* nextNode = createNode(a[i]);
        tail->next = nextNode;
        tail = nextNode;
    }

    printf("Original list: ");
    printList(head);

    head = deleteDuplicates(head);
    printf("After deleting duplicates: ");
    printList(head);

    struct Node* temp = head;
    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}