#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 5000
#define maxLen 50

struct FreeBlock {
    int index;
    struct FreeBlock* next;
    struct FreeBlock* prev;
};

struct FileNode {
    int size;
    int isDirectory;
    char name[maxLen];
    int blockSize[10];

    struct FileNode* parent;
    struct FileNode* child;
    struct FileNode* next;
    struct FileNode* prev;
};

void stringCopy(char destination[], char source[]) {
    int index = 0;

    while(source[index] != '\0') {
        destination[index] = source[index];
        index++;
    }

    destination[index] = '\0';
}

int stringCompare(char firstString[], char secondString[]) {
    int index = 0;

    while (firstString[index] != '\0' && secondString[index] != '\0') {
        if (firstString[index] != secondString[index]) {
            return firstString[index] - secondString[index];
        }
        index++;
    }

    return firstString[index] - secondString[index];
}

int stringLength(char source[]) {
    int length = 0;

    while(source[length] != '\0') {
        length++;
    }

    return length;
}

void stringConcate(char source[], char destination[]) {
    int firstIndex = 0, secondIndex = 0;

    while(destination[secondIndex] != '\0') {
        secondIndex++;
    }

    while(source[firstIndex] != '\0') {
        destination[secondIndex++] = source[firstIndex++];
    }

    destination[secondIndex] = '\0';
}

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];

struct FreeBlock* freeListHead = NULL;
struct FreeBlock* freeListTail = NULL;

struct FileNode* root = NULL;
struct FileNode* cwd = NULL;

struct FreeBlock* createFreeBlockNode(int index) {
    struct FreeBlock* freeBlockNode = (struct FreeBlock*)malloc(sizeof(struct FreeBlock));
    freeBlockNode->index = index;
    freeBlockNode->next = NULL;
    freeBlockNode->prev = NULL;
    return freeBlockNode;
}

void addFreeBlockToTail(int index) {
    struct FreeBlock* newNode = createFreeBlockNode(index);

    if(freeListHead == NULL) {
        freeListHead = newNode;
        freeListTail = newNode;
    } else {
        freeListTail->next = newNode;
        newNode->prev = freeListTail;
        freeListTail = newNode;
    }
}

int removeFreeBlockFromHead() {
    if(freeListHead == NULL) {
        return -1;
    }

    int index = freeListHead->index;
    struct FreeBlock* temp = freeListHead;
    freeListHead = freeListHead->next;

    if(freeListHead != NULL) {
        freeListHead->prev = NULL; 
    } else {
        freeListTail = NULL;
    }

    free(temp);
    return index;
}

int countFreeBlocks(struct FreeBlock* head) {
    struct FreeBlock* temp = head;
    int count = 0;

    while(temp != NULL) {
        count++;
        temp = temp->next;
    }

    return count;
}

void freeFreeList(struct FreeBlock* head) {
    struct FreeBlock* temp = head;

    while(temp != NULL) {
        struct FreeBlock* next = temp->next;
        free(temp);
        temp = next;
    }
}

struct FileNode* createFileNode(char* name, int isDirectory, struct FileNode* parent) {
    struct FileNode* newNode = (struct FileNode*)malloc(sizeof(struct FileNode));
    stringCopy(newNode->name, name);
    newNode->isDirectory = isDirectory;
    newNode->parent = parent;
    newNode->child = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->size = 0;

    for(int index = 0; index < 10; index++) {
        newNode->blockSize[index] = -1;
    }

    return newNode;
};

struct FileNode* insertIntoDirectory(struct FileNode** head, struct FileNode* node) {
    if(*head == NULL) {
        *head = node;
        node->next = node;
        node->prev = node;
    } else {
        struct FileNode* tail = (*head)->prev;
        tail->next = node;
        node->prev = tail;
        node->next = (*head);
        (*head)->prev = node;
    }

    return *head;
}

struct FileNode* findIntoDirectory(struct FileNode* directory, char* name) {
    if(directory->child == NULL) return NULL;

    struct FileNode* temp = directory->child;

    do {
        if(stringCompare(temp->name, name) == 0) return temp;
        temp = temp->next;
    } while(temp != directory->child);

    return NULL;
}

void initFileSystem() {
    for(int firstIndex = 0; firstIndex < NUM_BLOCKS; firstIndex++) {
        for(int secondIndex = 0; secondIndex < BLOCK_SIZE; secondIndex++) {
            virtualDisk[firstIndex][secondIndex] = '\0';
        }
    }

    for(int index = 0; index < NUM_BLOCKS; index++) {
        addFreeBlockToTail(index);
    }

    root = createFileNode("/", 1, NULL);
    cwd = root;
}

void mkdirCommand(char* name) {
    if(findIntoDirectory(cwd, name)) {
        printf("Directory already exists.\n");
        return;
    }

    struct FileNode* dir = createFileNode(name, 1, cwd);
    insertIntoDirectory(&cwd->child, dir);

    printf("Directory '%s' created successfully\n", name);
}

void createCommand(char* name) {
    if(findIntoDirectory(cwd, name)) {
        printf("File already exists.\n");
        return;
    }

    struct FileNode* file = createFileNode(name, 0, cwd);
    insertIntoDirectory(&cwd->child, file);

    printf("File '%s' created successfully.\n", name);
}

