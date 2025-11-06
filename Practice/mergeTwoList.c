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

struct Node* mergeTwoLists(struct Node* head1, struct Node* head2) {
    struct Node result;
    result.next = NULL;
    struct Node* temp = &result;

    struct Node* temp1 = head1;
    struct Node* temp2 = head2;

    while(temp1 != NULL && temp2 != NULL) {
        if(temp1->data < temp2->data) {
            temp->next = temp1;
            temp1 = temp1->next;
        } else {
            temp->next = temp2;
            temp2 = temp2->next;
        }

        temp = temp->next;
    }
    if (temp1)
        temp->next = temp1;
    else
        temp->next = temp2;

    return result.next;
}

void printList(struct Node* temp) {
    while(temp != NULL) {
        printf("%d->", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

int main() {
    int a[5] = {1, 3, 4, 6, 7};
    int b[5] = {1, 2, 5, 7, 8};

    struct Node* head1 = createNode(a[0]);
    struct Node* tail1 = head1;

    struct Node* head2 = createNode(b[0]);
    struct Node* tail2 = head2;

    for(int i=1; i<5; i++) {
        struct Node* nextNode = createNode(a[i]);
        tail1->next = nextNode;
        tail1 = nextNode;
    }

    for(int i=1; i<5; i++) {
        struct Node* nextNode = createNode(b[i]);
        tail2->next = nextNode;
        tail2 = nextNode;
    }

    struct Node* newHead = mergeTwoLists(head1, head2);
    printList(newHead);

    struct Node* temp = newHead;
    while(temp != NULL) {
        struct Node* next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}