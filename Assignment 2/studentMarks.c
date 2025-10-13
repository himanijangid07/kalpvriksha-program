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
    int numberOfStudents, studentIterator, subjectIterator;
    printf("Enter number of students: ");
    scanf("%d", &numberOfStudents);

    struct Student students[numberOfStudents];

    for (studentIterator = 0; studentIterator < numberOfStudents; studentIterator++)
    {
        scanf("%d", &students[studentIterator].rollNo);
        getchar();
        fgets(students[studentIterator].name, sizeof(students[studentIterator].name), stdin);
        students[studentIterator].name[strcspn(students[studentIterator].name, "\n")] = 0;

        students[studentIterator].totalMarks = 0;

        for (subjectIterator = 0; subjectIterator < 3; subjectIterator++)
        {
            scanf("%f", &students[studentIterator].marks[subjectIterator]);
            if(students[studentIterator].marks[subjectIterator] < 0 || students[studentIterator].marks[subjectIterator] > 100)
            {
                printf("Invalid marks entered. Please enter marks between 0 and 100.\n");
                subjectIterator--;
                continue;
            }
            students[studentIterator].totalMarks += students[studentIterator].marks[subjectIterator];
        }

        students[studentIterator].averageMarks = students[studentIterator].totalMarks / 3.0;
        students[studentIterator].grade = calculateGrade(students[studentIterator].averageMarks);
    }
    printf("--------Student Reports--------\n");
    for (studentIterator = 0; studentIterator < numberOfStudents; studentIterator++)
    {
        printf("Roll Number: %d\n", students[studentIterator].rollNo);
        printf("Name: %s\n", students[studentIterator].name);
        printf("Total Marks: %.2f\n", students[studentIterator].totalMarks);
        printf("Average Marks: %.2f\n", students[studentIterator].averageMarks);
        printf("Grade: %c\n", students[studentIterator].grade);
        
        switch (students[studentIterator].grade)
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