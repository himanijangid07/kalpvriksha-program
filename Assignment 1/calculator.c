#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    char expr[100];
    printf("Enter an arithmetic expression: ");
    fgets(expr, sizeof(expr), stdin);

    int nums[100];
    char ops[100];
    int numCount = 0, opCount = 0;
    int i = 0, num = 0;

    while (expr[i] != '\0')
    {
        if (expr[i] == ' ')
        {
            i++;
            continue;
        }
        else if (isdigit(expr[i]))
        {
            num = num * 10 + (expr[i] - '0');
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')
        {
            nums[numCount++] = num;
            num = 0;
            ops[opCount++] = expr[i];
        }
        else if (expr[i] == '\n')
        {
            break;
        }
        else
        {
            printf("Error: Invalid expression.\n");
            return 1;
        }
        i++;
    }
    nums[numCount++] = num;

    for (i = 0; i < opCount; i++)
    {
        if (ops[i] == '*' || ops[i] == '/')
        {
            if (ops[i] == '*')
            {
                nums[i] = nums[i] * nums[i + 1];
            }
            else
            {
                if (nums[i + 1] == 0)
                {
                    printf("Error: Division by zero.\n");
                    return 1;
                }
                nums[i] = nums[i] / nums[i + 1];
            }
            for (int j = i + 1; j < numCount - 1; j++)
                nums[j] = nums[j + 1];
            for (int j = i; j < opCount - 1; j++)
                ops[j] = ops[j + 1];
            numCount--;
            opCount--;
            i--;
        }
    }

    int result = nums[0];
    for (i = 0; i < opCount; i++)
    {
        if (ops[i] == '+')
            result += nums[i + 1];
        else
            result -= nums[i + 1];
    }

    printf("Result: %d\n", result);
    return 0;
}