#include <stdio.h>
#include <string.h>

struct Student
{
    int rollNo;
    char name[50];
    int marks[3];
    int totalMarks;
    float averageMarks;
    char grade;
};

char calculateGrade(float averageMarks) {
    if(averageMarks >= 85)
    {
        return 'A';
    } else if(averageMarks >= 70)
    {
        return 'B';
    } else if(averageMarks >= 50)
    {
        return 'C';
    } else if(averageMarks >= 35)
    {
        return 'D';
    } else
    {
        return 'F';
    }
}

void printRollNumbers(struct Student students[], int rollCount, int numberOfStudents) {
    if(rollCount == numberOfStudents)
    {
        return;
    }
    printf("%d ", students[rollCount].rollNo);

    printRollNumbers(students, rollCount + 1, numberOfStudents);
}

int main()
{
    int numberOfStudents, index, subjectIndex, rollCount;
    printf("Enter number of students: ");
    scanf("%d", &numberOfStudents);

    struct Student students[numberOfStudents];

    for (index = 0; index < numberOfStudents; index++)
    {
        printf("Enter details for student %d\n", index + 1);
        printf("Enter Roll Number: ");
        scanf("%d", &students[index].rollNo);
        printf("Enter Name: ");
        getchar();
        fgets(students[index].name, sizeof(students[index].name), stdin);
        students[index].name[strcspn(students[index].name, "\n")] = 0;

        students[index].totalMarks = 0;

        for (subjectIndex = 0; subjectIndex < 3; subjectIndex++)
        {
            printf("Enter marks for subject %d: ", subjectIndex + 1);
            scanf("%d", &students[index].marks[subjectIndex]);
            students[index].totalMarks += students[index].marks[subjectIndex];
        }

        students[index].averageMarks = students[index].totalMarks / 3.0;
        students[index].grade = calculateGrade(students[index].averageMarks);
    }
    printf("--------Student Reports--------\n");
    for (index = 0; index < numberOfStudents; index++)
    {
        printf("Roll Number: %d\n", students[index].rollNo);
        printf("Name: %s\n", students[index].name);
        printf("Total Marks: %d\n", students[index].totalMarks);
        printf("Average Marks: %.2f\n", students[index].averageMarks);
        printf("Grade: %c\n", students[index].grade);
        
        switch (students[index].grade)
        {
            case 'A':
                printf("Performance: *****\n");
                break;
            case 'B':
                printf("Performance: ****\n");
                break;
            case 'C':
                printf("Performance: ***\n");
                break;
            case 'D':
                printf("Performance: **\n");
                break;
            case 'F':
                break;
            default:
                printf("Invalid Grade\n");
        }
    }
    
    printf("List of Roll Numbers: ");
    printRollNumbers(students, 0, numberOfStudents);
    printf("\n");

    return 0;
}