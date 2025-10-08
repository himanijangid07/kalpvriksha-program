#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    int id;
    char name[50];
    int age;
};

void createFile()
{
    FILE *file = fopen("users.txt", "a");
    if (file == NULL)
    {
        printf("Error creating file.\n");
        return;
    }
    fclose(file);
}

void addUser()
{
    struct User userDetails;
    FILE *file = fopen("users.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &userDetails.id);
    getchar();
    printf("Enter Name: ");
    fgets(userDetails.name, sizeof(userDetails.name), stdin);
    userDetails.name[strcspn(userDetails.name, "\n")] = '\0';
    printf("Enter Age: ");
    scanf("%d", &userDetails.age);

    fprintf(file, "%d|%s|%d\n", userDetails.id, userDetails.name, userDetails.age);
    fclose(file);
    printf("User added successfully.\n");
}

void readUsers()
{
    struct User userDetails;
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("No users found.\n");
        return;
    }
    printf("\nID\tName\t\tAge\n");
    printf("---------------------------------\n");

    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%d|%49[^|]|%d", &userDetails.id, userDetails.name, &userDetails.age) == 3)
        {
            printf("%d\t%-15s\t%d\n", userDetails.id, userDetails.name, userDetails.age);
        }
    }
    fclose(file);
}

void deleteUser(int id)
{
    struct User userDetails;
    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%d|%49[^|]|%d", &userDetails.id, userDetails.name, &userDetails.age) == 3)
        {
            if (userDetails.id != id)
            {
                fprintf(tempFile, "%d|%s|%d\n", userDetails.id, userDetails.name, userDetails.age);
            }
            else
            {
                found = 1;
            }
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User with ID %d deleted successfully.\n", id);
    else
        printf("User with ID %d not found.\n", id);
}

void updateUser(int id)
{
    struct User userDetails;
    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%d|%49[^|]|%d", &userDetails.id, userDetails.name, &userDetails.age) == 3)
        {
            if (userDetails.id == id)
            {
                found = 1;
                printf("Enter new Name: ");
                getchar(); // clear newline
                fgets(userDetails.name, sizeof(userDetails.name), stdin);
                userDetails.name[strcspn(userDetails.name, "\n")] = '\0';
                printf("Enter new Age: ");
                scanf("%d", &userDetails.age);
            }
            fprintf(tempFile, "%d|%s|%d\n", userDetails.id, userDetails.name, userDetails.age);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User with ID %d updated successfully.\n", id);
    else
        printf("User with ID %d not found.\n", id);
}

int main()
{
    createFile();
    int choice, id;

    while (1)
    {
        printf("\n--- User Management System ---\n");
        printf("1. Add User\n");
        printf("2. View Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addUser();
            break;
        case 2:
            readUsers();
            break;
        case 3:
            printf("Enter ID to update: ");
            scanf("%d", &id);
            updateUser(id);
            break;
        case 4:
            printf("Enter ID to delete: ");
            scanf("%d", &id);
            deleteUser(id);
            break;
        case 5:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}