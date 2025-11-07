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

int lengthOfLoop(struct Node* head) {
    if(head == NULL || head->next == NULL) return 0;

    struct Node* slow = head;
    struct Node* fast = head;

    while(fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if(slow == fast) {
            fast = fast->next;
            int length = 1;
            while(slow != fast) {
                length++;
                fast = fast->next;
            }
            return length;
        }
    }
    return 0;
}

int main() {
    struct Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    head->next->next->next->next->next = head->next->next;

    int length = lengthOfLoop(head);

    printf("Length is: %d\n", length);

    return 0;
}