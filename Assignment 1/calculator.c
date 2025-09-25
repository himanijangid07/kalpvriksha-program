#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

void removeWhiteSpaces(char* str) {
    int i, j = 0;
    for (i = 0; str[i]; i++) {
        if (str[i] != ' ') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

int evaluate(char *expr) {
    int numbers[MAX], numTop = -1;
    char ops[MAX];
    int opTop = -1;
    
    int i = 0;
    while (expr[i] != '\0') {
        if (isdigit(expr[i])) {
            int num = 0;
            while (isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            numbers[++numTop] = num;
        } else {
            if (expr[i] == '*' || expr[i] == '/') {
                char op = expr[i++];
                int num = 0;
                while (isdigit(expr[i])) {
                    num = num * 10 + (expr[i] - '0');
                    i++;
                }
                int a = numbers[numTop--];
                if (op == '*') {
                    numbers[++numTop] = a * num;
                } else {
                    if (num == 0) {
                        printf("Error: Division by zero\n");
                        return 0;
                    }
                    numbers[++numTop] = a / num;
                }
            } else {
                ops[++opTop] = expr[i++];
            }
        }
    }

    int result = numbers[0];
    int numIndex = 1;
    for (int j = 0; j <= opTop; j++) {
        if (ops[j] == '+') {
            result += numbers[numIndex++];
        } else if (ops[j] == '-') {
            result -= numbers[numIndex++];
        }
    }
    return result;
}

int main() {
    char expr[MAX];
    printf("Enter an expression: ");
    fgets(expr, MAX, stdin);
    expr[strcspn(expr, "\n")] = '\0';
    removeWhiteSpaces(expr);
    for (int i = 0; expr[i] != '\0'; i++){
        if (!isdigit(expr[i]) && expr[i] != '+' && expr[i] != '-' && expr[i] != '*' && expr[i] != '/'){
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }
    int result = evaluate(expr);
    printf("Result: %d\n", result);
    return 0;
}