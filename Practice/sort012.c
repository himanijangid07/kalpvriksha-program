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

struct Node* sort012(struct Node* head) {
    struct Node zeroHead = {0 , NULL};
    struct Node oneHead = {0, NULL};
    struct Node twoHead = {0, NULL};

    struct Node* zero = &zeroHead;
    struct Node* one = &oneHead;
    struct Node* two = &twoHead;

    struct Node* temp = head;

    while(temp != NULL) {
        if(temp->data == 0) {
            zero->next = temp;
            zero = zero->next;
        } else if(temp->data == 1) {
            one->next = temp;
            one = one->next;
        } else {
            two->next = temp;
            two = two->next;
        }
        temp = temp->next;
    }

    zero->next = (oneHead.next) ? oneHead.next : twoHead.next;
    one->next = twoHead.next;
    two->next = NULL;

    return zeroHead.next;
}

void printList(struct Node* temp) {
    while(temp != NULL) {
        printf("%d->", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

int main() {
    int arr[7] = {1, 0, 1, 2, 0, 2, 1};
    int number = 7;

    struct Node* head = createNode(arr[0]);
    struct Node* tail = head;

    for(int index=1; index<number; index++) {
        struct Node* nextNode = createNode(arr[index]);
        tail->next = nextNode;
        tail = nextNode;
    }

    printf("Original List: ");
    printList(head);
    
    head = sort012(head);
    printf("After sorting: ");
    printList(head);

    struct Node* temp = head;

    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}