void writeCommand(char* name, char* content) {
    struct FileNode* file = findIntoDirectory(cwd, name);
    if(file == NULL || file->isDirectory) {
        printf("File not found.\n");
        return;
    }

    int contentLength = stringLength(content);
    int requiredBlocks = (contentLength + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if(requiredBlocks > countFreeBlocks(freeListHead)) {
        printf("Error: not enough free blocks\n");
        return;
    }

    int offset = 0;

    for(int index = 0; index < requiredBlocks; index++) {
        int blockIndex = removeFreeBlockFromHead();
        if(blockIndex == -1) break;

        for(int iterator = 0; iterator < BLOCK_SIZE && offset + iterator < contentLength; iterator++) {
            virtualDisk[blockIndex][iterator] = content[offset + iterator];
        }

        file->blockSize[index] = blockIndex;
        offset = offset + BLOCK_SIZE;
    }

    file->size = contentLength;

    printf("Data written successfully (size = %d bytes)\n", contentLength);
}

void readCommand(char* name) {
    struct FileNode* file = findIntoDirectory(cwd, name);
    if(file == NULL || file->isDirectory) {
        printf("File not found\n");
        return;
    }

    printf("Content of %s is:\n", name);

    for(int index = 0; index < 10 && file->blockSize[index] != -1; index++) {
        printf("%s", virtualDisk[file->blockSize[index]]);
    }
    printf("\n");
}

void deleteCommand(char* name) {
    struct FileNode* file = findIntoDirectory(cwd, name);
    if(file == NULL || file->isDirectory) {
        printf("File not found\n");
        return;
    }

    for(int index = 0; index < 10 && file->blockSize[index] != -1; index++) {
        addFreeBlockToTail(file->blockSize[index]);
        file->blockSize[index] = -1;
    }

    if(file->next == file) {
        cwd->child = NULL;
    } else {
        file->next->prev = file->prev;
        file->prev->next = file->next;
        if(cwd->child == file) {
            cwd->child = file->next;
        }
    }

    free(file);
    printf("File deleted successfully.\n");
}

void rmdirCommand(char* name) {
    struct FileNode* dir = findIntoDirectory(cwd, name);
    if(dir == NULL || !dir->isDirectory) {
        printf("Directory not found\n");
        return;
    }

    if(dir->child != NULL) {
        printf("Directory not empty. Remove files first\n");
        return;
    }

    if(dir->next == dir) {
        cwd->child = NULL;
    } else {
        dir->next->prev = dir->prev;
        dir->prev->next = dir->next;
        if(cwd->child == dir) {
            cwd->child = dir->next;
        }
    }

    free(dir);
    printf("Directory removed successfully\n");
}

void lsCommand() {
    if(cwd->child == NULL) {
        printf("(empty)\n");
        return;
    }

    struct FileNode* temp = cwd->child;
    do {
        printf(temp->isDirectory ? "%s/\n" : "%s\n", temp->name);
        temp = temp->next;
    } while(temp != cwd->child);
}

void cdCommand(char *name) {
    if (stringCompare(name, "..") == 0) {
        if (cwd->parent != NULL) {
            cwd = cwd->parent;
            printf("Moved to /%s\n", cwd == root ? "" : cwd->name);
        }
        else {
            printf("Already at root directory.\n");
        }
        return;
    }

    struct FileNode *dir = findIntoDirectory(cwd, name);
    if (dir == NULL || !dir->isDirectory) {
        printf("Directory not found\n");
        return;
    }

    cwd = dir;
    printf("Moved to /%s\n", cwd == root ? "" : cwd->name);
}

void pwdCommand() {
    struct FileNode* temp = cwd;
    char tempPath[500] = "";

    while(temp != NULL) {
        char nameWithSlash[50] = "";
        stringCopy(nameWithSlash, temp->name);

        if(temp->parent != NULL) {
            stringCopy(nameWithSlash, "/");
        }

        stringConcate(nameWithSlash, tempPath);
        stringCopy(tempPath, nameWithSlash);

        temp = temp->parent;
    }

    printf("/%s\n", tempPath[0] == '/' ? tempPath + 1 : tempPath);
}

void dfCommand() {
    int freeBlocks = countFreeBlocks(freeListHead);
    int usedBlocks = NUM_BLOCKS - freeBlocks;
    int diskUsage = (((float)NUM_BLOCKS - freeBlocks) / NUM_BLOCKS) * 100;

    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %d\n", NUM_BLOCKS, usedBlocks, freeBlocks, diskUsage);
}

void exitCommand() {
    printf("Memory released. Exiting program...\n");
    exit(0);
}

void printPrompt(struct FileNode *dir) {
    if (dir == root)
        printf("/ > ");
    else
        printf("%s > ", dir->name);
}

int main() {
    printf("$ ./vfs\n");
    initFileSystem();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char command[100], arg1[50], arg2[512];

    while(1) {
        printf("\n");
        printPrompt(cwd);
        scanf("%s", command);

        if(stringCompare(command, "mkdir") == 0) {
            scanf("%s", arg1);
            mkdirCommand(arg1);
        } else if(stringCompare(command, "create") == 0) {
            scanf("%s", arg1);
            createCommand(arg1);
        } else if(stringCompare(command, "write") == 0) {
            scanf("%s", arg1);
            getchar();
            fgets(arg2, sizeof(arg2), stdin);
            writeCommand(arg1, arg2);
        } else if(stringCompare(command, "read") == 0) {
            scanf("%s", arg1);
            readCommand(arg1);
        } else if(stringCompare(command, "delete") == 0) {
            scanf("%s", arg1);
            deleteCommand(arg1);
        } else if(stringCompare(command, "rmdir") == 0) {
            scanf("%s", arg1);
            rmdirCommand(arg1);
        } else if(stringCompare(command, "ls") == 0) {
            lsCommand();
        } else if(stringCompare(command, "cd") == 0) {
            scanf("%s", arg1);
            cdCommand(arg1);
        } else if(stringCompare(command, "pwd") == 0) {
            pwdCommand();
        } else if(stringCompare(command, "df") == 0) {
            dfCommand();
        } else if(stringCompare(command, "exit") == 0) {
            exitCommand();
        } else {
            printf("Unknown command\n");
        }
    }

    return 0;
}