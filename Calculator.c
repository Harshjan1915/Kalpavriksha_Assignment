#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define MAX_EXPRESSION_SIZE 200
#define MAX_STACK_SIZE 100

int getOperatorPrecedence(char operatorSymbol) {
    switch (operatorSymbol) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default: return 0;
    }
}

int performOperation(int leftOperand, int rightOperand, char operatorSymbol, int *errorFlag) {
    switch (operatorSymbol) {
        case '+':
            if (leftOperand > INT_MAX - rightOperand) {
                *errorFlag = 1;
                printf("Error: Integer overflow.\n");
                return 0;
            }
            return leftOperand + rightOperand;
        case '-':
            return leftOperand - rightOperand;
        case '*':
            if (rightOperand != 0 && leftOperand > INT_MAX / rightOperand) {
                *errorFlag = 1;
                printf("Error: Integer overflow.\n");
                return 0;
            }
            return leftOperand * rightOperand;
        case '/':
            if (rightOperand == 0) {
                *errorFlag = 1;
                printf("Error: Division by zero.\n");
                return 0;
            }
            return leftOperand / rightOperand;
        default:
            return 0;
    }
}

void processOperatorStack(int *numbers, int *numberTop, char *operators, int *operatorTop, int *errorFlag) {
    int rightOperand = numbers[(*numberTop)--];
    int leftOperand = numbers[(*numberTop)--];
    char operatorSymbol = operators[(*operatorTop)--];
    int result = performOperation(leftOperand, rightOperand, operatorSymbol, errorFlag);
    numbers[++(*numberTop)] = result;
}

int evaluateExpression(const char *expression, int *errorFlag) {
    int numbers[MAX_STACK_SIZE];
    char operators[MAX_STACK_SIZE];
    int numberTop = -1, operatorTop = -1;

    for (int i = 0; expression[i] != '\0'; i++) {
        if (isspace(expression[i])) continue;

        if (isdigit(expression[i])) {
            int number = 0;
            while (isdigit(expression[i])) {
                number = number * 10 + (expression[i] - '0');
                i++;
            }
            i--;
            numbers[++numberTop] = number;
        } else if (expression[i] == '+' || expression[i] == '-' ||
                   expression[i] == '*' || expression[i] == '/') {
            while (operatorTop >= 0 &&
                   getOperatorPrecedence(operators[operatorTop]) >= getOperatorPrecedence(expression[i])) {
                processOperatorStack(numbers, &numberTop, operators, &operatorTop, errorFlag);
            }
            operators[++operatorTop] = expression[i];
        } else {
            *errorFlag = 1;
            printf("Error: Invalid character '%c' in expression.\n", expression[i]);
            return 0;
        }
    }

    while (operatorTop >= 0) {
        processOperatorStack(numbers, &numberTop, operators, &operatorTop, errorFlag);
    }

    return numbers[numberTop];
}

int main() {
    char expression[MAX_EXPRESSION_SIZE];
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);

    int errorFlag = 0;
    int result = evaluateExpression(expression, &errorFlag);

    if (!errorFlag) {
        printf("Result: %d\n", result);
    }

    return 0;
}
