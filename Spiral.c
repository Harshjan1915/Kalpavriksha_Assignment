#include <stdio.h>

#define MAX 20 

void generateSpiralMatrix(int n, int matrix[MAX][MAX]) {
    int startRow = 0, endRow = n - 1;
    int startCol = 0, endCol = n - 1;
    int num = 1;

    while (startRow <= endRow && startCol <= endCol) {

        // (left → right)
        for (int col = startCol; col <= endCol; col++)
            matrix[startRow][col] = num++;
        startRow++;

        // (top → bottom)
        for (int row = startRow; row <= endRow; row++)
            matrix[row][endCol] = num++;
        endCol--;

        // (right → left)
        if (startRow <= endRow) {
            for (int col = endCol; col >= startCol; col--)
                matrix[endRow][col] = num++;
            endRow--;
        }

        // (bottom → top)
        if (startCol <= endCol) {
            for (int row = endRow; row >= startRow; row--)
                matrix[row][startCol] = num++;
            startCol++;
        }
    }
}

void printMatrix(int n, int matrix[MAX][MAX]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%3d ", matrix[i][j]);
        printf("\n");
    }
}

int main() {
    int n;
    int matrix[MAX][MAX];

    printf("Enter the value of n (1-20): ");
    scanf("%d", &n);

    if (n < 1 || n > 20) {
        printf("Invalid input. n must be between 1 and 20.\n");
        return 1;
    }

    generateSpiralMatrix(n, matrix);
    printf("\nSpiral Matrix:\n");
    printMatrix(n, matrix);

    return 0;
}
