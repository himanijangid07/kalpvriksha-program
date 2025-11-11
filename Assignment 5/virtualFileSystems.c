#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 5000
#define MAX_LEN 50

struct FreeBlock {
    int index;
    struct FreeBlock* next;
    struct FreeBlock* prev;
};

struct FileNode {
    int size;
    int isDirectory;
    char name[MAX_LEN];
    int blockSize[10]; 

    struct FileNode* parent;
    struct FileNode* child;
    struct FileNode* next;
    struct FileNode* prev;
};

void stringCopy(char destination[], const char source[]) {
    int index = 0;
    while (source[index] != '\0' && index < MAX_LEN - 1) {
        destination[index] = source[index];
        index++;
    }
    destination[index] = '\0';
}

int stringCompare(const char firstString[], const char secondString[]) {
    int index = 0;
    while (firstString[index] != '\0' && secondString[index] != '\0')
    {
        if (firstString[index] != secondString[index])
            return (firstString[index] - secondString[index]);
        index++;
    }
    return (firstString[index] - secondString[index]);
}

int stringLength(const char source[]) {
    int length = 0;
    while (source[length] != '\0')
        length++;
    return length;
}

void stringConcat(char destination[], const char source[]) {
    int firstIndex = 0, secondIndex = 0;
    while (destination[firstIndex] != '\0')
        firstIndex++;
    while (source[secondIndex] != '\0') {
        destination[firstIndex++] = source[secondIndex++];
        if (firstIndex >= MAX_LEN - 1)
            break; 
    }
    destination[firstIndex] = '\0';
}

void readLine(char buffer[], int size) {
    int index = 0;
    int ch = getchar();
    if (ch == '\n')
        ch = getchar();
    while (ch != '\n' && ch != EOF && index < size - 1)
    {
        buffer[index++] = (char)ch;
        ch = getchar();
    }
    buffer[index] = '\0';
}

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];

struct FreeBlock* freeListHead = NULL;
struct FreeBlock* freeListTail = NULL;

struct FileNode* root = NULL;
struct FileNode* cwd = NULL;

struct FreeBlock* createFreeBlockNode(int index) {
    struct FreeBlock* newNode = (struct FreeBlock *)malloc(sizeof(struct FreeBlock));
    
