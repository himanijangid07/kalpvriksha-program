#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    char expression[100];
    printf("Enter an arithmetic expression: ");
    fgets(expression, sizeof(expression), stdin);

    int numbers[100];
    char operators[100];
    int numberCount = 0, operatorCount = 0;
    int index = 0, currentNumber = 0;

    while (expression[index] != '\0')
    {
        if (expression[index] == ' ')
        {
            index++;
            continue;
        }
        else if (isdigit(expression[index]))
        {
            currentNumber = currentNumber * 10 + (expression[index] - '0');
        }
        else if (expression[index] == '+' || expression[index] == '-' || expression[index] == '*' || expression[index] == '/')
        {
            numbers[numberCount++] = currentNumber;
            currentNumber = 0;
            operators[operatorCount++] = expression[index];
        }
        else if (expression[index] == '\n')
        {
            break;
        }
        else
        {
            printf("Error: Invalid expression.\n");
            return 1;
        }
        index++;
    }
    numbers[numberCount++] = currentNumber;

    for (index = 0; index < operatorCount; index++)
    {
        if (operators[index] == '*' || operators[index] == '/')
        {
            if (operators[index] == '*')
            {
                numbers[index] = numbers[index] * numbers[index + 1];
            }
            else
            {
                if (numbers[index + 1] == 0)
                {
                    printf("Error: Division by zero.\n");
                    return 1;
                }
                numbers[index] = numbers[index] / numbers[index + 1];
            }
            for (int j = index + 1; j < numberCount - 1; j++)
                numbers[j] = numbers[j + 1];
            for (int j = index; j < operatorCount - 1; j++)
                operators[j] = operators[j + 1];
            numberCount--;
            operatorCount--;
            index--;
        }
    }

    int result = numbers[0];
    for (index = 0; index < operatorCount; index++)
    {
        if (operators[index] == '+')
            result += numbers[index + 1];
        else
            result -= numbers[index + 1];
    }

    printf("Result: %d\n", result);
    return 0;
}