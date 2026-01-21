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

struct Node* addTwoLinkedLists(struct Node* head1, struct Node* head2) {
    struct Node* dummy = createNode(-1);
    struct Node* current = dummy;

    struct Node* t1 = head1;
    struct Node* t2 = head2;

    int carry = 0;

    while(t1 != NULL || t2 != NULL) {
        int sum = carry;

        if(t1) sum += t1->data;
        if(t2) sum += t2->data;

        struct Node* newNode = createNode(sum % 10);
        carry = sum / 10;

        current->next = newNode;
        current = current->next;

        if(t1) t1 = t1->next;
        if(t2) t2 = t2->next;
    }

    if(carry) {
        struct Node* newNode = createNode(carry);
        current->next = newNode;
    }

    return dummy->next;
}

void printList(struct Node* temp) {
    while(temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main() {
    struct Node* head1 = createNode(5);
    head1->next = createNode(6);
    head1->next->next = createNode(3);

    struct Node* head2 = createNode(4);
    head2->next = createNode(4);
    head2->next->next = createNode(6);

    printList(head1);
    printList(head2);

    printf("After adding: ");
    struct Node* result = addTwoLinkedLists(head1, head2);
    printList(result);

    return 0;
}