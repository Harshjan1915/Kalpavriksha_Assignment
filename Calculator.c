#include <stdio.h>
#include <ctype.h>
#include <string.h>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int applyOp(int a, int b, char op, int *err) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) {
            *err = 1; // divide by zero
            return 0;
        }
        return a / b; 
    }
    return 0;
}

int main() {
    char exp[200];
    printf("Enter expression: ");
    fgets(exp, sizeof(exp), stdin);

    int numStack[100], nsTop = -1;
    char opStack[100]; int osTop = -1;
    int i = 0, error = 0;

    while (exp[i] != '\0' && exp[i] != '\n') {
        if (exp[i] == ' ') {
            i++;
            continue;
        }

        if (isdigit(exp[i])) {
            int val = 0;
            while (isdigit(exp[i])) {
                val = val * 10 + (exp[i] - '0');
                i++;
            }
            numStack[++nsTop] = val;
            continue;
        }
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            while (osTop != -1 && precedence(opStack[osTop]) >= precedence(exp[i])) {
                int b = numStack[nsTop--];
                int a = numStack[nsTop--];
                char op = opStack[osTop--];
                int res = applyOp(a, b, op, &error);
                if (error) {
                    printf("Error: Division by zero.\n");
                    return 0;
                }
                numStack[++nsTop] = res;
            }
            opStack[++osTop] = exp[i];
        }
        else {
            printf("Error: Invalid expression.\n");
            return 0;
        }
        i++;
    }

    while (osTop != -1) {
        int b = numStack[nsTop--];
        int a = numStack[nsTop--];
        char op = opStack[osTop--];
        int res = applyOp(a, b, op, &error);
        if (error) {
            printf("Error: Division by zero.\n");
            return 0;
        }
        numStack[++nsTop] = res;
    }

    printf("%d\n", numStack[nsTop]);
    return 0;
}
