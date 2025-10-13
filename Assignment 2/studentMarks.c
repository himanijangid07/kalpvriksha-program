#include <stdio.h>
#include <string.h>

struct Student
{
    int rollNo;
    char name[50];
    float marks[3];
    float totalMarks;
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
    int numberOfStudents, iterator, subjectIterator;
    printf("Enter number of students: ");
    scanf("%d", &numberOfStudents);

    struct Student students[numberOfStudents];

    for (iterator = 0; iterator < numberOfStudents; iterator++)
    {
        scanf("%d", &students[iterator].rollNo);
        getchar();
        fgets(students[iterator].name, sizeof(students[iterator].name), stdin);
        students[iterator].name[strcspn(students[iterator].name, "\n")] = 0;

        students[iterator].totalMarks = 0;

        for (subjectIterator = 0; subjectIterator < 3; subjectIterator++)
        {
            scanf("%f", &students[iterator].marks[subjectIterator]);
            if(students[iterator].marks[subjectIterator] < 0 || students[iterator].marks[subjectIterator] > 100)
            {
                printf("Invalid marks entered. Please enter marks between 0 and 100.\n");
                subjectIterator--;
                continue;
            }
            students[iterator].totalMarks += students[iterator].marks[subjectIterator];
        }

        students[iterator].averageMarks = students[iterator].totalMarks / 3.0;
        students[iterator].grade = calculateGrade(students[iterator].averageMarks);
    }
    printf("--------Student Reports--------\n");
    for (iterator = 0; iterator < numberOfStudents; iterator++)
    {
        printf("Roll Number: %d\n", students[iterator].rollNo);
        printf("Name: %s\n", students[iterator].name);
        printf("Total Marks: %.2f\n", students[iterator].totalMarks);
        printf("Average Marks: %.2f\n", students[iterator].averageMarks);
        printf("Grade: %c\n", students[iterator].grade);
        
        switch (students[iterator].grade)
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