    if (newNode == NULL) {
        printf("Error: memory allocation failed (FreeBlock)\n");
        exit(1);
    }
    newNode->index = index;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void addFreeBlockToTail(int index) {
    struct FreeBlock* newNode = createFreeBlockNode(index);

    if (freeListHead == NULL) {
        freeListHead = newNode;
        freeListTail = newNode;
    }
    else {
        freeListTail->next = newNode;
        newNode->prev = freeListTail;
        freeListTail = newNode;
    }
}

int removeFreeBlockFromHead() {
    if (freeListHead == NULL)
        return -1;

    int index = freeListHead->index;
    struct FreeBlock* temp = freeListHead;
    freeListHead = freeListHead->next;

    if (freeListHead != NULL) {
        freeListHead->prev = NULL;
    } else {
        freeListTail = NULL;
    }
    
    free(temp);

    return index;
}

int countFreeBlocks(struct FreeBlock *head) {
    int count = 0;
    struct FreeBlock* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

void freeFreeList(struct FreeBlock *head) {
    struct FreeBlock* temp = head;

    while (temp != NULL) {
        struct FreeBlock* next = temp->next;
        free(temp);
        temp = next;
    }
}

struct FileNode* createFileNode(const char* name, int isDirectory, struct FileNode* parent) {
    struct FileNode* newNode = (struct FileNode *)malloc(sizeof(struct FileNode));

    if (newNode == NULL) {
        printf("Error: memory allocation failed (FileNode)\n");
        exit(1);
    }

    stringCopy(newNode->name, name);
    newNode->isDirectory = isDirectory;
    newNode->parent = parent;
    newNode->child = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->size = 0;

    for (int index = 0; index < 10; ++index) {
        newNode->blockSize[index] = -1;
    }

    return newNode;
}

struct FileNode* insertIntoDirectory(struct FileNode **head, struct FileNode* node) {
    if (head == NULL)
        return NULL;
    
    if (*head == NULL) {
        *head = node;
        node->next = node;
        node->prev = node;
    } else {
        struct FileNode *tail = (*head)->prev;
        tail->next = node;
        node->prev = tail;
        node->next = *head;
        (*head)->prev = node;
    }

    return *head;
}

struct FileNode* findIntoDirectory(struct FileNode* directory, char* name) {
    if (directory == NULL || directory->child == NULL)
        return NULL;
    
    struct FileNode *temp = directory->child;
    
    do {
        if (stringCompare(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    } while (temp != directory->child);
    
    return NULL;
}

void initFileSystem() {
    for (int index = 0; index < NUM_BLOCKS; ++index)
        for (int iterator = 0; iterator < BLOCK_SIZE; ++iterator) {
            virtualDisk[index][iterator] = '\0';
        }

    freeListHead = freeListTail = NULL;

    for (int iterator = 0; iterator < NUM_BLOCKS; ++iterator) {
        addFreeBlockToTail(iterator);
    }

    root = createFileNode("/", 1, NULL);
    cwd = root;
}

void mkdirCommand(char *name) {
    if (name == NULL || name[0] == '\0') {
        printf("Error: Directory name cannot be empty\n");
        return;
    }

    for (int index = 0; name[index] != '\0'; ++index) {
        if (name[index] == ' ' || name[index] == '/' || name[index] == '\\') {
            printf("Error: Invalid character '%c' in directory name\n", name[index]);
            return;
        }
    }

    if (stringLength(name) >= MAX_LEN) {
        printf("Error: Directory name too long (max %d chars)\n", MAX_LEN - 1);
        return;
    }

    if (findIntoDirectory(cwd, name) != NULL) {
        printf("Error: Directory '%s' already exists\n", name);
        return;
    }

    struct FileNode *newDir = createFileNode(name, 1, cwd);
    insertIntoDirectory(&cwd->child, newDir);
    printf("Directory '%s' created successfully\n", name);
}

void createCommand(char *name) {
    if (name == NULL || name[0] == '\0') {
        printf("Error: File name cannot be empty\n");
        return;
    }

    if (findIntoDirectory(cwd, name) != NULL) {
        printf("File already exists.\n");
        return;
    }

    struct FileNode *file = createFileNode(name, 0, cwd);
    insertIntoDirectory(&cwd->child, file);
    printf("File '%s' created successfully.\n", name);
}

void writeCommand(char *name, char *content) {
    struct FileNode *file = findIntoDirectory(cwd, name);

    if (file == NULL || file->isDirectory) {
        printf("File not found\n");
        return;
    }

    for (int index = 0; index < 10 && file->blockSize[index] != -1; ++index) {
        addFreeBlockToTail(file->blockSize[index]);
        file->blockSize[index] = -1;
    }

    int length= stringLength(content);
    int requiredBlocks = (length == 0) ? 0 : ((length + BLOCK_SIZE - 1) / BLOCK_SIZE);

    if (requiredBlocks == 0) {
        file->size = 0;
        printf("Data written successfully (size = 0 bytes)\n");
        return;
    }

    if (requiredBlocks > countFreeBlocks(freeListHead)) {
        printf("Error: not enough free blocks\n");
        return;
    }

    int offset = 0;
    for (int index = 0; index < requiredBlocks && index < 10; ++index) {
        int blockNum = removeFreeBlockFromHead();
        if (blockNum == -1) { 
            break;
        }

        int wrote = 0;
        for (; wrote < BLOCK_SIZE && offset + wrote < length; ++wrote) {
            virtualDisk[blockNum][wrote] = content[offset + wrote];
        }

        if (wrote < BLOCK_SIZE)
            virtualDisk[blockNum][wrote] = '\0';
        else
            virtualDisk[blockNum][BLOCK_SIZE - 1] = '\0';
        file->blockSize[index] = blockNum;
        offset += BLOCK_SIZE;
    }

    file->size = length;

    printf("Data written successfully (size = %d bytes)\n", length);
}

void readCommand(char *name) {
    struct FileNode *file = findIntoDirectory(cwd, name);
    if (file == NULL || file->isDirectory) {
        printf("File not found\n");
        return;
    }

    printf("Content of %s is:\n", name);
    for (int index = 0; index < 10 && file->blockSize[index] != -1; ++index) {
        int idx = file->blockSize[index];
        int iterator = 0;

        while (iterator < BLOCK_SIZE && virtualDisk[idx][iterator] != '\0') {
            putchar(virtualDisk[idx][iterator]);
            ++iterator;
        }
    }
    printf("\n");
}

void deleteCommand(char *name) {
    struct FileNode *file = findIntoDirectory(cwd, name);
    if (file == NULL || file->isDirectory) {
        printf("File not found\n");
        return;
    }

    for (int index = 0; index < 10 && file->blockSize[index] != -1; ++index) {
        addFreeBlockToTail(file->blockSize[index]);
        file->blockSize[index] = -1;
    }

    if (file->next == file) {
        cwd->child = NULL;
    } else {
        file->next->prev = file->prev;
        file->prev->next = file->next;
        if (cwd->child == file) {
            cwd->child = file->next;
        }
    }

    free(file);

    printf("File deleted successfully.\n");
}

void rmdirCommand(char *name) {
    struct FileNode *dir = findIntoDirectory(cwd, name);
    if (dir == NULL || !dir->isDirectory) {
        printf("Directory not found\n");
        return;
    }

    if (dir->child != NULL) {
        printf("Directory not empty. Remove files first\n");
        return;
    }

    if (dir->next == dir) {
        cwd->child = NULL;
    } else {
        dir->next->prev = dir->prev;
        dir->prev->next = dir->next;
        if (cwd->child == dir) {
            cwd->child = dir->next;
        }
    }

    free(dir);
    printf("Directory removed successfully\n");
}

void lsCommand() {
    if (cwd->child == NULL) {
        printf("(empty)\n");
        return;
    }

    struct FileNode *temp = cwd->child;
    do {
        if (temp->isDirectory) {
            printf("%s/\n", temp->name);
        } else {
            printf("%s\n", temp->name);
        }
        temp = temp->next;
    } while (temp != cwd->child);
}

void cdCommand(char *name) {
    if (stringCompare(name, "..") == 0) {
        if (cwd->parent != NULL) {
            cwd = cwd->parent;
            if (cwd == root) {
                printf("Moved to /\n");
            } else {
                char buf[500] = "";
                struct FileNode *temp = cwd;

                while (temp != NULL && temp->parent != NULL) {
                    char seg[100] = "/";
                    stringConcat(seg, temp->name); 
                    stringConcat(seg, buf);   
                    stringCopy(buf, seg);
                    temp = temp->parent;
                }
                printf("Moved to %s\n", (buf[0] == '\0') ? "/" : buf);
            }
        } else {
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
    if (cwd == root) {
        printf("Moved to /\n");
    } else {
        char buf[500] = "";
        struct FileNode *t = cwd;
        while (t != NULL && t->parent != NULL) {
            char seg[100] = "/";
            stringConcat(seg, t->name);
            stringConcat(seg, buf);
            stringCopy(buf, seg);
            t = t->parent;
        }
        printf("Moved to %s\n", (buf[0] == '\0') ? "/" : buf);
    }
}

void pwdCommand() {
    char path[500] = "";
    struct FileNode *temp = cwd;
    if (cwd == root) {
        printf("/\n");
        return;
    }

    while (temp != NULL && temp->parent != NULL) {
        char seg[100] = "/";
        stringConcat(seg, temp->name);
        stringConcat(seg, path);    
        stringCopy(path, seg);
        temp = temp->parent;
    }
    printf("%s\n", path[0] == '\0' ? "/" : path);
}

void dfCmd() {
    int freeBlocks = countFreeBlocks(freeListHead);
    int usedBlocks = NUM_BLOCKS - freeBlocks;
    float diskUsage = ((float)usedBlocks / (float)NUM_BLOCKS) * 100.0f;
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", diskUsage);
}

void freeDirectoryTree(struct FileNode *node) {
    if (node == NULL)
        return;
    if (node->child != NULL) {
        struct FileNode *start = node->child;
        struct FileNode *tmp = start;
        do {
            struct FileNode *next = tmp->next;
            tmp->next = tmp->prev = NULL;
            freeDirectoryTree(tmp);
            tmp = next;
        } while (tmp != start);
    }
    free(node);
}

void exitCommand() {
    freeFreeList(freeListHead);
    freeListHead = NULL;
    freeListTail = NULL;
    freeDirectoryTree(root);
    root = NULL;
    cwd = NULL;
    printf("Memory released. Exiting program...\n");
    exit(0);
}

void printPrompt(struct FileNode *dir) {
    char path[500] = "";
    if (dir == root) {
        printf("/ > ");
        return;
    }

    struct FileNode *current = dir;
    while (current != NULL && current->parent != NULL) {
        char seg[100] = "/";
        stringConcat(seg, current->name); 
        stringConcat(seg, path);      
        stringCopy(path, seg);
        current = current->parent;
    }

    if (path[0] == '\0')
        printf("/ > ");
    else
        printf("%s > ", path);
}

int main() {
    initFileSystem();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char command[100], arg1[50], arg2[512];

    while (1) {
        printPrompt(cwd);
        if (scanf("%99s", command) != 1) {
            break;
        }

        if (stringCompare(command, "mkdir") == 0) {
            if (scanf("%49s", arg1) != 1) {
                printf("Missing directory name\n");
                continue;
            }
            mkdirCommand(arg1);
        } else if (stringCompare(command, "create") == 0) {
            if (scanf("%49s", arg1) != 1) {
                printf("Missing file name\n");
                continue;
            }
            createCommand(arg1);
        } else if (stringCompare(command, "write") == 0) {
            if (scanf("%49s", arg1) != 1) {
                printf("Missing filename\n");
                continue;
            }
            readLine(arg2, sizeof(arg2));
            writeCommand(arg1, arg2);
        } else if (stringCompare(command, "read") == 0) {
            if (scanf("%49s", arg1) != 1) {
                printf("Missing filename\n");
                continue;
            }
            readCommand(arg1);
        } else if (stringCompare(command, "delete") == 0) {
            if (scanf("%49s", arg1) != 1) {
                printf("Missing filename\n");
                continue;
            }
            deleteCommand(arg1);
        } else if (stringCompare(command, "rmdir") == 0) {
            if (scanf("%49s", arg1) != 1) {
                printf("Missing directory name\n");
                continue;
            }
            rmdirCommand(arg1);
        } else if (stringCompare(command, "ls") == 0) {
            lsCommand();
        } else if (stringCompare(command, "cd") == 0) {
            if (scanf("%49s", arg1) != 1) {
                printf("Missing directory name\n");
                continue;
            }
            cdCommand(arg1);
        } else if (stringCompare(command, "pwd") == 0) {
            pwdCommand();
        } else if (stringCompare(command, "df") == 0) {
            dfCmd();
        } else if (stringCompare(command, "exit") == 0) {
            exitCommand();
        } else {
            printf("Unknown command\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
        }
    }

    exitCommand();
    return 0;
}