#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <windows.h>


int compareIntegers(const void *firstElement, const void *secondElement) {
  return (*(int *)firstElement - *(int *)secondElement);
}
void readInputArray(int *numbers, int arraySize) {
  for (int i = 0; i < arraySize; i++) {
    scanf("%d", &numbers[i]);
  }
}

void printArray(int *numbers, int arraySize) {
  for (int i = 0; i < arraySize; i++) {
    printf("%d\t", numbers[i]);
  }
  printf("\n");
}

void writeArrayToFile(const char *filename, int *numbers, int arraySize) {
  FILE *dataFile = fopen(filename, "w");
  if (dataFile == NULL) {
    perror("Error opening file for writing");
    exit(1);
  }
  fwrite(numbers, sizeof(int), arraySize, dataFile);
  fclose(dataFile);
}

void readArrayFromFile(const char *filename, int *numbers, int arraySize) {
  FILE *dataFile = fopen(filename, "r");
  if (dataFile == NULL) {
    perror("Error opening file for reading");
    exit(1);
  }
  fread(numbers, sizeof(int), arraySize, dataFile);
  fclose(dataFile);
}

void executeChildProcess(const char *filename, int arraySize) {
  int *numbers = (int *)malloc(arraySize * sizeof(int));
  if (numbers == NULL) {
    perror("Malloc failed");
    exit(1);
  }

  readArrayFromFile(filename, numbers, arraySize);

  qsort(numbers, arraySize, sizeof(int), compareIntegers);

  writeArrayToFile(filename, numbers, arraySize);

  free(numbers);
}

void executeParentProcess(const char *filename, int *numbers, int arraySize) {
  writeArrayToFile(filename, numbers, arraySize);

  wait(NULL);

  readArrayFromFile(filename, numbers, arraySize);

  printf("After sorting:\n");
  printArray(numbers, arraySize);
}

int main() {
  int arraySize;
  if (scanf("%d", &arraySize) != 1) {
    fprintf(stderr, "Error reading array size\n");
    return 1;
  }

  int numbers[arraySize];

  readInputArray(numbers, arraySize);

  printf("Before sorting:\n");
  printArray(numbers, arraySize);

  int processId = fork();

  if (processId < 0) {
    perror("Fork failed");
    return 1;
  } else if (processId == 0) {
    Sleep(1);
    executeChildProcess("data.txt", arraySize);
  } else {
    executeParentProcess("data.txt", numbers, arraySize);
  }
  return 0;
}
